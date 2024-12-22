# C-Wire

## Utilisation

Pour exécuter l'application, utilisez la commande suivante :

```
./c-wire.sh [-h] <chemin_du_fichier> <type_de_station> <type_de_consommateur> [identifiant_de_centrale]
```

Les fichiers résultat sont au départ trié par station, mais il est possible de trié par capacité en enlevant le # à la ligne 223 du fichier `c-wire.sh`.
### Options

- `[-h]` *(Optionnel)* : Affiche l'aide de cette commande et ignore les autres options.
- `<chemin_du_fichier>` *(Obligatoire)* : Spécifie le chemin du fichier à traiter.
- `<type_de_station>` *(Obligatoire)* : Type de station à traiter (hvb, hva, lv).
- `<type_de_consommateur>` *(Obligatoire)* : Type de consommateur à traiter (comp, indiv, all).
- `[identifiant_de_centrale]` *(Optionnel)* : Affiche les résultats pour une centrale spécifique. Si laissé vide, tous les résultats de centrales seront inclus.

### Limitation

Les options `(hvb, hva)` ne sont **pas compatibles** avec `(all, indiv)` et ne sont donc **pas autorisées**.

---

## Auteurs

- **Alex Mikolajewski** - [GitHub](https://github.com/Strasox)
- **Alexis Guimbard** - [GitHub](https://github.com/AlexisG115)
