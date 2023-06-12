#include <iostream>
#include <cstdlib>

#include "../src/state/state.hpp"
#include "../src/config.hpp"

#include<stdio.h>

int search(int depth, State* state) {
    static FILE* fp1 = fopen("output1.txt", "w+");
    static FILE* fp2 = fopen("output2.txt", "w+");
    static FILE* fp3 = fopen("output3.txt", "w+");
    static FILE* fp4 = fopen("output4.txt", "w+");
    static int count = 0;

    if (depth == 5) {
        FILE* fp;
        if (count < 15000) {
            fp = fp1;
        }
        else if (count < 30000) {
            fp = fp2;
        }
        else if (count < 45000) {
            fp = fp3;
        }
        else{
            fp = fp4;
        }
        fprintf(fp, "%s\n", state->encode_state().c_str());
        count++;
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
  //state->player = 1;

  std::cout << search(0, state) << std::endl;

  /*State state;

  std::cout << state.encode_output() << std::endl;
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