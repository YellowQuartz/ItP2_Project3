#include <iostream>
#include <cstdlib>

#include "../src/state/state.hpp"
#include "../src/config.hpp"

#include<stdio.h>
#include<set>

std::set<std::string> possible;
int maxDepth = 3;

int search(int depth, State* state) {
    /*static FILE* fp1 = fopen("output1.txt", "w+");
    static FILE* fp2 = fopen("output2.txt", "w+");
    static FILE* fp3 = fopen("output3.txt", "w+");
    static FILE* fp4 = fopen("output4.txt", "w+");
    static int count = 0;*/

    if (depth == maxDepth) {
        return 1;
    } else {
        int answer = 0;
        if (!state->legal_actions.size())
            state->get_legal_actions();

        for (auto move : state->legal_actions) {
            auto temp = state->next_state(move);
            answer += search(depth + 1, temp);
        }
        return answer;
    }
}


int main(int argc, char** argv){
  srand(RANDOM_SEED);
  
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

  std::cout << "Yeah\n";

  State state;

  std::cout << state.encode_state() << std::endl;
  state.get_legal_actions();
  for(auto move: state.legal_actions){
    std::cout << move.first.first << " " << move.first.second << " " << move.second.first << " " << move.second.second << "\n";
    auto temp = state.next_state(move);
    if(temp != NULL){
      std::cout << temp->encode_state();
    }else{
      std::cout << "NULL\n";
    }
  }

  return 0;
}