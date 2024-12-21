#include "fonctions.h"
#include "structure.h"

// Fonction pour obtenir le maximum
int max(int a, int b) {
    return (a > b) ? a : b;
}

// Fonction pour obtenir le minimum 
int min(int a, int b) {
    return (a < b) ? a : b;
}

// Créé un oeud AVL avec une station
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

    // On rééquilibre l'équilibre des noeuds 
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

    // On rééquilibre l'équilibre des noeuds 
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

// On rééquilibre l'AVL selon l'équilibre des noeud
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

// On insère une station récursivement dans l'AVL
AVL* insertionAVL(AVL* abr, Station* s, int* h){
    // Si le noeud est vide, on insère la station ici
    if(abr == NULL){
        *h = 1;
        return creerAVL(s);
    // Si l'identifiant de la station est plus petit que celle du noeud
    }else if(s->identifiant < abr->station->identifiant){
        abr->fg = insertionAVL(abr->fg, s, h);
        *h = -*h;
    // Si l'identifiant de la station est plus grand que celle du noeud
    }else if(s->identifiant > abr->station->identifiant){
        abr->fd = insertionAVL(abr->fd, s, h);
    }else{
        *h = 0;
        return abr;
    }
    // On rééquilibre l'AVL
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

// Suppression du minimum de l'AVL récursivement
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
    // On rééquilibre l'AVL
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

// // Fonction pour insérer chaque ligne du fichier des stations dans l'AVL
void insertionStation(AVL **abr, int colonneStation) {
    Station *s;
    int h;
    int colonne = 0;
    char* donnee;
    char ligne[1024];  // Buffer pour lire chaque ligne du fichier
    
    // Ouvre le fichier pour lecture
    FILE* fichier = fopen("tmp/filtre_station.csv", "r");
    if (fichier == NULL) {
        printf("Erreur : fichier vide ou inaccessible.\n");
        exit(0);
    }
    // On récupère chaque ligne que l'on transforme en station
    while (fgets(ligne, sizeof(ligne), fichier)) {
        colonne = 0;  // La colonne ou on est placée
        s = malloc(sizeof(Station));
        if (s == NULL) {
            printf("Erreur d'allocation mémoire pour la station.\n");
            exit(1);
        }
        // Les ligne qui nous intéresse
        donnee = strtok(ligne, ";");
        while (donnee != NULL) {
            
            if (colonne == colonneStation-1) {
                s->identifiant = atoi(donnee);
            }
            
            else if (colonne == 6) {
                s->capacite = atol(donnee);
            }
            colonne++;
            donnee = strtok(NULL, ";");
        }
        s->consommation = 0;  
        *abr = insertionAVL(*abr, s, &h);  // Insertion dans l'AVL
    }
    fclose(fichier);
}

// Recherche dans un AVL une station et modifie dans sa structure la consommation de la station
AVL* modifierAVL(AVL* abr, int idStation, int somme){
    if(abr == NULL){
        printf("erreur : arbre vide");
        exit(1);
    }
    if (abr->station->identifiant == idStation){
        abr->station->consommation = somme;
    }
    else if(abr->station->identifiant < idStation){
        abr->fd = modifierAVL(abr->fd, idStation, somme);
    }
    else{
        abr->fg = modifierAVL(abr->fg, idStation, somme);
    }
    return abr;
}

// Calcul la consommation d'un type de station en donnant la somme de tous les consommateurs
void calculConso(AVL* abr, int colonneStation) {
    int colonne = 0;
    long int somme = 0;      
    int idStation = -1; 
    int tmp = 0;        // Variable pour séparé les consommateurs par station
    char* donnee;
    char ligne[1024];  // Buffer pour lire chaque ligne du fichier

    // Ouvre le fichier pour lecture
    FILE* fichier = fopen("tmp/filtre_consommateur.csv", "r");
    if (fichier == NULL) {
        printf("Erreur : fichier vide ou inaccessible.\n");
        exit(0);
    }
    // Parcours chaque ligne du fichier
    while (fgets(ligne, sizeof(ligne), fichier)) {
        colonne = 0;  // La colonne ou on est placée
        
        donnee = strtok(ligne, ";");
        while (donnee != NULL) {
            
            // Vérifie si on est à la colonne qui contient l'identifiant de la station
            if (colonne == colonneStation - 1) {
                // Si on change de station, on réinitialise la consommation
                if (idStation == -1 || idStation != atoi(donnee)) {
                    idStation = atoi(donnee);  // Met à jour idStation
                    tmp = 0;  // Nouvelle station, réinitialisation de tmp
                    somme = 0;  // Réinitialise la consommation
                } else {
                    tmp = 1;  // Si c'est la même station, tmp est mis à 1
                }
            }

            // Si on est dans la colonne des consommations
            else if (colonne == 7) {
                if (tmp == 0) {
                    abr = modifierAVL(abr, idStation, somme);  // Mofifie l'AVL
                    somme = atol(donnee);  // Si tmp est 0, on initialise la consommation
                } else {
                    somme += atol(donnee);  // Si tmp est 1, on ajoute à la consommation
                }
            }

            colonne++;  // On passe à la colonne suivante
            donnee = strtok(NULL, ";");  // On récupère la prochaine colonne
            
        }
        // Une fois que l'on a terminé, on met à jour l'AVL avec la dernière consommation
        abr = modifierAVL(abr, idStation, somme);  // Insertion dans l'AVL
        
    }

    fclose(fichier);  // Fermeture du fichier après traitement
}

// Création du fichier avec les résultat de l'AVL
void creerFichier(AVL* abr,char* s,char* c,char* central){
    // On crée le nom du fichier
    char nomfichier[50];
    strcpy(nomfichier, "tests/");
    strcat(nomfichier, s);
    strcat(nomfichier, "_");
    strcat(nomfichier, c);
    if(!(strcmp(central, "Vide") == 0)){
        strcat(nomfichier, "_");
        strcat(nomfichier, central);
    }
    strcat(nomfichier, ".csv");
    
    // Ouvre crée le fichier
    FILE* fichier = fopen(nomfichier, "w");
    if (fichier == NULL) {
        printf("Erreur: Impossible de créer le fichier");
        exit(1);
    }

    // On écrit la première ligne dans le fichier selon les paramètres
    if(strcmp(s, "hvb") == 0){
        fprintf(fichier, "%s:", "Station HV-B");
    }else if(strcmp(s, "hva") == 0){
        fprintf(fichier, "%s:", "Station HV-A");
    }else if(strcmp(s, "lv") == 0){
        fprintf(fichier, "%s:", "Station LV");
    }
    fprintf(fichier, "%s:", "Capacité");
    if(strcmp(c, "comp") == 0){
        fprintf(fichier, "%s\n", "Consommation (entreprises)");
    }else if(strcmp(c, "indiv") == 0){
        fprintf(fichier, "%s\n", "Consommation (particuliers)");
    }else if(strcmp(c, "all") == 0){
        fprintf(fichier, "%s\n", "Consommation (tous)");
    }

    Station* temp;
    int h;
    // On remplie le fichier avec l'AVL
    while(abr != NULL){
        abr = suppMinAVL(abr,&h,&temp);
        fprintf(fichier, "%d:", temp->identifiant);
        fprintf(fichier, "%ld:", temp->capacite);
        fprintf(fichier, "%ld\n", temp->consommation);

        free(temp);
    }
    
    fclose(fichier);
}

// Affichage de l'AVL (ChatGPT pour vérifier si l'AVL fonctionne)
void afficherAVL(AVL* a, int niveau) {
    if (a != NULL) {
        afficherAVL(a->fd, niveau + 1);
        for (int i = 0; i < niveau; i++) {
            printf("    ");
        }
        printf("[%d, %ld, %ld]\n", a->station->identifiant,a->station->capacite, a->station->consommation);
        afficherAVL(a->fg, niveau + 1);
    }
}

// Programme principal pour le résultat final
void programme(char *argv[]) {
    AVL* arbre = NULL;
    int s;
    if(strcmp(argv[1], "hvb") == 0){
        s=2;
    }else if(strcmp(argv[1], "hva") == 0){
        s=3;
    }else if(strcmp(argv[1], "lv") == 0){
        s=4;
    }
    
    insertionStation(&arbre, s);  
    calculConso(arbre,s);
    afficherAVL(arbre, 0); // Affiche l'arbre AVL(optionnel)
    creerFichier(arbre,argv[1],argv[2],argv[3]);
}
