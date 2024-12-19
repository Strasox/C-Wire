#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct 
{
    int identifiant;  // Identifiant d'un station
    int capacite;     // Capacité d'une station
    int consommation; // Consomation à calculer d'une station
}Station;

typedef struct avl
{
    Station* station;   // Pointeur vers une station
    struct avl* fg;     // Pointeur vers le fils gauche
    struct avl* fd;     // Pointeur vers le fils droit
    int eq;             // Facteur d'équilibre 
} AVL;

// Fonction pour obtenir le maximum
int max(int a, int b) {
    return (a > b) ? a : b;
}

// Fonction pour obtenir le minimum 
int min(int a, int b) {
    return (a < b) ? a : b;
}

AVL* creerAVL(Station* s)
{
    // Allocation de la mémoire pour un nouveau noeud dans l'AVL d'une station
    AVL* nouv = (AVL*)malloc(sizeof(AVL));

    // Si problème d'allocation, erreur
    if (nouv == NULL)
    {
        printf("Erreur: Problème lors de l'allocation mémoire pour ajouter une nouvelle station à l'AVL.");
        exit(1); 
    }

    // Initialisation des valeurs de base
    nouv->station = s;
    nouv->fg = NULL; // Pas de fils gauche
    nouv->fd = NULL; // Pas de fils droit
    nouv->eq = 0;    // Facteur d'équilibre initialisé à 0
    return nouv;
}


// Rotation gauche pour rééquilibrer un AVL
AVL* rotationGauche(AVL* abr){
    // On initialise le pivot
    AVL* p = abr->fd;
    int eq_a = abr->eq;
    int eq_p = p->eq;

    // On rééquilibre l'AVL
    abr->fd = p->fg;
    p->fg = abr;

    // On rééquilibre les noeuds 
    abr->eq = eq_a - max(eq_p, 0) - 1;
    p->eq = min(min(eq_a - 2, eq_a + eq_p - 2), eq_p - 1);
    return p;
}

// Rotation droite pour rééquilibrer un AVL
AVL* rotationDroite(AVL* abr){
    // On initialise le pivot
    AVL* p = abr->fg;
    int eq_a = abr->eq;
    int eq_p = p->eq;

    // On rééquilibre l'AVL
    abr->fg = p->fd;
    p->fd = abr;

    // On rééquilibre les noeuds 
    abr->eq = eq_a - min(eq_p, 0) + 1;
    p->eq = max(max(eq_a + 2, eq_a + eq_p + 2), eq_p + 1);
    return p;
}

AVL* doubleRotationDroite(AVL* abr){
    abr->fg = rotationGauche(abr->fg);
    return rotationDroite(abr);
}

AVL* doubleRotationGauche(AVL* abr){
    abr->fd = rotationDroite(abr->fd);
    return rotationGauche(abr);
}

AVL* equilibrerAVL(AVL* abr){
    // Si l'AVL est penché trop à droite
    if(abr->eq >= 2){
        // On vérifie si le sous-arbre droit ne penche pas à gauche
        if(abr->fd->eq >= 0){
            return rotationGauche(abr);
        }else{
            return doubleRotationGauche(abr);
        }
    // Si l'AVL penche trop à gauche
    }else if(abr->eq <= -2){
        // On vérifie si le sous-arbre gauche ne penche pas à droite
        if(abr->fg->eq <= 0){
            return rotationDroite(abr);
        }else{
            return doubleRotationDroite(abr);
        }
    }
    return abr;
}

AVL* insertionAVL(AVL* abr,Station* s,int* h){
    if(abr == NULL){
        *h = 1;
        return creerAVL(s);
    }else if(s->identifiant < abr->station->identifiant){
        abr->fg = insertionAVL(abr->fg, s, h);
        *h = -*h;
    }else if(s->identifiant > abr->station->identifiant){
        abr->fd = insertionAVL(abr->fd, s, h);
    }else{
        *h = 0;
        return abr;
    }

    if(*h != 0){
        abr->eq = abr->eq + *h;
        abr = equilibrerAVL(abr);
        if(abr->eq == 0){
            *h = 0;
        }else{
            *h = 1;
        }
    }
    return abr;
}

