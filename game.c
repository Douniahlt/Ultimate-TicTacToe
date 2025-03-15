#include "game.h"
#include <stdio.h>
#include <stdlib.h>

// Couleurs ANSI
#define COLOR_RESET "\033[0m"
#define COLOR_X "\033[1;31m" // Rouge
#define COLOR_O "\033[1;34m" // Bleu
#define COLOR_GRID "\033[1;37m" // Blanc brillant

// Initialise une grille locale (vide)
static void init_local_grid(LocalGrid *subgrid) {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            subgrid->cells[i][j] = EMPTY;
        }
    }
    subgrid->winner = NO_PLAYER;
    subgrid->full = false;
}

// Initialise une grille globale
GlobalGrid init_global_grid() {
    GlobalGrid grid = {0}; // Initialise toutes les cases à 0
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            init_local_grid(&grid.sub_grids[i][j]);
        }
    }
    grid.global_winner = NO_PLAYER;
    grid.next_subgrid = -1;
    return grid;
}

// Affiche la grille globale
void display_global_grid(const GlobalGrid *grid) {
    printf("\n%sGrille globale:%s\n\n", COLOR_GRID, COLOR_RESET);

    for (int row = 0; row < 3; row++) {
        for (int sub_row = 0; sub_row < 3; sub_row++) {
            for (int col = 0; col < 3; col++) {
                for (int sub_col = 0; sub_col < 3; sub_col++) {
                    Player cell = grid->sub_grids[row][col].cells[sub_row][sub_col];
                    char symbol = (cell == PLAYER_X) ? 'X' : (cell == PLAYER_O) ? 'O' : '.';
                    const char *color = (cell == PLAYER_X) ? COLOR_X : (cell == PLAYER_O) ? COLOR_O : COLOR_GRID;

                    printf("%s%c%s ", color, symbol, COLOR_RESET);
                }
                if (col < 2) {
                    printf("%s|%s ", COLOR_GRID, COLOR_RESET); // Séparateur entre sous-colonnes
                }
            }
            printf("\n");
        }
        if (row < 2) {
            printf("%s-------------------%s\n", COLOR_GRID, COLOR_RESET); // Séparateur entre sous-plateaux
        }
    }
    printf("\n");
}

// Vérifie si une grille locale est pleine
bool is_local_grid_full(const LocalGrid *subgrid) {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (subgrid->cells[i][j] == EMPTY) {
                return false;
            }
        }
    }
    return true;
}

// Vérifie si une grille locale est gagnée
bool check_local_winner(LocalGrid *subgrid) {
    Player winner = NO_PLAYER;

    // Lignes
    for (int i = 0; i < 3; i++) {
        if (subgrid->cells[i][0] == subgrid->cells[i][1] &&
            subgrid->cells[i][1] == subgrid->cells[i][2] &&
            subgrid->cells[i][0] != EMPTY) {
            winner = subgrid->cells[i][0];
        }
    }

    // Colonnes
    for (int i = 0; i < 3; i++) {
        if (subgrid->cells[0][i] == subgrid->cells[1][i] &&
            subgrid->cells[1][i] == subgrid->cells[2][i] &&
            subgrid->cells[0][i] != EMPTY) {
            winner = subgrid->cells[0][i];
        }
    }

    // Diagonales
    if (subgrid->cells[0][0] == subgrid->cells[1][1] &&
        subgrid->cells[1][1] == subgrid->cells[2][2] &&
        subgrid->cells[0][0] != EMPTY) {
        winner = subgrid->cells[0][0];
    }

    if (subgrid->cells[0][2] == subgrid->cells[1][1] &&
        subgrid->cells[1][1] == subgrid->cells[2][0] &&
        subgrid->cells[0][2] != EMPTY) {
        winner = subgrid->cells[0][2];
    }

    subgrid->winner = winner;
    subgrid->full = is_local_grid_full(subgrid);
    return winner != NO_PLAYER;
}

// Vérifie si la grille globale est pleine
bool is_global_grid_full(const GlobalGrid *grid) {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (!grid->sub_grids[i][j].full) {
                return false;
            }
        }
    }
    return true;
}

// Vérifie le gagnant global de la grille
Player get_global_winner(const GlobalGrid *grid) {
    // Lignes
    for (int i = 0; i < 3; i++) {
        if (grid->sub_grids[i][0].winner != NO_PLAYER &&
            grid->sub_grids[i][0].winner == grid->sub_grids[i][1].winner &&
            grid->sub_grids[i][1].winner == grid->sub_grids[i][2].winner) {
            return grid->sub_grids[i][0].winner;
        }
    }

    // Colonnes
    for (int i = 0; i < 3; i++) {
        if (grid->sub_grids[0][i].winner != NO_PLAYER &&
            grid->sub_grids[0][i].winner == grid->sub_grids[1][i].winner &&
            grid->sub_grids[1][i].winner == grid->sub_grids[2][i].winner) {
            return grid->sub_grids[0][i].winner;
        }
    }

    // Diagonales
    if (grid->sub_grids[0][0].winner != NO_PLAYER &&
        grid->sub_grids[0][0].winner == grid->sub_grids[1][1].winner &&
        grid->sub_grids[1][1].winner == grid->sub_grids[2][2].winner) {
        return grid->sub_grids[0][0].winner;
    }

    if (grid->sub_grids[0][2].winner != NO_PLAYER &&
        grid->sub_grids[0][2].winner == grid->sub_grids[1][1].winner &&
        grid->sub_grids[1][1].winner == grid->sub_grids[2][0].winner) {
        return grid->sub_grids[0][2].winner;
    }

    return NO_PLAYER;
}

// Effectue un mouvement
void make_move(GlobalGrid *grid, Position pos, Player player, int subgrid_constraint) {
    (void)subgrid_constraint; // Supprime l'avertissement sur le paramètre inutilisé

    int subgrid_row = pos.x / 3;
    int subgrid_col = pos.y / 3;

    int local_row = pos.x % 3;
    int local_col = pos.y % 3;

    LocalGrid *subgrid = &grid->sub_grids[subgrid_row][subgrid_col];
    subgrid->cells[local_row][local_col] = player;

    check_local_winner(subgrid);

    if (subgrid->winner != NO_PLAYER || is_local_grid_full(subgrid)) {
        grid->next_subgrid = -1; // Le joueur suivant peut jouer n'importe où
    } else {
        grid->next_subgrid = local_row * 3 + local_col;
    }

    grid->global_winner = get_global_winner(grid);
}
