#ifndef GAME_H
#define GAME_H

#include <stdbool.h>

// Définition des joueurs
typedef enum {
    EMPTY = 0,
    PLAYER_X = 1,
    PLAYER_O = -1,
    NO_PLAYER = 2
} Player;

// Position sur la grille
typedef struct {
    int x;
    int y;
} Position;

// Grille locale (3x3)
typedef struct {
    Player cells[3][3]; // Cases du sous-plateau
    Player winner;      // Gagnant de cette grille locale
    bool full;          // Indique si la grille locale est pleine
} LocalGrid;

// Grille globale (3x3 de grilles locales)
typedef struct {
    LocalGrid sub_grids[3][3];
    Player global_winner; // Gagnant de la grille globale
    int next_subgrid;     // Contrainte pour le prochain coup
} GlobalGrid;

// Initialisation des grilles
GlobalGrid init_global_grid();
void display_global_grid(const GlobalGrid *grid);

// Validation et statut des grilles
bool is_global_grid_full(const GlobalGrid *grid);
Player get_global_winner(const GlobalGrid *grid);

// Mouvements
void make_move(GlobalGrid *grid, Position pos, Player player, int subgrid_constraint);

#endif /* GAME_H */
