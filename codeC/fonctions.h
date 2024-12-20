#ifndef FUNCTIONS_H
#define FUNCTIONS_H


#include <stdio.h>
#include <stdlib.h>
#include <string.h>


// Fonction pour obtenir le maximum
int max(int a, int b);

// Fonction pour obtenir le minimum 
int min(int a, int b);

AVL* creerAVL(Station* s);

// Rotation gauche pour rééquilibrer un AVL
AVL* rotationGauche(AVL* abr);

// Rotation droite pour rééquilibrer un AVL
AVL* rotationDroite(AVL* abr);

AVL* doubleRotationDroite(AVL* abr);

AVL* doubleRotationGauche(AVL* abr);

AVL* equilibrerAVL(AVL* abr);

AVL* insertionAVL(AVL* abr,Station* s,int* h);

// Suppression du minimum
AVL* suppMinAVL(AVL* abr, int* h, Station** stationMin);

// Suppression
AVL* suppressionAVL(AVL* abr, int id, int* h);

void insertionStation(AVL **abr, int colonneStation);

//Recherche dans un AVL une station et modifie dans sa structure la consommation de la station
AVL* modifierAVL(AVL* abr, int idStation, int somme);

//Calcul la consommation d'un type de station en ddonnant la somme de tout les consommateurs
void calculConso(AVL* abr, int colonneStation);

// Affichage de l'AVL (ChatGPT pour vérifier si l'AVL fonctionne)
void afficherAVL(AVL* a, int niveau);

void interface(int argc, char *argv[]);


#endif