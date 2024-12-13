#include <stdio.h>
#include <stdlib.h>

typedef struct station
{
    int centrale;
    int hvb;
    int hva;
    int lv;
    int compagnie;
    int capacite;
    int conso;

}Station;

typedef struct avl_struct
{
    Station *val;             // pointeur vers les valeurs du nœud
    int eq;                // Facteur d'équilibre 
    struct avl_struct *fg; // Pointeur vers le fils gauche
    struct avl_struct *fd; // Pointeur vers le fils droit
} AVL;

AVL* creerAVL(int e, int f, int g, int h, int i, int j, int k)
{
    // Alloue de la mémoire pour un nouveau nœud
    AVL* new = (AVL* )malloc(sizeof(AVL));
    if (new == NULL)
    {
        exit(EXIT_FAILURE); // Arrêt immédiat en cas d'erreur d'allocation
    }
    new->val->centrale = e;
    new->val->hvb = f;
    new->val->hva = g;
    new->val->lv = h;
    new->val->compagnie = i;
    new->val->capacite = j;
    new->val->conso = k;
    new->fg = NULL; // Pas de fils gauche
    new->fd = NULL; // Pas de fils droit
    new->eq = 0;    // Facteur d'équilibre initialisé à 0
    return new;
}

AVL* equilibrerAVL(AVL* a)
{
    if (a->eq >= 2)
    { // Cas où l'arbre est déséquilibré à droite
        if (a->fd->eq >= 0)
        {
            return rotationGauche(a); // Rotation simple gauche
        }
        else
        {
            return doubleRotationGauche(a); // Double rotation gauche
        }
    }
    else if (a->eq <= -2)
    { // Cas où l'arbre est déséquilibré à gauche
        if (a->fg->eq <= 0)
        {
            return rotationDroite(a); // Rotation simple droite
        }
        else
        {
            return doubleRotationDroite(a); // Double rotation droite
        }
    }
    return a; // Aucun rééquilibrage nécessaire
}


AVL* insertionAVL(AVL* a, int k, int *h)
{
    if (a == NULL)
    {           // Si l'arbre est vide, crée un nouveau nœud
        *h = 1; // La hauteur a augmenté
        return creerAVL(k);
    }
    else if (k < a->val->conso)
    { // Si consommation est inférieure, insérer à gauche
        a->fg = insertionAVL(a->fg, k, h);
        *h = -*h; // Inverse l'impact de la hauteur
    }
    else if (k > a->val->conso)
    { // Si la consommation est plus grande, insérer à droite
        a->fd = insertionAVL(a->fd, k, h);
    }
    else
    { // Élément déjà présent
        *h = 0;
        return a;
    }

    // Mise à jour du facteur d'équilibre et rééquilibrage si nécessaire
    if (*h != 0)
    {
        a->eq += *h;
        a = equilibrerAVL(a);
        *h = (a->eq == 0) ? 0 : 1; // Mise à jour de la hauteur
    }
    return a;
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

