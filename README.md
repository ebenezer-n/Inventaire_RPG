
content = r"""# RESET_01 — J4/B : Arborescence

Projet de synthèse consacré à la **récursivité**, à la lecture de **fichiers texte** et à la manipulation dynamique d'une structure arborescente en langage C.

## 1. Présentation

La commande `tree` affiche une arborescence de dossiers sous forme indentée. Ce projet réalise l'opération inverse :

1. lire un fichier texte décrivant une arborescence ;
2. reconstruire cette arborescence en mémoire sous la forme d'un arbre ;
3. parcourir récursivement l'arbre pour calculer des statistiques ;
4. afficher l'arbre et les résultats ;
5. libérer entièrement la mémoire utilisée.

L'objectif principal est de mettre en pratique la **récursion de descente d'arbre**, la gestion dynamique des enfants d'un nœud et la libération récursive d'une structure.

## 2. Objectifs pédagogiques

- Construire une structure arborescente dynamique à partir d'un texte indenté.
- Réaliser un parsing récursif.
- Écrire des fonctions récursives de parcours : comptage, calcul de profondeur et somme.
- Gérer un tableau de pointeurs vers les enfants.
- Libérer toute la structure sans fuite mémoire.

## 3. Environnement et contraintes techniques

### Fonctions autorisées

Le sujet autorise :

- les fonctions de `<stdio.h>` ;
- `malloc` ;
- `free` ;
- `realloc`.

### Contraintes

- Aucune fonction externe de parsing ou de manipulation d'arbre ne doit être utilisée.
- Les fonctions de statistiques de la partie 3 doivent être **récursives**. Une implémentation uniquement itérative n'est pas conforme.
- Le projet utilise l'allocation dynamique.
- La vérification avec `valgrind` est obligatoire.
- L'arbre doit être entièrement libéré avant la fin du programme.

## 4. Format du fichier d'entrée

Le fichier décrit l'arborescence avec une indentation de **deux espaces par niveau**.

Règles de lecture :

- Une ligne qui se termine par `/` représente un dossier.
- Une ligne au format `nom (taille)` représente un fichier.
- La taille d'un fichier est exprimée en octets.
- Les espaces en début de ligne déterminent le niveau du nœud.

### Exemple

```text
projet/
  src/
    main.c (120)
    utils.c (85)
  docs/
    readme.txt (10)
  build.log (5)
```

Dans cet exemple, `projet/` est la racine. Les dossiers `src/` et `docs/` sont ses enfants. Les fichiers sont rattachés au dossier correspondant à leur niveau d'indentation.

## 5. Structure de données

La structure imposée par le sujet est :

```c
typedef struct s_noeud
{
    char            nom[64];
    int             est_dossier;
    int             taille;
    struct s_noeud  *enfants[32];
    int             nb_enfants;
}   t_noeud;
```

| Champ | Rôle |
|---|---|
| `nom` | Nom du fichier ou du dossier |
| `est_dossier` | Indique si le nœud représente un dossier |
| `taille` | Taille du fichier en octets ; vaut `0` pour un dossier |
| `enfants` | Tableau de pointeurs vers les enfants du nœud |
| `nb_enfants` | Nombre d'enfants du nœud |

Le tableau `enfants` permet de relier chaque nœud à ses descendants directs. Le sujet prévoit une capacité de 32 enfants par nœud.

## 6. Fonctionnalités obligatoires

### 6.1 Parsing récursif

Prototype imposé :

```c
t_noeud *parser_arborescence(
    char lignes[][128],
    int nb_lignes,
    int *index,
    int niveau
);
```

La fonction construit un nœud à partir des lignes du fichier.

Son principe de fonctionnement :

1. analyser la ligne désignée par `*index` ;
2. créer le nœud correspondant ;
3. si le nœud est un dossier, examiner les lignes suivantes ;
4. pour chaque ligne enfant dont l'indentation est strictement supérieure au niveau courant, appeler récursivement `parser_arborescence` ;
5. avancer `*index` au fur et à mesure des lignes effectivement consommées ;
6. arrêter le traitement lorsqu'une ligne possède une indentation inférieure ou égale au niveau courant, ou lorsqu'il n'y a plus de lignes.

**Point important :** lorsqu'une ligne d'indentation inférieure ou égale au niveau courant est rencontrée, elle ne doit pas être consommée par l'appel récursif courant. Elle sera traitée par l'appelant.

La racine du fichier doit être gérée comme un cas particulier explicite dans `main` ou dans une fonction dédiée.

### 6.2 Statistiques récursives

Les quatre fonctions suivantes sont demandées :

```c
int compter_fichiers(t_noeud *n);
int profondeur_max(t_noeud *n);
int taille_totale(t_noeud *n);
void afficher_arbre(t_noeud *n, int niveau);
```

Elles doivent toutes traiter le nœud courant puis déléguer le traitement de ses enfants par des appels récursifs à la fonction concernée.

| Fonction | Résultat attendu |
|---|---|
| `compter_fichiers` | Nombre total de fichiers dans le sous-arbre |
| `profondeur_max` | Profondeur maximale sous le nœud |
| `taille_totale` | Somme des tailles de tous les fichiers du sous-arbre |
| `afficher_arbre` | Réaffichage de l'arbre avec une indentation correspondant aux niveaux |

### 6.3 Libération de la mémoire

La fonction suivante est également obligatoire :

```c
void liberer_arbre(t_noeud *n);
```

Elle doit libérer récursivement les enfants, puis le nœud courant. Elle doit être appelée avant la fin du programme afin qu'aucune allocation de l'arbre ne reste non libérée.

## 7. Compilation et exécution

La commande `make` doit produire un exécutable nommé `arborescence`.

Depuis le dossier du projet :

```bash
make
```

Exécution avec le fichier d'exemple :

```bash
./arborescence exemples/projet1.txt
```

Le programme doit lire le chemin du fichier fourni en argument.

### Résultat attendu pour l'exemple du sujet

L'affichage doit présenter l'arborescence, puis les statistiques suivantes :

```text
Nombre de fichiers : 4
Profondeur maximale : 2
Taille totale : 220 octets
```

Ces valeurs correspondent aux quatre fichiers de l'exemple :

- `main.c` : 120 octets ;
- `utils.c` : 85 octets ;
- `readme.txt` : 10 octets ;
- `build.log` : 5 octets.

## 8. Gestion des erreurs

Le programme doit gérer au minimum les situations explicitement prévues par le sujet :

- fichier inexistant ;
- format invalide ;
- indentation incohérente ;
- ligne mal formée.

En cas d'erreur, il doit :

1. afficher un message d'erreur clair ;
2. terminer avec un code de sortie différent de `0` ;
3. libérer les ressources déjà allouées, si nécessaire.

Le sujet ne fixe pas le texte exact des messages d'erreur.

## 9. Vérification avec Valgrind

La vérification mémoire est obligatoire. Après compilation et exécution, lancer par exemple :

```bash
valgrind --leak-check=full --show-leak-kinds=all ./arborescence exemples/projet1.txt
```

Le critère attendu est **zéro fuite mémoire**. Toute allocation effectuée pour construire l'arbre doit être libérée, y compris lorsqu'une erreur survient pendant le traitement.

## 10. Bonus facultatifs

Les améliorations suivantes sont proposées par le sujet, mais ne sont pas obligatoires :

- afficher la taille cumulée à côté de chaque dossier dans l'arbre ;
- rechercher et afficher le fichier le plus lourd de toute l'arborescence.

## 11. Organisation attendue du projet

Le sujet demande l'organisation suivante :

```text
J4/
└── B_arborescence/
    ├── Makefile
    ├── arborescence.h
    ├── parser_arborescence.c
    ├── compter_fichiers.c
    ├── profondeur_max.c
    ├── taille_totale.c
    ├── afficher_arbre.c
    ├── liberer_arbre.c
    ├── main.c
    └── exemples/
        └── projet1.txt
```

Le fichier `Makefile` doit permettre de construire l'exécutable `arborescence` avec la commande `make`.

## 12. Préparation à la soutenance

La soutenance peut notamment porter sur le déroulement exact des appels récursifs de `parser_arborescence` à partir du fichier d'exemple.

Il est donc important de savoir expliquer :

- quel nœud est créé à chaque étape ;
- comment le niveau d'indentation est interprété ;
- à quel moment `*index` avance ;
- pourquoi une ligne de niveau inférieur ou égal n'est pas consommée par l'appel courant ;
- comment les appels récursifs construisent les relations parent-enfant ;
- comment les fonctions de statistiques parcourent l'arbre ;
- dans quel ordre la mémoire est libérée.

## 13. Références du sujet

Ce README est rédigé à partir du document de consignes **« RESET_01 — J4.B — arborescence »**. Pour les règles générales du projet, le sujet renvoie également à `00_Presentation_RESET_01.md`. La grille d'évaluation est indiquée dans `grille_correction.md`.

---

**Projet :** RESET_01 — J4/B — Arborescence  
**Thème :** Récursivité et fichiers texte  
**Langage :** C
