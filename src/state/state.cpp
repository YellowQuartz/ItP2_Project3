#include <iostream>
#include <sstream>
#include <cstdint>

#include "./state.hpp"
#include "../config.hpp"


static const int material_table[7] = { 0, 2, 6, 7, 8, 20, 100 };
std::map<int, int> twos_table = { {1, 0},  {2, 1},  {4, 2},  {8, 3},  {16, 4},  {32, 5},  {64, 6},  {128, 7},  {256, 8},  {512, 9},  {1024, 10},  {2048, 11},  {4096, 12},  {8192, 13},  {16384, 14},  {32768, 15},  {65536, 16},  {131072, 17},  {262144, 18},  {524288, 19},  {1048576, 20},  {2097152, 21},  {4194304, 22},  {8388608, 23},  {16777216, 24},  {33554432, 25},  {67108864, 26},  {134217728, 27},  {268435456, 28},  {536870912, 29} };

float State::evaluate(Player side){
    float answer = 0;
    for (int i = 0; i < 12; i += 2) {
        answer += material_table[i / 2 + 1] * countOnes(this->board[i]);
        answer -= material_table[i / 2 + 1] * countOnes(this->board[i + 1]);
    }

    answer *= side;
    return answer;
}


/**
 * @brief return next state after the move
 * 
 * @param move 
 * @return State* 
 */
State* State::next_state(Move move){
    int newBoard[12];
    for (int i = 0; i < 12; i++) {
        newBoard[i] = this->board[i];
    }
    int fromKey = 1 << pointToIndex(move.first);
    int toKey   = 1 << pointToIndex(move.second);
    int mask = universe - fromKey - toKey;
    
    for (int i = 0; i < 12; i++) {
        if (newBoard[i] & fromKey) {
            newBoard[i] &= mask;
            newBoard[i] |= toKey;
        }
        else {
            newBoard[i] &= mask;
        }
    }
    //promotion for pawns
    int queenMask = universe - queenZone;
    int whitePromoted = newBoard[0] & queenZone;
    newBoard[0] &= queenMask;
    newBoard[8] |= whitePromoted;
    int blackPromoted = newBoard[1] & queenZone;
    newBoard[1] &= queenMask;
    newBoard[9] |= blackPromoted;
  

    State* next_state = new State(newBoard, this->opponent());

    if(this->game_state != WIN)
        next_state->get_legal_actions();
    return next_state;
}

/**
 * @brief get all legal actions of now state
 * 
 */
