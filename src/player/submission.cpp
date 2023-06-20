#include <iostream>
#include <fstream>

#include "../config.hpp"
#include "../state/state.hpp"
#include "../policy/Exorcist.hpp"


State* root;

/**
 * @brief Read the board from the file
 *
 * @param fin
 */
void read_board(std::ifstream& fin) {
    int board[12] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    int p;
    fin >> p;
    Player player = p == 0 ? (Player) WHITE : (Player) BLACK;

    for (int pl = 0; pl < 2; pl++) {
        int key = 1 << 30;
        for (int i = 0; i < BOARD_H; i++) {
            for (int j = 0; j < BOARD_W; j++) {
                int c; fin >> c;

                key >>= 1;
                if (c == 0) continue;
                // std::cout << c << " ";
                board[2 * (c - 1) + pl] |= key;
            }
            // std::cout << std::endl;
        }
    }
    root = new State(board, player);
    root->get_legal_actions();
}


/**
 * @brief randomly choose a move and then write it into output file
 *
 * @param fout
 */
void write_valid_spot(std::ofstream& fout) {
    // Keep updating the output until getting killed.
    while (true) {
        // Choose a random spot.
        auto move = Exorcist::get_move(root);
        fout << move.first.first << " " << move.first.second << " "\
            << move.second.first << " " << move.second.second << std::endl;

        // Remember to flush the output to ensure the last action is written to file.
        fout.flush();
        break;
    }
}


/**
 * @brief Main function for player
 *
 * @param argv
 * @return int
 */
int main(int, char** argv) {
    srand(RANDOM_SEED);
    std::ifstream fin(argv[1]);
    std::ofstream fout(argv[2]);

    read_board(fin);
    write_valid_spot(fout);

    fin.close();
    fout.close();
    return 0;
}
