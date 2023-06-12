#include <iostream>
#include <cstdlib>

#include "../src/state/state.hpp"
#include "../src/config.hpp"

#include<stdio.h>

int search(int depth, State* state) {
    static FILE* fp = fopen("output.txt", "w+");
    if (depth == 5) {
        fprintf(fp, "%s\n", state->encode_state().c_str());
        return 1;
    } else {
        int answer = 0;
        state->get_legal_actions();
        for (auto move : state->legal_actions) {
            auto temp = state->next_state(move);
            answer += search(depth + 1, temp);
        }
        return answer;
    }
}


int main(){
  srand(RANDOM_SEED);
  
  State* state = new State;

  std::cout << search(0, state) << std::endl;

  /*std::cout << state.encode_output() << std::endl;
  state.get_legal_actions();
  for(auto move: state.legal_actions){
    std::cout << move.first.first << " " << move.first.second << " " << move.second.first << " " << move.second.second << "\n";
    auto temp = state.next_state(move);
    if(temp != NULL){
      std::cout << temp->encode_output();
    }else{
      std::cout << "NULL\n";
    }
  }*/
  return 0;
}