typedef struct station
{
    int centrale;
    int hvb;
    int hva;
    int lv;
    int compagnie;
    int capacite;
    int conso;

}Station;

typedef struct avl_struct
{
    Station *val;             // La valeur du nœud
    int eq;                // Facteur d'équilibre (balance factor)
    struct avl_struct *fg; // Pointeur vers le fils gauche
    struct avl_struct *fd; // Pointeur vers le fils droit
} AVL;