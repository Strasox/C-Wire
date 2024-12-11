#!/bin/bash

# Début du calcule du temps d'execution
debut=$(date +%s)

# Fonction pour afficher l'aide (-h)
affiche_-h(){
    echo "Aide : $0 [-h] <chemin_du_fichier> <type_de_station> <type_de_consommateur> [Identifiant_de_centrale]"
    echo
    echo "Description :"
    echo "Ce script permet de traiter les informations d'un fichier selon les paramètre de l'utilisateur de crée un nouveau fichier avec ces résultat."
    echo  
    echo "Options :"
    echo " [-h] (Optionnel) Affiche l'aide de cette commande et ignore les autres options."
    echo " <chemin_du_fichier> (Obligatoire) Chemin indiquant l'endroit du fichier à traiter."
    echo " <type_de_station> (Obligatoire) Type de station à traiter (hvb, hva, lv)"
    echo " <type_de_consommateur> (Obligatoire) Type de consommateur à traiter (comp, indiv, all)"
    echo " [Identifiant_de_centrale] (Optionnel) Affiche les résultats pour une centrale spécifique ou, si laissé vide, pour toutes les centrales." 
    echo
    echo "Limitation :"
    echo " Les options (hvb, hva) ne sont pas compatible avec (all, indiv) et ne sont donc pas autoriser."
    echo "--------------------"
}

# Fonction pour afficher le temps d'execution du fichier 
affiche_temps(){
    fin=$(date +%s)
    duree=$(( $fin - $debut ))
    echo "[$duree sec]"
}

## On verifie les argument entrer par l'utilisateur
 
# On verifie si l'un des parametre est -h
for option in "$@"; do
    if [[ "$option" == "-h" ]]; then
        affiche_-h
        affiche_temps
        exit 0
    fi
done

# On verifie le nombre de parametre
if [ "$#" -lt 3 ]; then
    affiche_-h
    affiche_temps
    echo "Erreur: Parametre manquant."
    exit 1
fi

# On verifie que le 1er parametre est un fichier et si on le trouve.
if [ ! -f "$1" ]; then
    affiche_-h
    affiche_temps
    echo "Erreur: Le premier parametre n'est pas un fichier ou est introuvable."
    exit 2
fi

# On verifie que le 2eme parametre est 'hub','hva' ou 'lv'
if [[ "$2" != "hvb" && "$2" != "hva" && "$2" != "lv" ]]; then
    affiche_-h
    affiche_temps
    echo "Erreur: Le parametre type de station n'est pas 'hub','hva' ou 'lv' et n'est donc pas valide."
    exit 3
fi

# On verifie que le 3eme parametre est 'comp','indiv' ou 'all'
if [[ "$3" != "comp" && "$3" != "indiv" && "$3" != "all" ]]; then
    affiche_-h
    affiche_temps
    echo "Erreur: Le parametre type de consomateur n'est pas 'comp','indiv' ou 'all' et n'est donc pas valide."
    exit 4
fi

# On verifie que les 2eme et 3eme parametre sont compatible
if [[ ("$2" == "hvb" || "$2" == "hva") && ( "$3" == "all" || "$3" == "indiv" ) ]]; then
    affiche_-h
    affiche_temps
    echo "Erreur: Le parametre type de station et type de consomateur ne sont pas compatible."
    exit 5
fi

# On verifie si le numéro de station est en parametre
if [ ! -z "$4" ];then
    # On verifie que le parametre est un int (StackOverflow)
    if ! [[ "$4" =~ ^[0-9]+$ ]]; then 
        affiche_-h
        affiche_temps
        echo "Erreur: Le numéro de la station doit être un entier."
        exit 7
    fi

    # On verifie qu'il est supperieur ou egal a 1
    if [ "$4" -lt 1 ]; then
        affiche_-h
        affiche_temps
        echo "Erreur: Le numero de la station doit etre positif."
        exit 7
    fi
fi


