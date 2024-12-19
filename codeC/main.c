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
    Station* station;   // pointeur vers les valeurs du nœud
    struct avl* fg;     // Pointeur vers le fils gauche
    struct avl* fd;     // Pointeur vers le fils droit
    int eq;             // Facteur d'équilibre 
} AVL;



AVL* creerAVL(int id, int capacite)
{
    // Alloue de la mémoire pour un nouveau nœud
    AVL* nouv = (AVL*)malloc(sizeof(AVL));
    if (nouv == NULL)
    {
        exit(1); // Arrêt immédiat en cas d'erreur d'allocation
    }

    // Initialisation des valeurs de base
    nouv->station->identifiant = id;
    nouv->station->capacite = capacite;
    nouv->station->consommation = 0;
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
    p->eq = min( eq_a-2, eq_a+eq_p-2, eq_p-1 );
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
    p->eq = max( eq_a+2, eq_a+eq_p+2, eq_p+1 );
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
        *h = -*h
    }else if(s->identifiant > abr->station->identifiant){
        abr->fd = insertionAVL(abr->fd, s, h);
    }else{
        *h = 0;
        return abr;
    }

    if(*h != 0){
        abr->eq = abr->eq + *h;
        abr = equilibrerAVL(abr)
        if(abr->eq = 0){
            *h = 0;
        }else{
            *h = 1;
        }
    }
    return abr;
}




int sommeConsoHvb(AVL *a, AVL *b, int *somme) //retourne la somme de la consommation de la station 'a'
{
    if (a == NULL)
    {
        return 0;
    }
    if (b->val->centrale != a->val->centrale)
    {
        return 0;
    }
    if (a->val->hvb != NULL && a->val->hvb == b->val->hvb)
    {
        somme += b->val->conso; //somme des grandes entreprises
        if (b->fg != NULL)
        {
            b->fg = sommeConso(a, b->fg, somme);
        }
        if (b->fd != NULL)
        {
            b->fd = sommeConso(a, b->fd , somme);
        }
        return somme;
    }
    else
    {
        return 0;
    }
}

int sommeConsoHva(AVL *a, AVL *b, int *somme) //retourne la somme de la consommation de la station 'a'
{
    if (a == NULL)
    {
        return 0;
    }
    if (b->val->centrale != a->val->centrale)
    {
        return 0;
    }
    if (a->val->hvb != NULL && a->val->hvb == b->val->hvb)
    {
        if ( a->val->hva != NULL && a->val->hva == b->val->hva)
        {
            somme += b->val->conso; //somme de conso 
            if (b->fg != NULL)
            {
                b->fg = sommeConso(a, b->fg, somme);
            }
            if (b->fd != NULL)
            {
                b->fd = sommeConso(a, b->fd , somme);
            }
            return somme;
        }
    }
    else
    {
        return 0;
    }
}

int sommeConsoLv(AVL *a, AVL *b, int *somme) //retourne la somme de la consommation de la station 'a'
{
    if (a == NULL)
    {
        return 0;
    }
    if (b->val->centrale != a->val->centrale)
    {
        return 0;
    }
    if (a->val->hvb != NULL && a->val->hvb == b->val->hvb)
    {
        if ( a->val->hva != NULL && a->val->hva == b->val->hva) //test si la station a existe et si b est issu de la même station que a
        {
            if ( a->val->lv != NULL && a->val->lv == b->val->lv )
            {
                somme += b->val->conso; //somme de conso 
                if (b->fg != NULL)
                {
                    b->fg = sommeConso(a, b->fg, somme);
                }
                if (b->fd != NULL)
                {
                    b->fd = sommeConso(a, b->fd , somme);
                }
                return somme;
            }
        }
    }
    else
    {
        return 0;
    }
}

