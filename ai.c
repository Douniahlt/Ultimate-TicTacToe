#include "ai.h"
#include <limits.h>
#include <stdio.h>
#include <stdbool.h>
#include <time.h>

// Évalue la grille globale pour un joueur donné
int evaluate_grid(const GlobalGrid *grid, Player player) {
    int score = 0;

    // Vérification des sous-plateaux
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            const LocalGrid *subgrid = &grid->sub_grids[i][j];

            if (subgrid->winner == player) {
                score += 10; // Bonus pour chaque sous-plateau gagné
            } else if (subgrid->winner == -player) {
                score -= 10; // Malus pour chaque sous-plateau perdu
            }
        }
    }

    // Vérification du gagnant global
    if (grid->global_winner == player) {
        score += 100; // Victoire globale
    } else if (grid->global_winner == -player) {
        score -= 100; // Défaite globale
    }

    return score;
}

// Implémentation de l'algorithme Minimax
int minimax(GlobalGrid *grid, int depth, bool is_maximizing, Player player) {
    if (depth == 0 || grid->global_winner != NO_PLAYER || is_global_grid_full(grid)) {
        return evaluate_grid(grid, player);
    }

    int best_score = is_maximizing ? INT_MIN : INT_MAX;

    for (int subgrid_row = 0; subgrid_row < 3; subgrid_row++) {
        for (int subgrid_col = 0; subgrid_col < 3; subgrid_col++) {
            int subgrid_index = subgrid_row * 3 + subgrid_col;
            if (grid->next_subgrid != -1 && grid->next_subgrid != subgrid_index) {
                continue;
            }
            for (int local_row = 0; local_row < 3; local_row++) {
                for (int local_col = 0; local_col < 3; local_col++) {
                    Position pos = {subgrid_row * 3 + local_row, subgrid_col * 3 + local_col};

                    if (grid->sub_grids[subgrid_row][subgrid_col].cells[local_row][local_col] == EMPTY) {
                        make_move(grid, pos, is_maximizing ? player : -player, grid->next_subgrid);
                        int score = minimax(grid, depth - 1, !is_maximizing, player);
                        grid->sub_grids[subgrid_row][subgrid_col].cells[local_row][local_col] = EMPTY;

                        best_score = is_maximizing ? (score > best_score ? score : best_score)
                                                   : (score < best_score ? score : best_score);
                    }
                }
            }
        }
    }

    return best_score;
}

// Implémentation de l'algorithme Alpha-Bêta
int alphabeta(GlobalGrid *grid, int depth, bool is_maximizing, int alpha, int beta, Player player) {
    if (depth == 0 || grid->global_winner != NO_PLAYER || is_global_grid_full(grid)) {
        return evaluate_grid(grid, player);
    }

    if (is_maximizing) {
        int max_eval = INT_MIN;

        for (int subgrid_row = 0; subgrid_row < 3; subgrid_row++) {
            for (int subgrid_col = 0; subgrid_col < 3; subgrid_col++) {
                int subgrid_index = subgrid_row * 3 + subgrid_col;
            if (grid->next_subgrid != -1 && grid->next_subgrid != subgrid_index) {
                continue;
            }
                for (int local_row = 0; local_row < 3; local_row++) {
                    for (int local_col = 0; local_col < 3; local_col++) {
                        Position pos = {subgrid_row * 3 + local_row, subgrid_col * 3 + local_col};

                        if (grid->sub_grids[subgrid_row][subgrid_col].cells[local_row][local_col] == EMPTY) {
                            make_move(grid, pos, player, grid->next_subgrid);
                            int eval = alphabeta(grid, depth - 1, false, alpha, beta, player);
                            grid->sub_grids[subgrid_row][subgrid_col].cells[local_row][local_col] = EMPTY;

                            max_eval = eval > max_eval ? eval : max_eval;
                            alpha = eval > alpha ? eval : alpha;

                            if (beta <= alpha) {
                                return max_eval; // Coupure
                            }
                        }
                    }
                }
            }
        }

        return max_eval;
    } else {
        int min_eval = INT_MAX;

        for (int subgrid_row = 0; subgrid_row < 3; subgrid_row++) {
            for (int subgrid_col = 0; subgrid_col < 3; subgrid_col++) {
                int subgrid_index = subgrid_row * 3 + subgrid_col;
            if (grid->next_subgrid != -1 && grid->next_subgrid != subgrid_index) {
                continue;
            }
                for (int local_row = 0; local_row < 3; local_row++) {
                    for (int local_col = 0; local_col < 3; local_col++) {
                        Position pos = {subgrid_row * 3 + local_row, subgrid_col * 3 + local_col};

                        if (grid->sub_grids[subgrid_row][subgrid_col].cells[local_row][local_col] == EMPTY) {
                            make_move(grid, pos, -player, grid->next_subgrid);
                            int eval = alphabeta(grid, depth - 1, true, alpha, beta, player);
                            grid->sub_grids[subgrid_row][subgrid_col].cells[local_row][local_col] = EMPTY;

                            min_eval = eval < min_eval ? eval : min_eval;
                            beta = eval < beta ? eval : beta;

                            if (beta <= alpha) {
                                return min_eval; // Coupure
                            }
                        }
                    }
                }
            }
        }

        return min_eval;
    }
}

