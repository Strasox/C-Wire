#!/bin/bash

## Dans cette section, on initialise les fonctions générales

# Initialisation du temps à 0.0
temps=0.0

# Fonction pour afficher l'aide (-h)
afficher_-h(){
    echo "Aide : $0 [-h] <chemin_du_fichier> <type_de_station> <type_de_consommateur> [identifiant_de_centrale]"
    echo
    echo "Description :"
    echo "Ce script permet de donner la capacité et la consommation de chaque station pour le type sélectionné de station et de consommateur, mais également pour une centrale spécifique ou non."
    echo  
    echo "Options :"
    echo " [-h] (Optionnel) Affiche l'aide de cette commande et ignore les autres options."
    echo " <chemin_du_fichier> (Obligatoire) Chemin indiquant l'endroit du fichier à traiter."
    echo " <type_de_station> (Obligatoire) Type de station à traiter (hvb, hva, lv)."
    echo " <type_de_consommateur> (Obligatoire) Type de consommateur à traiter (comp, indiv, all)."
    echo " [identifiant_de_centrale] (Optionnel) Affiche les résultats pour une centrale spécifique ou, si laissé vide, pour toutes les centrales." 
    echo
    echo "Limitation :"
    echo " Les options (hvb, hva) ne sont pas compatibles avec (all, indiv) et ne sont donc pas autorisées."
    echo "--------------------"
}

# Fonction pour afficher le temps de mis par le filtrage, s'il n'a pas été effectuer, il est de 0.0
afficher_temps(){
    # Afficher jusqu'à la première décimal, on affiche le temps même si il est 0.0
    printf "[%.1f sec] " $temps
}

## Dans cette section, on vérifie les arguments entrer par l'utilisateur
 
# Pour chaque paramètre, on verifie si il s'agit -h
for option in "$@"; do
    if [[ "$option" == "-h" ]]; then
        afficher_-h
        afficher_temps
        exit 0
    fi
done

# On vérifie qu'il ne manque pas de paramètre
if [ "$#" -lt 3 ]; then
    afficher_-h
    afficher_temp
    echo "Erreur: Paramètre manquant."
    exit 1
fi

# On vérifie que le 1er paramètre est un fichier et si on le trouve
if [ ! -f "$1" ]; then
    afficher_-h
    afficher_temp
    echo "Erreur: Le premier paramètre n'est pas un fichier ou est introuvable."
    exit 2
fi

# On vérifie que le 2ème paramètre est 'hvb','hva' ou 'lv'
if [[ "$2" != "hvb" && "$2" != "hva" && "$2" != "lv" ]]; then
    afficher_-h
    afficher_temp
    echo "Erreur: Le paramètre type de station n'est pas 'hvb','hva' ou 'lv' et n'est donc pas valide."
    exit 3
fi

# On vérifie que le 3ème paramètre est 'comp','indiv' ou 'all'
if [[ "$3" != "comp" && "$3" != "indiv" && "$3" != "all" ]]; then
    afficher_-h
    afficher_temp
    echo "Erreur: Le paramètre type de consommateur n'est pas 'comp','indiv' ou 'all' et n'est donc pas valide."
    exit 4
fi

# On vérifie que les 2ème et 3ème paramètre sont compatible
if [[ ("$2" == "hvb" || "$2" == "hva") && ( "$3" == "all" || "$3" == "indiv" ) ]]; then
    afficher_-h
    afficher_temp
    echo "Erreur: Le paramètre type de station et type de consommateur ne sont pas compatible."
    exit 5
fi

# On vérifie si le numéro de central est en paramètre
if [ ! -z "$4" ];then
    # On vérifie que le paramètre est un nombre (StackOverflow)
    if ! [[ "$4" =~ ^[0-9]+$ ]]; then 
        afficher_-h
        afficher_temps
        echo "Erreur: Le numéro de la station doit être un entier."
        exit 7
    fi

    # On vérifie qu'il est supérieur ou égal a 1
    if [ "$4" -lt 1 ]; then
        afficher_-h
        afficher_temps
        echo "Erreur: Le numéro de central doit être positif."
        exit 7
    fi
fi

# Vérification de l'existence de l'exécutable c 
if [ ! -x "codeC/exec" ]; then
    # Vérification de l'existence du makefile
    if (! make -C codeC); then
        echo "Erreur: Le makefile n'existe pas."
        afficher_temps
        exit 8
    fi

    # Exécution du makefile
    make -C codeC 

    # Vérification de l'existence de l'exécutable c 
    if [ ! -x "codeC/exec" ]; then
        echo "Erreu: Le fichier exécutable, C est introuvables et ne peut pas être créés."
        afficher_temps
        exit 9
    fi
fi

# Vérification de la présence du dossier tmp et création s'il n'existe pas
if [ ! -d "tmp" ]; then 
    mkdir tmp
