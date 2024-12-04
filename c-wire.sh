#!/bin/bash

## On verifie les argument entrer par l'utilisateur

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

if [[ "$3" != "hvb" && "$3" != "hva" && "$3" != "lv" ]]; then
    echo "Erreur: Le deuxieme parametre n'est pas 'hub','hva' ou 'lv'."
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
if [ ! -n "$4"];then
    if [ "$4" gt 0]; then
        echo "Erreur: Le numero de la station doit etre positif."
        exit 7
    fi
fi


