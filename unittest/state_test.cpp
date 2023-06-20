#include <iostream>
#include <cstdlib>
#include <sstream>

#include "../src/state/state.hpp"
#include "../src/config.hpp"
#include "../src/policy/Exorcist.hpp"

#include<stdio.h>
#include<set>

const int weightsCount = 30 * 6 * 32 * 2 + 64;
class Species {
public:
    Species();
    Species(Species, Species);

    std::string toString();
    float weights[weightsCount];
};

Species::Species() {
    float max = -1, min = 1;
    for (int i = 0; i < weightsCount; i++) {
        this->weights[i] = ((float)rand() / (RAND_MAX)) - 0.5f;
        this->weights[i] *= 2;
        max = std::max(max, this->weights[i]);
        min = std::min(min, this->weights[i]);
    }
}

std::string Species::toString() {
    std::stringstream ss;
    int wIndex = 0;
    for (int pl = 0; pl < 2; pl++) {
        for (int type = 0; type < 6; type++) {
            for (int pos = 0; pos < 30; pos++) {
                for (int target = 0; target < 32; target++) {
                    ss << this->weights[wIndex++] << " ";
                }
                ss << "\n";
            }
            ss << "\n";
        }
    }
    for (int i = 0; i < 64; i++) ss << this->weights[wIndex++] << " ";
    ss << "\n";
    return ss.str();
}


int main(int argc, char** argv){
  srand(time(0));
  
  /*
  State* state = new State;
  possible.clear();
  maxDepth = atoi(argv[1]);
  //state->player = 1;

  search(0, state);
  std::cout << maxDepth << " " << possible.size() << std::endl;

  FILE* fp = fopen("output.txt", "w+");
  for (auto state_ : possible) {
      fprintf(fp, "%s", state_.c_str());
  }
  */

  /*for (int i = 0; i < 6; i++) {
      for (int j = 0; j < 5; j++) {
        printf("(%d, %d), %d\n", i, j, pointToIndex(Point(i, j)));
      }
  }*/

  /*int board[12] = { 31457281 + 31,0,0,64 * 32,0,0,0,0,0,0,1,16};

  State state = State(board, WHITE);

  std::cout << state.encode_state();
  state.get_legal_actions();
  for (auto move : state.legal_actions) {
      State* temp = state.next_state(move);
      std::cout << temp->encode_state() << temp->evaluate(state.player) << std::endl;
  }

  Move best = Exorcist::get_move(&state);
  printf("%d %d %d %d\n", best.first.first, best.first.second, best.second.first, best.second.second);*/

  Species test;
  State state;
  Move bestMove = Exorcist::get_move(&state, test.weights);

  //printf("%f\n", Species::Eval(state.board, ));
  //printf("%s\n", test.toString().c_str());
  printf("(%lu, %lu) (%lu, %lu)\n", bestMove.first.first, bestMove.first.second, bestMove.second.first, bestMove.second.second);

  return 0;
}