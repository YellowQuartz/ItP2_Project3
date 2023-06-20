#include <cstdlib>
#include <algorithm>

#include "../state/state.hpp"
#include "./Exorcist.hpp"
#include<stdio.h>

#define INF 1E10
#define maxDepth 3

Move Exorcist::get_move(State* state) {
    return search(state, -INF, INF, maxDepth, true, state->player).bestMove;
}

Move Exorcist::get_move(State* state, float* weights) {
    return search(state, -INF, INF, maxDepth, true, state->player, weights).bestMove;
}

Exorcist::Result Exorcist::search(State* state, float alpha, float beta, int depth, bool forPlayer, Player side) {
    //static FILE* fp = fopen("output.txt", "w+");
    //fprintf(fp, "%f %f %d\n", alpha, beta, depth);
    //fprintf(fp, "%s", state->encode_state().c_str());

    if (!state->legal_actions.size())
        state->get_legal_actions();

    Move answer;
    float value;

    if (depth == 0 || state->legal_actions.size() == 0) {
        return Result(state->evaluate(side));
    }
    else {
        if (forPlayer) {
            value = -INF;
            if (state->legal_actions.size() == 0) {
                state->get_legal_actions();
            }
            for (Move& move : state->legal_actions) {
                State* next = state->next_state(move);
                Result result = search(next, alpha, beta, depth - 1, false, side);
                
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
            if (state->legal_actions.size() == 0) {
                state->get_legal_actions();
            }
            for (Move& move : state->legal_actions) {
                State* next = state->next_state(move);
                Result result = search(next, alpha, beta, depth - 1, true, side);

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
            //printf("%f\n", value);
            return Result(value);
        }
    }
}

float Eval(float* weights, int* board) {
    //FILE* fp = fopen("weight.txt", "w+");
    float hidden[64], answer = 0;
    int wIndex = 0;
    for (int i = 0; i < 64; i++) hidden[i] = 0;
    for (int pl = 0; pl < 2; pl++) {
        for (int type = 0; type < 6; type++) {
            for (int pos = 1 << 29, idx = 0; pos; pos >>= 1, idx++) {
                for (int target = 0; target < 32; target++) {
                    hidden[32 * pl + target] += ((board[type * 2 + pl] & pos) != 0) * weights[wIndex++];
                    //fprintf(fp, "(%d %f %f) ", (board[type * 2 + pl] & pos != 0), this->weights[wIndex++], (board[type * 2 + pl] & pos != 0) * this->weights[wIndex++]);
                }
            }
            //fprintf(fp, "\n");
        }
    }
    //printf("\n");
    for (int i = 0; i < 64; i++) {
        //printf("%f\n", hidden[i]);
        answer += hidden[i] * weights[wIndex++];
    }
    return answer;
}

Exorcist::Result Exorcist::search(State* state, float alpha, float beta, int depth, bool forPlayer, Player side, float* weights) {
    //static FILE* fp = fopen("output.txt", "w+");
    //fprintf(fp, "%f %f %d\n", alpha, beta, depth);
    //fprintf(fp, "%s", state->encode_state().c_str());
    
    if (!state->legal_actions.size())
        state->get_legal_actions();

    Move answer;
    float value;

    if (depth == 0 || state->legal_actions.size() == 0) {
        //printf("%f\n", Eval(weights, state->board));
        return Result(Eval(weights, state->board));
    }
    else {
        if (forPlayer) {
            value = -INF;
            if (state->legal_actions.size() == 0) {
                state->get_legal_actions();
            }
            for (Move& move : state->legal_actions) {
                State* next = state->next_state(move);
                Result result = search(next, alpha, beta, depth - 1, false, side, weights);

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
            if (state->legal_actions.size() == 0) {
                state->get_legal_actions();
            }
            for (Move& move : state->legal_actions) {
                State* next = state->next_state(move);
                Result result = search(next, alpha, beta, depth - 1, true, side, weights);

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
            //printf("%f\n", value);
            return Result(value);
        }
    }
}