// Choix du meilleur coup avec Minimax
Position get_best_move_minimax(const GlobalGrid *grid, int depth, Player player) {
    Position best_move = {-1, -1};
    int best_score = INT_MIN;

    for (int subgrid_row = 0; subgrid_row < 3; subgrid_row++) {
        for (int subgrid_col = 0; subgrid_col < 3; subgrid_col++) {
            int subgrid_index = subgrid_row * 3 + subgrid_col;
            if (grid->next_subgrid != -1 && grid->next_subgrid != subgrid_index) {
                continue;
            }
            for (int local_row = 0; local_row < 3; local_row++) {
                for (int local_col = 0; local_col < 3; local_col++) {
                    Position pos = {subgrid_row * 3 + local_row, subgrid_col * 3 + local_col};

                    if (grid->sub_grids[subgrid_row][subgrid_col].cells[local_row][local_col] == EMPTY) {
                        GlobalGrid temp_grid = *grid;
                        make_move(&temp_grid, pos, player, temp_grid.next_subgrid);
                        int score = minimax(&temp_grid, depth - 1, false, player);

                        if (score > best_score) {
                            best_score = score;
                            best_move = pos;
                        }
                    }
                }
            }
        }
    }

    return best_move;
}

// Choix du meilleur coup avec Alpha-Bêta
Position get_best_move_alphabeta(const GlobalGrid *grid, int depth, Player player) {
    Position best_move = {-1, -1};
    int best_score = INT_MIN;
    int alpha = INT_MIN, beta = INT_MAX;

    for (int subgrid_row = 0; subgrid_row < 3; subgrid_row++) {
        for (int subgrid_col = 0; subgrid_col < 3; subgrid_col++) {
            int subgrid_index = subgrid_row * 3 + subgrid_col;
            if (grid->next_subgrid != -1 && grid->next_subgrid != subgrid_index) {
                continue;
            }
            for (int local_row = 0; local_row < 3; local_row++) {
                for (int local_col = 0; local_col < 3; local_col++) {
                    Position pos = {subgrid_row * 3 + local_row, subgrid_col * 3 + local_col};

                    if (grid->sub_grids[subgrid_row][subgrid_col].cells[local_row][local_col] == EMPTY) {
                        GlobalGrid temp_grid = *grid;
                        make_move(&temp_grid, pos, player, temp_grid.next_subgrid);
                        int score = alphabeta(&temp_grid, depth - 1, false, alpha, beta, player);

                        if (score > best_score) {
                            best_score = score;
                            best_move = pos;
                        }
                        alpha = score > alpha ? score : alpha;

                        if (beta <= alpha) {
                            return best_move; // Coupure
                        }
                    }
                }
            }
        }
    }

    return best_move;
}