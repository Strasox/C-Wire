#include <stdio.h>
#include <stdlib.h>

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

// Fonction pour obtenir le maximum de deux nombres
int max(int a, int b) {
    return (a > b) ? a : b;
}

// Fonction pour obtenir le minimum de deux nombres
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
    int eq_p = abr->eq;

    // On rééquilibre l'AVL
    abr->fd = p->fg;
    p->fg = abr;

    // On rééquilibre les noeud 
    abr->eq = eq_a - max(eq_p, 0) - 1;
    p->eq = min( min(eq_a-2, eq_a+eq_p-2), eq_p-1 );
    return p;
}

// Rotation droite pour rééquilibrer un AVL
AVL* rotationDroite(AVL* abr){
    // On initialise le pivot
    AVL* p = abr->fg;
    int eq_a = abr->eq;
    int eq_p = abr->eq;

    // On rééquilibre l'AVL
    abr->fg = p->fd;
    p->fd = abr;

    // On rééquilibre les noeud 
    abr->eq = eq_a - min(eq_p, 0) + 1;
    p->eq = max( max(eq_a+2, eq_a+eq_p+2), eq_p+1 );
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
    //Si l'AVL est penche trop à droite
    if(abr->eq >= 2){
        // On verifie si le sous arbre droit ne penche pas a gauche
        if(abr->fd->eq >= 0){
            return rotationGauche(abr);
        }else{
            return doubleRotationGauche(abr);
        }
    // Si l'AVL penche trop à gauche
    }else if(abr->eq <= -2){
        // On verifie si le sous arbre gauche ne penche pas a droite
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
        if(abr->eq = 0){
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
        // abr = equilibrerAVL(abr);
        if(abr->eq = 0){
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

//(ChatGPT pour vérifier si l'AVL fonctionne)
int main() {
    AVL* arbre = NULL;
    int h;

    // Insertion de plusieurs stations pour construire un grand AVL
    Station stations[] = {
        {15, 150, 0}, {10, 100, 0}, {20, 200, 0},
        {8, 80, 0}, {12, 120, 0}, {17, 170, 0},
        {25, 250, 0}, {6, 60, 0}, {9, 90, 0},
        {11, 110, 0}, {13, 130, 0}, {16, 160, 0},
        {18, 180, 0}, {23, 230, 0}, {27, 270, 0}
    };

    int n = sizeof(stations) / sizeof(stations[0]);

    // Construire l'AVL avec toutes les stations
    for (int i = 0; i < n; i++) {
        arbre = insertionAVL(arbre, &stations[i], &h);
    }

    // Afficher l'arbre AVL avant suppression
    printf("Avant suppression :\n");
    afficherAVL(arbre, 0);

    // Supprimer certains nœuds
    arbre = suppressionAVL(arbre, 10, &h);
    arbre = suppressionAVL(arbre, 20, &h);
    arbre = suppressionAVL(arbre, 15, &h);

    // Afficher l'arbre AVL après suppression
    printf("Après suppression :\n");
    afficherAVL(arbre, 0);

    return 0;
}