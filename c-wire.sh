#!/bin/bash

# Début du calcule du temps d'execution en nanosecondes 
temps_debut=$(date +%s%1N)

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
    # On prend le temps maintenant, en fin de programme en seconde et nanoseconde
    temps_fin=$(date +%s%1N)
    # On soustrait le temps du début à celui de maintenant que l'on divise par 10 pour la décimal et comme on fait une opération sur float on doit envoyer à bc 
    temps=$(bc <<< "scale=1; ($temps_fin - $temps_debut) / 10")
    # Affichage avec la virgule et le 0 toujours afficher et sans retour à la ligne
    printf "[%.1f sec] " $temps
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

# Verification de l'existance de l'executable c 
if [ ! -x "codeC/exec" ]; then
    # Verification de l'existance du makefile
    if (! make -C codeC); then
        echo "Erreur: Le makefile n'existe pas."
        affiche_temps
        exit 8
    fi

    # Execution du makefile
    make -C codeC --no-print-directory

    # Verification de l'existance de l'executable c 
    if [ ! -x "codeC/exec" ]; then
        echo "Erreu: Le fichier executable C est introuvable et ne peut pas etre cree."
        affiche_temps
        exit 9
    fi
fi

# Vérification présence du dossier tmp et création si il n'existe pas 
if [ ! -d "tmp" ]; then 
    mkdir tmp
else
    rm -rf tmp/*
fi

# Vérification de la présence de graphs et création si il n'existe pas
if [ ! -d "graphs" ]; then
    mkdir graphs
fi

# On regarde l'option 4 des centrale
if [ -z "$4" ]; then
    # On cree un fichier avec les donner a traiter pour tout les centrale
    grep "$2" "$1" > "tmp/donner_a_traiter.cvs"
else
    # On cree un fichier avec les donner a traiter pour tout les centrale
    grep "$2" "$1" | grep "$4" > "tmp/donner_a_traiter.cvs"
fi

# Vérification si le fichier temporaire contient des données
if [ ! -e "tmp/donner_a_traiter.cvs" ]; then
    echo "Erreur: Le fichier n'a pas ete cree."
    affiche_temps
    exit 10
fi

codeC/exec "tmp/donner_a_traiter.cvs" > tmp/resultats.csv
if [ $? -ne 0 ]; then
    echo "Erreur: Le programme C a eu une erreur."
    affiche_temps
    exit 11
fi





affiche_temps
echo "Fin du Programme"
exit 0