else 
    # On le vide s'il existe déjà
    rm -rf tmp/*
fi

# Vérification de la présence de graphs et création s'il n'existe pas
if [ ! -d "graphs" ]; then
    mkdir graphs
fi

# Vérification de la présence de tests et création si il n'existe pas
if [ ! -d "tests" ]; then
    mkdir tests
fi

## Dans cette section, on filtre le fichier et on lance l'exécutable

# On défini la colonne des station que l'on regarde
case "$2" in
    "hvb") station_colonne=2 ;;
    "hva") station_colonne=3 ;;
    "lv") station_colonne=4 ;;
    *) echo "Erreur: Le paramètre type de station a un problème."
        afficher_temps
        exit 10 ;;
esac

# On défini la colonne des consommateur que l'on regarde
case "$3" in
    "comp") consommateur_colonne=5 ;;
    "indiv") consommateur_colonne=6 ;;
    "all") consommateur_colonne="all" ;;
    *) echo "Erreur: Le paramètre type de consommateur a un problème."
        afficher_temps
        exit 11 ;;
esac

# Début du calcul du temps d'exécution, le temps écoulé depuis l'époque UNIX + la 1er décimale des nanosecondes
temps_debut=$(date +%s%1N)

# On vérifie si le numéro de central est en paramètre
if [ ! -z "$4" ]; then
    # Commande pour filtrer, on saute la 1er ligne puis on filtre selon le numéro de central, puis si la colonne/station qu'on recherche est (différent de -) 
    # dans ce cas la prochaine colonne doit être vide (Car si elle n'est pas vide, il s'agit d'un fils de la station recherché.). On met toutes les stations qui 
    # répondent à ces critères dans un fichier. (Aide : ChatGPT, IBM, Funix, StackOverflow, Redit) 
    awk -F';' -v central="$4" -v station_colonne="$station_colonne" 'NR > 1 && $1 == central && $(station_colonne) != "-" && $(station_colonne+1) == "-" && $8 == "-"' "$1" > tmp/filtre_station.csv

    # Cas spécifique, si on prend tout les consommateur(all)
    if [ $consommateur_colonne == "all" ]; then 
        # On enregistre dans un fichier les consommateurs, selon la central, la colonne de la station recherchée et la consommation doit être différent de rien (-)
        awk -F';' -v central="$4" -v station_colonne="$station_colonne" 'NR > 1 && $1 == central && $(station_colonne) != "-" && $8 != "-"' "$1" > tmp/filtre_consommateur.csv
    else
        # Même chose, mais on a un consommateur spécifique donc on vérifie la colonne des consommateurs.
        awk -F';' -v central="$4" -v station_colonne="$station_colonne" -v consommateur="$consommateur_colonne" 'NR > 1 && $1 == central && $(station_colonne) != "-" && $consommateur != "-"' "$1" > tmp/filtre_consommateur.csv
    fi

    # On prend le temps actuelle
    temps_fin=$(date +%s%1N)
    # On soustrait le temps du début à celui actuelle, on divise ensuite par 10 pour la décimale et comme on fait une opération sur float on doit envoyer à bc 
    temps=$(bc <<< "scale=1; ($temps_fin - $temps_debut) / 10")

    # On exécute exec avec les paramètres du script shell 
    ./codeC/exec $2 $3 $4
else
    # Même chose, mais sans filtrage par central
    awk -F';' -v station_colonne="$station_colonne" 'NR > 1 && $(station_colonne) != "-" && $(station_colonne+1) == "-" && $8 == "-"' "$1" > tmp/filtre_station.csv
    if [ $consommateur_colonne == "all" ]; then 
        awk -F';' -v station_colonne="$station_colonne" 'NR > 1 && $(station_colonne) != "-" && $8 != "-"' "$1" > tmp/filtre_consommateur.csv
    else
        awk -F';' -v station_colonne="$station_colonne" -v consommateur="$consommateur_colonne" 'NR > 1 && $(station_colonne) != "-" && $consommateur != "-"' "$1" > tmp/filtre_consommateur.csv
    fi

    # On prend le temps actuelle
    temps_fin=$(date +%s%1N)
    # On soustrait le temps du début à celui actuelle, on divise ensuite par 10 pour la décimale et comme on fait une opération sur float on doit envoyer à bc 
    temps=$(bc <<< "scale=1; ($temps_fin - $temps_debut) / 10")

    # On exécute exec avec les paramètres du script shell 
    ./codeC/exec $2 $3 Vide
fi

# Vérification du résultat de l'exec
if [ $? != 0 ]; then
    echo "Erreur: L'execution de exec à rencontrer un problème."
    afficher_temps
    exit 12
fi


afficher_temps
echo "Fin du Programme"
exit 0
