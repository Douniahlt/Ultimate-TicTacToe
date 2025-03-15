#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "ai.h"
#include "game.h"

// Simule une partie entre deux IA
void simulate_game(int depth_minimax, int depth_alphabeta) {
    GlobalGrid grid = init_global_grid();
    Player current_player = PLAYER_X;
    int turn = 0;

    printf("Simulation d'une partie entre Minimax et Alpha-Bêta...\n");
    while (!is_global_grid_full(&grid) && get_global_winner(&grid) == NO_PLAYER) {
        display_global_grid(&grid);
        Position best_move;

        if (current_player == PLAYER_X) {
            printf("Minimax (X) joue...\n");
            best_move = get_best_move_minimax(&grid, depth_minimax, PLAYER_X);
        } else {
            printf("Alpha-Bêta (O) joue...\n");
            best_move = get_best_move_alphabeta(&grid, depth_alphabeta, PLAYER_O);
        }

        if (best_move.x == -1 || best_move.y == -1) {
            printf("Aucun coup possible, match nul !\n");
            break;
        }

        printf("Joueur %c joue en (%d, %d)\n", current_player == PLAYER_X ? 'X' : 'O', best_move.x, best_move.y);
        make_move(&grid, best_move, current_player, grid.next_subgrid);
        current_player = -current_player;
        turn++;
    }

    display_global_grid(&grid);
    Player winner = get_global_winner(&grid);

    if (winner == PLAYER_X) {
        printf("Minimax (X) a gagné en %d tours !\n", turn);
    } else if (winner == PLAYER_O) {
        printf("Alpha-Bêta (O) a gagné en %d tours !\n", turn);
    } else {
        printf("Match nul après %d tours !\n", turn);
    }
}

// Compare l'efficacité de Minimax et Alpha-Bêta à différentes profondeurs
void compare_efficiency(int max_depth) {
    GlobalGrid grid = init_global_grid();
    printf("Comparaison de l'efficacité entre Minimax et Alpha-Bêta...\n");

    for (int depth = 1; depth <= max_depth; depth++) {
        clock_t start, end;
        double time_minimax, time_alphabeta;

        // Test de Minimax
        start = clock();
        get_best_move_minimax(&grid, depth, PLAYER_X);
        end = clock();
        time_minimax = (double)(end - start) / CLOCKS_PER_SEC;

        // Test d'Alpha-Bêta
        start = clock();
        get_best_move_alphabeta(&grid, depth, PLAYER_X);
        end = clock();
        time_alphabeta = (double)(end - start) / CLOCKS_PER_SEC;

        printf("Profondeur %d:\n", depth);
        printf("  Minimax: %.6f secondes\n", time_minimax);
        printf("  Alpha-Bêta: %.6f secondes\n", time_alphabeta);
    }
}

int main() {
    srand(time(NULL));
    printf("Bienvenue dans Ultimate Tic-Tac-Toe !\n");

    int choice;
    do {
        printf("\nMenu principal:\n");
        printf("1. Simuler une partie entre Minimax et Alpha-Bêta\n");
        printf("2. Comparer l'efficacité des algorithmes\n");
        printf("3. Quitter\n");
        printf("Choisissez une option: ");
        if (scanf("%d", &choice) != 1) {
            printf("Entrée invalide, veuillez réessayer.\n");
            while (getchar() != '\n'); // Vide le tampon
            continue;
        }

        if (choice == 1) {
            int depth_minimax, depth_alphabeta;
            printf("Profondeur pour Minimax: ");
            if (scanf("%d", &depth_minimax) != 1) {
                printf("Entrée invalide, veuillez réessayer.\n");
                while (getchar() != '\n'); // Vide le tampon
                continue;
            }
            printf("Profondeur pour Alpha-Bêta: ");
            if (scanf("%d", &depth_alphabeta) != 1) {
                printf("Entrée invalide, veuillez réessayer.\n");
                while (getchar() != '\n'); // Vide le tampon
                continue;
            }
            simulate_game(depth_minimax, depth_alphabeta);
        } else if (choice == 2) {
            int max_depth;
            printf("Profondeur maximale à tester: ");
            if (scanf("%d", &max_depth) != 1) {
                printf("Entrée invalide, veuillez réessayer.\n");
                while (getchar() != '\n'); // Vide le tampon
                continue;
            }
            compare_efficiency(max_depth);
        } else if (choice == 3) {
            printf("Au revoir !\n");
        } else {
            printf("Option invalide, veuillez réessayer.\n");
        }
    } while (choice != 3);

    return 0;
}
