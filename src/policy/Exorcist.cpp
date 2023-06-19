#include <cstdlib>
#include <algorithm>

#include "../state/state.hpp"
#include "./Exorcist.hpp"
#include<stdio.h>

#define INF 1E10
#define maxDepth 4

Move Exorcist::get_move(State* state) {

    return search(state, -INF, INF, maxDepth, true).bestMove;
}

Exorcist::Result Exorcist::search(State* state, float alpha, float beta, int depth, bool forPlayer) {
    //static FILE* fp = fopen("output.txt", "w+");
    //fprintf(fp, "%f %f %d\n", alpha, beta, depth);
    //fprintf(fp, "%s", state->encode_state().c_str());

    if (!state->legal_actions.size())
        state->get_legal_actions();

    Move answer;
    float value;

    if (depth == 0 || state->legal_actions.size() == 0) {
        return Result(state->evaluate());
    }
    else {
        if (forPlayer) {
            value = -INF;
            for (Move& move : state->legal_actions) {
                State* next = state->next_state(move);
                Result result = search(next, alpha, beta, depth - 1, false);
                
                if (result.value > value) {
                    answer = move;
                    value = result.value;
                }
                alpha = std::max(alpha, value);

                /*if (depth == maxDepth) {
                    fprintf(fp, "%d %lu %lu / %lu %lu : %f\n", depth, move.first.first, move.first.second, move.second.first, move.second.second, value);
                    fprintf(fp, "%s", state->encode_state().c_str());
                }*/

                if (alpha >= beta) {
                    break;
                }
            }
        }
        else {
            value = INF;
            for (Move& move : state->legal_actions) {
                State* next = state->next_state(move);
                Result result = search(next, alpha, beta, depth - 1, true);

                if (result.value < value) {
                    answer = move;
                    value = result.value;
                }
                beta = std::min(beta, value);

                /*if (depth == maxDepth) {
                    fprintf(fp, "%d %lu %lu / %lu %lu : %f\n", depth, move.first.first, move.first.second, move.second.first, move.second.second, value);
                    fprintf(fp, "%s", state->encode_state().c_str());
                }*/

                if (beta <= alpha) {
                    break;
                }
            }
        }

        if (depth == maxDepth) {
            return Result(answer);
        }
        else {
            return Result(value);
        }
    }
}