# C-Wire

## Utilisation

Pour exécuter l'application :
    ```
    ./c-wire.sh [-h] <chemin_du_fichier> <type_de_station> <type_de_consommateur> [identifiant_de_centrale]

    Options :
       [-h] (Optionnel) Affiche l'aide de cette commande et ignore les autres options.
       <chemin_du_fichier> (Obligatoire) Chemin indiquant l'endroit du fichier à traiter.
       <type_de_station> (Obligatoire) Type de station à traiter (hvb, hva, lv).
       <type_de_consommateur> (Obligatoire) Type de consommateur à traiter (comp, indiv, all).
       [identifiant_de_centrale] (Optionnel) Affiche les résultats pour une centrale spécifique ou, si laissé vide, pour toutes les centrales.
    
    Limitation :
       Les options (hvb, hva) ne sont pas compatibles avec (all, indiv) et ne sont donc pas autorisées.
    ```
## Auteurs

- **Alex Mikolajewski** - (https://github.com/Strasox)
- 
