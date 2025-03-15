# Ultimate Tic-Tac-Toe AI en C
### Description

Ce projet implémente une version avancée du Tic-Tac-Toe, connue sous le nom Ultimate Tic-Tac-Toe, en utilisant le langage C. L'objectif principal est de développer une intelligence artificielle capable de jouer stratégiquement en utilisant les algorithmes Minimax et Alpha-Bêta.
### Objectifs

    Développer une IA capable de jouer à Ultimate Tic-Tac-Toe.
    Comparer les performances des algorithmes Minimax et Alpha-Bêta.
    Implémenter une évaluation heuristique pour améliorer la prise de décision.
    Respecter les règles complexes du jeu tout en optimisant la gestion des données.

### Règles du Jeu

    Le jeu se joue sur une grille globale (3x3), contenant chacune 9 sous-grilles (3x3).
    Pour gagner une sous-grille, un joueur doit aligner trois symboles identiques (ligne, colonne ou diagonale).
    Pour remporter la partie, un joueur doit contrôler trois sous-grilles alignées dans la grille globale.
    Contrainte de jeu : Le coup joué dans une sous-case détermine la sous-grille où l'adversaire doit jouer.
    Si la sous-grille ciblée est pleine ou déjà gagnée, l'adversaire peut jouer librement.

### Installation et Compilation
##### Prérequis

    Compilateur C (GCC, Clang, MSVC...)
    Makefile

### Compilation du Projet
##### Avec Makefile

    make

##### Manuellement (sans Makefile)

    gcc -o ultimate_ttt main.c game.c ai.c -std=c11

Exécution du Jeu

    ./Ultimate_TicTacToe
