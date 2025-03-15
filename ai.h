#ifndef AI_H
#define AI_H

#include "game.h"

// Profondeurs par défaut pour les algorithmes Minimax et Alpha-Bêta
#define DEFAULT_MINIMAX_DEPTH 3
#define DEFAULT_ALPHABETA_DEPTH 3

// Fonctions pour évaluer la grille et choisir les meilleurs coups
int evaluate_grid(const GlobalGrid *grid, Player player);

Position get_best_move_minimax(const GlobalGrid *grid, int depth, Player player);
Position get_best_move_alphabeta(const GlobalGrid *grid, int depth, Player player);

#endif /* AI_H */
