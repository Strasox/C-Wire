#ifndef STRUCTURES_H
#define STRUCTURES_H



#include <stdio.h>
#include <stdlib.h>


typedef struct 
{
    int identifiant;  // Identifiant d'un station
    long int capacite;     // Capacité d'une station
    long int consommation; // Consomation à calculer d'une station
}Station;

typedef struct avl
{
    Station* station;   // Pointeur vers une station
    struct avl* fg;     // Pointeur vers le fils gauche
    struct avl* fd;     // Pointeur vers le fils droit
    int eq;             // Facteur d'équilibre 
} AVL;




#endif