#!/bin/bash

if [ "$#" -lt 3 ]; then
    echo "Erreur: Paramètre manquant."
    exit 1
fi

if [ ! -f "$1" ]; then
    echo "Erreur: Le fichiers de donnée n'existe pas ou n'a pas été trouvée."
    exit 1
fi