void State::get_legal_actions(){
    if (this->legal_actions.size()) {
        return;
    }
  // [Optional]
  // This method is not very efficient
  // You can redesign it
  std::vector<Move> all_actions;
  int key = 1 << 29;
  int pawnBoard = this->getBoardbyPiece(pawn);
  int knightBoard = this->getBoardbyPiece(knight);
  int kingBoard = this->getBoardbyPiece(king);
  int bishopBoard = this->getBoardbyPiece(bishop);
  int rookBoard = this->getBoardbyPiece(rook);
  int queenBoard = this->getBoardbyPiece(queen);

  int playerBoard = this->getPlayerBoard();
  int opponentBoard = this->getOpponentBoard();


  for (int i = 0; i < 30; i++) {
      Point cur = indexToPoint(29 - i);
      int moves = pawnAttack(key & opponentBoard, this->opponent()) & pawnBoard;
      for (Point pt : bitScan(moves)) {
          //std::cout << 1 << " " << pt.first << " " << pt.second << " " << cur.first << " " << cur.second << "\n";
          all_actions.push_back(Move(pt, cur));
      }
      moves = pawnMove(key & (universe - opponentBoard - playerBoard), this->opponent()) & pawnBoard;
      for (Point pt : bitScan(moves)) {
          //std::cout << 2 << " " << pt.first << " " << pt.second << " " << cur.first << " " << cur.second << "\n";
          all_actions.push_back(Move(pt, cur));
      }
      moves = knightAttack(key & (universe - playerBoard)) & knightBoard;
      for (Point pt : bitScan(moves)) {
          //std::cout << 3 << " " << pt.first << " " << pt.second << " " << cur.first << " " << cur.second << "\n";
          all_actions.push_back(Move(pt, cur));
      }
      moves = kingAttack(key & (universe - playerBoard)) & kingBoard;
      for (Point pt : bitScan(moves)) {
          //std::cout << 4 << " " << pt.first << " " << pt.second << " " << cur.first << " " << cur.second << "\n";
          //std::cout << "Here " << this->player << " " << key << " " << playerBoard << " " << (key & (universe - playerBoard)) << " " << moves << std::endl;
          all_actions.push_back(Move(pt, cur));
      }
      
      if (bishopBoard & key) {
          moves = bishopAttack(29 - i, playerBoard, opponentBoard);
          for (Point pt : bitScan(moves)) {
              //std::cout << 5 << " " << pt.first << " " << pt.second << " " << cur.first << " " << cur.second << "\n";
              all_actions.push_back(Move(cur, pt));
          }
      }
      if (rookBoard & key) {
          moves = rookAttack(29 - i, playerBoard, opponentBoard);
          for (Point pt : bitScan(moves)) {
              //std::cout << 7 << " " << pt.first << " " << pt.second << " " << cur.first << " " << cur.second << "\n";
              all_actions.push_back(Move(cur, pt));
          }
      }

      if (queenBoard & key) {
          moves = bishopAttack(29 - i, playerBoard, opponentBoard);
          moves |= rookAttack(29 - i, playerBoard, opponentBoard);
          //printf("Queen : %d %d %d %d %d\n", queenBoard, antiKey, i, bishopAttack(i, playerBoard, opponentBoard), rookAttack(i, playerBoard, opponentBoard));
          for (Point pt : bitScan(moves)) {
              //std::cout << 8 << " " << pt.first << " " << pt.second << " " << cur.first << " " << cur.second << "\n";
              all_actions.push_back(Move(cur, pt));
          }
      }
      
      key >>= 1;
  }
  //std::cout << queenBoard << std::endl;
  this->legal_actions = all_actions;
}

State::State() {
    for (int i = 0; i < 12; i++) {
        this->board[i] = initial_board[i];
    }
    this->player = WHITE;
    this->playerBoard = -1;
    this->opponentBoard = -1;
    checkWin();
}

State::State(Player player) {
    for (int i = 0; i < 12; i++) {
        this->board[i] = initial_board[i];
    }
    this->player = player;
    this->playerBoard = -1;
    this->opponentBoard = -1;
    checkWin();
}

State::State(int* board) {
    for (int i = 0; i < 12; i++) {
        this->board[i] = board[i];
    }
    this->player = WHITE;
    this->playerBoard = -1;
    this->opponentBoard = -1;
    checkWin();
}

State::State(int* board, Player player) {
    for (int i = 0; i < 12; i++) {
        this->board[i] = board[i];
    }
    this->player = player;
    this->playerBoard = -1;
    this->opponentBoard = -1;
    checkWin();
}

int State::getPlayerBoard() {
    if (this->playerBoard == -1) {
        this->playerBoard = 0;
        int start = this->player == WHITE ? 0 : 1;

        for (int i = start; i < 12; i += 2) {
            this->playerBoard |= this->board[i];
        }
    }
    return this->playerBoard;
}

Player State::opponent() {
    return (this->player == WHITE) ? BLACK : WHITE;
}

int State::getOpponentBoard() {
    if (this->opponentBoard == -1) {
        this->opponentBoard = 0;
        int start = this->player == WHITE ? 1 : 0;

        for (int i = start; i < 12; i += 2) {
            this->opponentBoard |= this->board[i];
        }
    }
    return this->opponentBoard;
}

int State::transform(int board, int magic, int offset) {
    long long mult = ((long long)board) * magic;
    return (mult % cap) >> (30 - offset);
}

void State::checkWin() {
    if (this->board[10] == 0) {
        //this->player = BLACK;
        this->game_state = WIN;
    }
    else if (this->board[11] == 0) {
        //this->player = WHITE;
        this->game_state = WIN;
    }
}

