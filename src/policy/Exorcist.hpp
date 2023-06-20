#pragma once
#include "../state/state.hpp"


/**
 * @brief Policy class for random policy,
 * your policy class should have get_move method
 */
class Exorcist {
	union Result {
		float value;
		Move bestMove;

		Result( Move bM) : bestMove(bM) {

		}
		Result(float v) : value(v) {

		}
	};
public:
	static Move get_move(State* state);
	static Move get_move(State* state, float* weights);
private:
	static Result search(State* state, float alpha, float beta, int depth, bool forPlayer, Player side);
	static Result search(State* state, float alpha, float beta, int depth, bool forPlayer, Player side, float* weights);
};