// Suppression du minimum
AVL* suppMinAVL(AVL* abr, int* h, Station** stationMin) {
    AVL* tmp;
    if (abr->fg == NULL) {
        *stationMin = abr->station;
        *h = -1;
        tmp = abr;
        abr = abr->fd;
        free(tmp);
        return abr;
    }else{
        abr->fg = suppMinAVL(abr->fg, h, stationMin);
        *h = -*h;
    }

    if (*h != 0) {
        abr->eq = abr->eq + *h;
        abr = equilibrerAVL(abr);
        if(abr->eq == 0){
            *h = -1;
        }else{
            *h = 0;
        }
    }
    return abr;
}

// Suppression
AVL* suppressionAVL(AVL* abr, int id, int* h) {
    if (abr == NULL) {
        *h = 0;
        return abr;
    }

    if (id > abr->station->identifiant) {
        abr->fd = suppressionAVL(abr->fd, id, h);
    }else if (id < abr->station->identifiant){
        abr->fg = suppressionAVL(abr->fg, id, h);
        *h = -*h;
    } else {
        if (abr->fd != NULL) {
            Station* stationMin;
            abr->fd = suppMinAVL(abr->fd, h, &stationMin);
            abr->station = stationMin;
        } else {
            AVL* temp = abr;
            abr = abr->fg;
            free(temp);
            *h = -1;
            return abr;
        }
    }

    if (abr == NULL) {
        return NULL;
    }

    if (*h != 0) {
        abr->eq += *h;
        abr = equilibrerAVL(abr);
        if(abr->eq == 0){
            *h = -1;
        }else{
            *h = 0;
        }
    }
    return abr;
}

// Affichage de l'AVL (ChatGPT pour vérifier si l'AVL fonctionne)
void afficherAVL(AVL* a, int niveau) {
    if (a != NULL) {
        afficherAVL(a->fd, niveau + 1);
        for (int i = 0; i < niveau; i++) {
            printf("    ");
        }
        printf("[%d, %d]\n", a->station->identifiant, a->eq);
        afficherAVL(a->fg, niveau + 1);
    }
}

void insertionStation(AVL **abr, int colonneStation) {
    Station *s;
    int h;
    int colonne = 0;
    char* donnee;
    char ligne[1024];  // Buffer pour lire chaque ligne du fichier
    FILE* file = fopen("tmp/filtre_station.csv", "r");
    if (file == NULL) {
        printf("Erreur : fichier vide ou inaccessible.\n");
        exit(0);
    }
    
    while (fgets(ligne, sizeof(ligne), file)) {
        colonne = 0;  // Réinitialisation de la colonne à chaque ligne
        s = malloc(sizeof(Station));
        if (s == NULL) {
            printf("Erreur d'allocation mémoire pour la station.\n");
            exit(1);
        }
        
        donnee = strtok(ligne, ";");
        while (donnee != NULL) {
            // La colonne 1 (index 1) contient l'identifiant de la station
            if (colonne == 1) {
                s->identifiant = atoi(donnee);
            }
            // La colonne 6 (index 6) contient la capacité de la station
            else if (colonne == 6) {
                s->capacite = atoi(donnee);
            }
            colonne++;
            donnee = strtok(NULL, ";");
        }
        s->consommation = 0;  // Par défaut, consommation = 0
        *abr = insertionAVL(*abr, s, &h);  // Insertion dans l'AVL
    }
    fclose(file);
}

int main() {
    AVL* arbre = NULL;
    insertionStation(&arbre, 1);  // Passez l'adresse de l'arbre, la colonne 1 pour l'identifiant (index 1)
    afficherAVL(arbre, 0);  // Affiche l'arbre AVL
    return 0;
}