char* State::encode_state() {
    int key = 1 << 29, index = 0;
    char* answer = (char*) malloc(sizeof(char) * 100);
    answer[index++] = (this->player == WHITE ? '0' : '1');
    answer[index++] = '\n';
    for (int i = 0; i < BOARD_H; i++) {
        for (int j = 0; j < BOARD_W; j++) {
            answer[index] = '0';
            for (int k = 0; k < 12; k++) {
                if (key & this->board[k]) {
                    answer[index] = '1' + (k / 2);
                    break;
                }
            }
            index++;
            answer[index++] = ' ';
            key >>= 1;
        }
        answer[index++] = '\n';
    }
    answer[index] = '\0';
    return answer;
}

Point indexToPoint(int index) {
    return Point(5 - (index / 5), 4 - (index % 5));
}

int pointToIndex(Point point) {
    return 29 - point.first * 5 - point.second;
}

int State::getBoardbyPiece(Piece pieceType) {
    int index = (pieceType / 3 * 2) + (this->player == WHITE ? 0 : 1);
    return this->board[index];
}

int State::getOpponentBoardbyPiece(Piece pieceType) {
    int index = (pieceType / 3 * 2) + (this->player == WHITE ? 1 : 0);
    return this->board[index];
}

int pawnAttack(int board, Player player) {
    int pawnEastAttack = player == WHITE ? board << 6 : board >> 4;
    pawnEastAttack &= universe - columns[4];
    int pawnWestAttack = player == WHITE ? board << 4 : board >> 6;
    pawnWestAttack &= universe - columns[0];

    return pawnEastAttack | pawnWestAttack;
}

int pawnMove(int board, Player player) {
    board = player == WHITE ? board << 5 : board >> 5;
    board %= cap;
    return board;
}

int eastOne(int board) {
    return (board >> 1) & (universe - columns[0]);
}

int westOne(int board) {
    return ((board << 1) % cap) & (universe - columns[4]);
}

int northOne(int board) {
    return ((board << 5) % cap);
}

int southOne(int board) {
    return (board >> 5);
}

int knightAttack(int board) {
    int east = eastOne(board);
    int west = westOne(board);
    int attacks = (east | west) << 10;
    attacks |= (east | west) >> 10;
    east = eastOne(east);
    west = westOne(west);
    attacks |= (east | west) << 5;
    attacks |= (east | west) >> 5;

    return attacks;
}

int kingAttack(int board) {
    int attacks = eastOne(board) | westOne(board);
    board |= attacks;
    attacks |= northOne(board) | southOne(board);

    return attacks;
}

int bishopAttack(int square, int selfBoard, int oppoBoard) {
    int blockerMask = BishopBlockerMask[square];
    int board = selfBoard | oppoBoard;
    board &= blockerMask;
    int pseudoLegal = BishopLookupTable[square][State::transform(board, BishopMagic[square], BishopBits[square])];
    return pseudoLegal & (universe - selfBoard);
}

int rookAttack(int square, int selfBoard, int oppoBoard) {
    int blockerMask = RookBlockerMask[square];
    int board = selfBoard | oppoBoard;
    board &= blockerMask;
    int pseudoLegal = RookLookupTable[square][State::transform(board, RookMagic[square], RookBits[square])];
    return pseudoLegal & (universe - selfBoard);
}

int isPowerofTwo(int num) {
    return num != 0 && ((num & (num - 1)) == 0);
}

std::vector<Point> binaryBitScan(int segment, int size, int offset) {
    std::vector<Point> answer;
    if (segment == 0) {
        return answer;
    }

    if (size == 1 || isPowerofTwo(segment)) {
        int index = twos_table[segment]+ offset;
        answer.push_back(indexToPoint(index));
        return answer;
    }

    int half = size / 2;
    int dilimeter = 1 << half;

    std::vector<Point> firstHalf = binaryBitScan(segment % dilimeter, half, offset);
    answer.insert(answer.end(), firstHalf.begin(), firstHalf.end());
    std::vector<Point> secondHalf = binaryBitScan(segment >> size, size - half, offset + half);
    answer.insert(answer.end(), secondHalf.begin(), secondHalf.end());

    return answer;
}

std::vector<Point> bitScan(int board) {
    return binaryBitScan(board, 30, 0);
}

int countOnes(int board) {
    int answer = 0;
    while (board != 0) {
        answer++;
        board &= board - 1;
    }
    return answer;
}