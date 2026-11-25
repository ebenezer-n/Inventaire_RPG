# RESET_01 - J3.B : Inventaire RPG

## 📝 Description du projet
Ce projet est un système de gestion d'inventaire pour un jeu de rôle, développé en langage C dans le cadre du sprint **RESET_01 (J3.B)**. 
Il permet à un personnage de ramasser des objets au fil de son aventure. L'inventaire n'ayant pas de taille fixe connue à l'avance, le programme repose sur une **liste chaînée générique** (allocation dynamique) garantissant une utilisation optimale de la mémoire et une gestion sans fuite.

**Règle métier principale :** Le personnage ne peut pas porter plus de `POIDS_MAX` (10 000 g / 10 kg).

## ✨ Fonctionnalités
* **Menu interactif sécurisé** : Utilisation exclusive de `fgets` (interdiction de `scanf` et `gets`).
* **Ramasser** : Ajout d'un objet en vérifiant que le poids total ne dépasse pas la limite autorisée.
* **Jeter** : Suppression d'un objet par son nom et libération immédiate de sa mémoire.
* **Lister** : Affichage détaillé des objets et du poids total porté.
* **Quitter** : Libération complète de toute la mémoire restante avant de fermer le programme.

### 🎁 Bonus implémentés
* **B1 - Fusion automatique** : Ramasser un objet déjà présent augmente sa quantité au lieu de créer un doublon.
* **B2 - Tri décroissant** : L'inventaire s'affiche du poids le plus lourd au plus léger (tri à bulles manuel, sans `qsort`).

## 📋 Prérequis
Pour compiler et exécuter ce projet, vous avez besoin de :
* Un compilateur C (`gcc`).
* L'outil `make` (ou `mingw32-make` sous Windows).
* *(Optionnel)* `valgrind` pour la vérification des fuites de mémoire (sous Linux).

## 📂 Structure du projet
```text
B_inventaire_rpg/
├── Makefile
├── list.h               # En-tête de la liste chaînée générique
├── objet.h              # En-tête de la structure métier (t_objet)
├── ft_lstnew.c          # Création d'un maillon
├── ft_lstadd_back.c     # Ajout en fin de liste
├── ft_lstsize.c         # Comptage des maillons
├── ft_lstdelone.c       # Suppression d'un maillon
├── ft_lstclear.c        # Nettoyage complet de la liste
├── ft_lstiter.c         # Parcours de la liste
└── main.c               # Logique principale et menu interactif

# 🛠️ Compilation et Exécution
Sous Linux / macOS
bash
make re
./inventaire_rpg
Sous Windows (MinGW)
bash
mingw32-make re
.\inventaire_rpg.exe

# 💻 Exemple d'utilisation
Une fois lancé, le programme affiche le menu suivant :

text
===== INVENTAIRE RPG =====
1. Ramasser un objet
2. Jeter un objet
3. Lister l'inventaire
4. Quitter
Choix: 
Si vous tapez 1, le programme vous demandera le nom, le poids et la quantité. S'il y a un dépassement, il affichera par exemple :

text
Refus: depassement de 1 g (actuel 1000 g + 9001 g > max 10000 g).

# 🛡️ Gestion de la Mémoire (Points clés)
Le projet a été conçu pour garantir zéro fuite mémoire (critères #13, #14, #15 de la grille) :

Vérification avant allocation : Le poids est calculé avant le malloc. Si l'objet est refusé, aucune mémoire n'est allouée inutilement.

Fonction de rappel (del) : La liste générique ne sachant pas ce qu'elle transporte, la fonction del lui indique comment libérer le contenu.

Nettoyage systématique : ft_lstclear est appelé à la fermeture pour détruire chaque maillon et son contenu.

Vérification avec Valgrind :

bash
valgrind --leak-check=full ./inventaire_rpg
# Résultat attendu : "All heap blocks were freed -- no leaks are possible"

# 👤 Auteur
Nom : NAMBIMAN Eben-Ezer

Projet : RESET_01 - J3.B - Inventaire RPG
