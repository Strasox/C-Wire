#include <stdio.h>
#include <stdlib.h>

typedef struct {

    int identifiant;  // Identifiant d'un station
    int capacite;     // Capacité d'une station
    int consommation; // Consomation à calculer d'une station

}Station;

typedef struct avl_struct
{
    Station *station;      // pointeur vers les valeurs du nœud
    struct avl_struct *fg; // Pointeur vers le fils gauche
    struct avl_struct *fd; // Pointeur vers le fils droit
    int eq;                // Facteur d'équilibre 
} Noeud;

Noeud* creerAVL(int id, int capacite)
{
    // Alloue de la mémoire pour un nouveau nœud
    Noeud* nouv = (Noeud* )malloc(sizeof(Noeud));
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

/*
// Fonction pour calculer la hauteur d'un arbre
int hauteur(Noeud* abr) {
    if (abr == NULL){
       return -1;
    }
    int hGauche = hauteur(abr->fg);
    int hDroit = hauteur(abr->fd);
    return (hGauche > hDroit) ? hGauche + 1 : hDroit + 1;
}

// Rotation gauche (simple)
Noeud* rotationGauche(Noeud* abr) {
    Noeud* p = abr->fd;
    abr->fd = p->fg;
    p->fg = abr_a;
    abr->eq = hauteur(abr->fg) - hauteur(abr->fd);
    p->eq = hauteur(p->fg) - hauteur(p->fd);
    return p;
}

AVL* rotationGauche(AVL* a)
{
    AVL* pivot = a->fd; // Le fils droit devient le pivot
    int eq_a = a->eq, eq_p = pivot->eq;

    a->fd = pivot->fg; // Le sous-arbre gauche du pivot devient le fils droit de `a`
    pivot->fg = a;     // `a` devient le fils gauche du pivot

    // Mise à jour des facteurs d'équilibre
    a->eq = eq_a - max(eq_p, 0) - 1;
    pivot->eq = min3(eq_a - 2, eq_a + eq_p - 2, eq_p - 1);

    return pivot; // Le pivot devient la nouvelle racine
}

*/

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

