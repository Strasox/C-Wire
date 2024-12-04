#!/bin/bash

## On verifie les argument entrer par l'utilisateur

# Fonction pour afficher l'aide (-h)
affiche_-h(){
    echo "Usage : $0 [options] [paramètres]"
    echo
    echo "Description :"
    echo "Ce script permet de traiter les informations des stations et des centrales."
    echo
    echo "Options :"
    echo "  -h        Affiche cette aide et ignore les autres options."
    echo "  <id_centrale>  (optionnel) Filtre les résultats pour une centrale spécifique."
    echo "  <num_station>  (optionnel) Numéro de la station à traiter (doit être un entier positif)."
    echo
    echo "Exemple d'utilisation :"
    echo "  $0 -h"
    echo "  $0 12345"
    echo "  $0 12345 5"
    exit 0
}

# On verifie si l'un des parametre est -h
for argument in "$*"; do
    if [[ "$argument" == "-h" ]]; then
        affiche_-h
    fi
done



# On verifie le nombre de parametre
if [ "$#" -lt 3 ]; then
    echo "Erreur: Parametre manquant."
    exit 1
fi

# On verifie que le 1er parametre est un fichier et donc si il existe.
if [ ! -f "$1" ]; then
    echo "Erreur: La premier parametre n'est pas un fichier ou n'existe pas."
    exit 2
fi


### Piste amélioration : posibilite de faire un case ?


# On verifie que le 2eme parametre est 'hub','hva' ou 'lv'


if [[ "$2" != "hvb" && "$2" != "hva" && "$2" != "lv" ]]; then
    echo "Erreur: Le deuxieme parametre n'est pas 'hub','hva' ou 'lv'."
    exit 3
fi

if [[ "$3" != "comp" && "$3" != "indiv" && "$3" != "all" ]]; then
    echo "Erreur: Le deuxieme parametre n'est pas 'comp','indiv' ou 'all'."
    exit 4
fi



# On verifie que le 3eme parametre n'est pas interdite
if [[ "$2" == "hvb" && ( "$3" == "all" || "$3" == "indiv" ) ]]; then
    echo "Erreur: Le troisieme parametre ne peut pas etre all ou indiv avec hvb."
    exit 5
fi

if [[ "$2" == "hva" && ( "$3" == "all" || "$3" == "indiv" ) ]]; then
    echo "Erreur: Le troisieme parametre ne peut pas etre all ou indiv avec hva."
    exit 6
fi

# On verifie si il y a le 4eme parametre et qu'il est possible

if [ ! -z "$4" ];then
    # On verifie que le parametre est un int
    if ! [[ "$4" =~ ^[0-9]+$ ]]; then
        echo "Erreur: Le numéro de la station doit être un entier."
        exit 7
    fi
    # On verifie qu'il est positif
    if [ "$4" -lt 1 ]; then
        echo "Erreur: Le numero de la station doit etre positif."
        exit 7
    fi
fi


