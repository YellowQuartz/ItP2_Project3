#include <iostream>
#include <cstdlib>
#include <sstream>

#include "../src/state/state.hpp"
#include "../src/config.hpp"
#include "../src/policy/Exorcist.hpp"

#include<stdio.h>
#include<deque>
#include <iomanip>
#include<math.h>
#include<string.h>

const int weightsCount = 30 * 6 * 32 * 2 + 64;
class Species {
public:
    Species();
    Species(Species, Species);

    std::string toString();
    float weights[weightsCount];
};

Species::Species() {
    for (int i = 0; i < weightsCount; i++) {
        this->weights[i] = ((float)rand() / (RAND_MAX)) - 0.5f;
        this->weights[i] *= 200;
        this->weights[i] = round(this->weights[i]);
        this->weights[i] /= 100;
    }
}

Species::Species(Species f1, Species f2) {
    int sum = weightsCount;
    std::deque<int> sequence;
    while(sum > 0) {
        int next = rand() % 101;
        next = std::min(sum, next);
        sum -= next;
        sequence.push_back(next);
    }
    bool toggle = false;
    for (int i = 0; i < weightsCount; i++) {
        this->weights[i] = toggle ? f1.weights[i] : f2.weights[i];
        sequence[0]--;
        if (sequence[0] == 0) {
            sequence.pop_front();
            toggle = !toggle;
        }
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

bool game(Species a, Species b) {
    State* state = new State;
    Move move;
    int step = 0;

    while (state->game_state != WIN && step <= 50) {
        step++;
        move = Exorcist::get_move(state, step % 2 == 1 ? a.weights : b.weights);
        state = state->next_state(move);
    }
    if (state->game_state == WIN) {
        return state->player == BLACK;
    }
    return state->evaluate(WHITE) >= 0;
}

std::string getPath(int generation) {
    std::stringstream ss;
    ss << "./Models/Gen";
    ss << std::setfill('0') << std::setw(3) << generation << ".txt";
    return ss.str();
}

int main(int argc, char** argv){
  srand(time(0));
  std::deque<Species> population;

  if(argc && strcmp(argv[1], "getBest") == 0){
      std::deque<Species> list;
      FILE* fp = fopen("./Models/Gen049.txt", "r");
      for (int count = 0; count < 32; count++) {
          Species s;
          for (int i = 0; i < weightsCount; i++) {
              fscanf(fp, "%f", &s.weights[i]);
          }
          list.push_back(s);
      }

      printf("Loaded\n");

      for (int i = 0; i < 5; i++) {
          std::deque<Species> winner;
          printf("List : %d\n", list.size());
          while (list.size() > 1) {
              printf("Hey %d\n", list.size());
              Species Alice = list.front();
              list.pop_front();
              Species Bob = list.front();
              list.pop_front();
              winner.push_back(game(Alice, Bob) ? Alice : Bob);
          }
          printf("Winner : %d\n", winner.size());
          list = std::deque<Species>(winner);
      }

      FILE* best = fopen("./Models/best.txt", "w+");
      printf("%s\n", list[0].toString().c_str());
      //fprintf(best, "%s", list[0].toString().c_str());
      fprintf(best, "{");
      for (int i = 0; i < weightsCount; i++) {
          fprintf(best, "%.2f, ", list[0].weights[i]);
      }
      fprintf(best, "}");
      return 0;
  }

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

  //int board[12] = { 32 + (1 << 19) + (1 << 13),5506048,0,1 << 25,0,0,4,8,2,1 << 27,1,1 << 29};
  /*int board[12] = { 17408,352321536 >> 5,16,1 << 25,0,1<<26,4,4 << 25,2,4096,1,1 << 29 };

  State state = State(board, WHITE);

  std::cout << state.encode_state();*/
  /*state.get_legal_actions();
  for (auto move : state.legal_actions) {
      State* temp = state.next_state(move);
      std::cout << temp->encode_state() << "Eval : " << temp->evaluate(state.player) << std::endl;
  }*/

  /*Move best = Exorcist::get_move(&state);
  printf("player : %d\n", state.player);
  printf("%ld %ld %ld %ld\n", best.first.first, best.first.second, best.second.first, best.second.second);

  return 0;*/

  /*Species test1, test2;
  Species child = Species(test1, test2);

  printf("%s %s %s", test1.toString().c_str(), test2.toString().c_str(), child.toString().c_str());
  return 0;*/

  //init
  if (argc == 3) {
      FILE* fp = fopen(getPath(atoi(argv[2])).c_str(), "r");
      for (int count = 0; count < 32; count++) {
          Species s;
          for (int i = 0; i < weightsCount; i++) {
              fscanf(fp, "%f", &s.weights[i]);
          }
          population.push_back(s);
      }

      printf("Loaded from checkpoint\n");

      for (int i = 0; i < 224; i++) {
          int fIndex1 = std::rand() % 33, fIndex2 = std::rand() % 33;
          population.push_back(Species(population[fIndex1], population[fIndex2]));
      }
  }
  else {
    for (int i = 0; i < 256; i++) {
          population.push_back(Species());
        //printf("%d\n", i);
    }
  }

  int target = atoi(argv[1]);

  for (int generation = 1;generation != target; generation++) {
      FILE* fp = fopen(getPath(generation).c_str(), "w+");

      std::deque<Species> winner;

      //competition
      float gamesDone = 0;
      for (int i = 0; i < 3; i++) {
          while (population.size() > 1) {
               Species Alice = population[0], Bob = population[1];
               population.pop_front();
               population.pop_front();
               winner.push_back(game(Alice, Bob) ? Alice : Bob);
               gamesDone += 1;
               printf("%.2f%% (%d/224) of games in generation %d is done.\n", gamesDone / 2.24, (int) gamesDone, generation);
          }
          population = std::deque<Species>(winner);
          winner.clear();
      }

      //printf("After competition : %ld\n", population.size());
      //recording
      for (int i = 0; i < 32; i++) {
          fprintf(fp, "%s\n", population[i].toString().c_str());
      }

      //reproduce
      for (int i = 0; i < 96; i++) {
          int fIndex1 = std::rand() % 33, fIndex2 = std::rand() % 33;
          population.push_back(Species(population[fIndex1], population[fIndex2]));
      }
      for (int i = 0; i < 128; i++) {
          population.push_back(Species());
      }
      //printf("After reproduction : %ld\n", population.size());

      fclose(fp);
  }

  return 0;
}