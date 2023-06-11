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
private:
	static Result search(State* state, float alpha, float beta, int depth, bool forPlayer);
};