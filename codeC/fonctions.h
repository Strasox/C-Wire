#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "structure.h"

// Fonction pour obtenir le maximum
int max(int a, int b);

// Fonction pour obtenir le minimum 
int min(int a, int b);

// Créé un oeud AVL avec une station
AVL* creerAVL(Station* s);

// Rotation gauche pour rééquilibrer un AVL
AVL* rotationGauche(AVL* abr);

// Rotation droite pour rééquilibrer un AVL
AVL* rotationDroite(AVL* abr);

AVL* doubleRotationDroite(AVL* abr);

AVL* doubleRotationGauche(AVL* abr);

// On rééquilibre l'AVL selon l'équilibre des noeud
AVL* equilibrerAVL(AVL* abr);

// On insert une station dans l'AVL
AVL* insertionAVL(AVL* abr,Station* s,int* h);

// Suppression du minimum de l'AVL
AVL* suppMinAVL(AVL* abr, int* h, Station** stationMin);

// Fonction pour insérer chaque ligne du fichier des stations dans l'AVL
void insertionStation(AVL** abr, int colonneStation);

// Recherche dans un AVL une station et modifie dans sa structure la consommation de la station
AVL* modifierAVL(AVL* abr, int idStation, int somme);

// Calcul la consommation d'un type de station en donnant la somme de tout les consommateurs
void calculConso(AVL* abr, int colonneStation);

// Création du fichier avec les résultat de l'AVL
void creerFichier(AVL* abr,char* s,char* c,char* central);

// Affichage de l'AVL (ChatGPT pour vérifier si l'AVL fonctionne)
void afficherAVL(AVL* a, int niveau);

// Programme principale
void programme(char* argv[]);

#endif
