#include <stdio.h>

int main(){
    printf("Hello World!");
    return 0;
}

// Fonction pour calculer la hauteur d'un arbre
int hauteur(Noeud* A) {
    if (A == NULL){
       return 0;
    }
    int hGauche = hauteur(A->gauche);
    int hDroit = hauteur(A->droit);
    return (hGauche > hDroit) ? hGauche + 1 : hDroit + 1;
}

// Rotation gauche (simple)
Noeud* RotationGauche(Noeud* A) {
    Noeud* B = A->droit;
    A->droit = B->gauche;
    B->gauche = A;
    A->equilibre = hauteur(A->gauche) - hauteur(A->droit);
    B->equilibre = hauteur(B->gauche) - hauteur(B->droit);
    return B;
}

// Rotation droite (simple)
Noeud* RotationDroite(Noeud* A) {
    Noeud* B = A->gauche;
    A->gauche = B->droit;
    B->droit = A;
    A->equilibre = hauteur(A->gauche) - hauteur(A->droit);
    B->equilibre = hauteur(B->gauche) - hauteur(B->droit);
    return B;
}

// Double rotation droite (gauche-droite)
Noeud* DoubleRotationDroite(Noeud* A) {
    A->gauche = RotationGauche(A->gauche);
    return RotationDroite(A);
}

// Double rotation gauche (droite-gauche)
Noeud* DoubleRotationGauche(Noeud* A) {
    A->droit = RotationDroite(A->droit);
    return RotationGauche(A);
}

// Fonction d'équilibrage
void equilibrerAVL(Noeud** A) {
    if (*A == NULL) return;

    if ((*A)->equilibre > 1) {
        if ((*A)->gauche->equilibre < 0) {
            *A = DoubleRotationDroite(*A);
        } else {
            *A = RotationDroite(*A);
        }
    } else if ((*A)->equilibre < -1) {
        if ((*A)->droit->equilibre > 0) {
            *A = DoubleRotationGauche(*A);
        } else {
            *A = RotationGauche(*A);
        }
    }

    (*A)->equilibre = hauteur((*A)->gauche) - hauteur((*A)->droit);
}

// Insertion dans l'arbre AVL
Noeud* insertionAVL(Noeud* A, int e, int* h) {
    if (A == NULL) {
        *h = 1;
        return creerArbre(e);
    }

    if (e < A->elem) {
        A->gauche = insertionAVL(A->gauche, e, h);
        if (*h) {
            A->equilibre--;
        }
    } else if (e > A->elem) {
        A->droit = insertionAVL(A->droit, e, h);
        if (*h) {
            A->equilibre++;
        }
    } else {
        *h = 0;  // L'élément est déjà dans l'arbre, pas besoin de l'ajouter
    }

    // Rééquilibrage de l'arbre après insertion
    if (A->equilibre > 1 || A->equilibre < -1) {
        equilibrerAVL(&A);
    }

    *h = 0;
    return A;
}

// Fonction pour trouver le noeud avec la valeur minimale dans un sous-arbre
Noeud* minimum(Noeud* A) {
    while (A->gauche != NULL) A = A->gauche;
    return A;
}

// Suppression d'un noeud dans un arbre AVL
Noeud* suppAVL(Noeud* A, int e, int* h) {
    if (A == NULL) return NULL;

    if (e < A->elem) {
        A->gauche = suppAVL(A->gauche, e, h);
    } else if (e > A->elem) {
        A->droit = suppAVL(A->droit, e, h);
    } else {
        if (A->gauche == NULL || A->droit == NULL) {
            Noeud* temp = A->gauche ? A->gauche : A->droit;
            if (temp == NULL) {
                temp = A;
                A = NULL;
            } else {
                *A = *temp;
            }
        } else {
            Noeud* temp = minimum(A->droit);
            A->elem = temp->elem;
            A->droit = suppAVL(A->droit, temp->elem, h);
        }
    }

    if (A == NULL) return NULL;

    A->equilibre = hauteur(A->gauche) - hauteur(A->droit);
    if (A->equilibre > 1 || A->equilibre < -1) {
        equilibrerAVL(&A);
    }

    *h = 0;
    return A;
}

// Fonction d'affichage de l'arbre en préordre (pour tester)
void afficherArbre(Noeud* A) {
    if (A == NULL) return;
    printf("%d (Equilibre: %d)\n", A->elem, A->equilibre);
    afficherArbre(A->gauche);
    afficherArbre(A->droit);
}

// Fonction principale pour tester
int main() {
    Noeud* arbre = NULL;
    int h = 0;

    // Insertion d'éléments dans l'arbre AVL
    arbre = insertionAVL(arbre, 10, &h);
    arbre = insertionAVL(arbre, 20, &h);
    arbre = insertionAVL(arbre, 30, &h);
    arbre = insertionAVL(arbre, 15, &h);
    arbre = insertionAVL(arbre, 25, &h);
    arbre = insertionAVL(arbre, 5, &h);

    // Affichage de l'arbre après insertion
    printf("Arbre après insertion:\n");
    afficherArbre(arbre);

    // Suppression d'un élément
    h = 0;
    arbre = suppAVL(arbre, 20, &h);

    // Affichage de l'arbre après suppression
    printf("\nArbre après suppression de 20:\n");
    afficherArbre(arbre);

    return 0;
}