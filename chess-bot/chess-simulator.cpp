#include "chess-simulator.h"
// disservin's lib. drop a star on his hard work!
// https://github.com/Disservin/chess-library
#include "chess.hpp"
#include <random>
using namespace ChessSimulator;

std::string ChessSimulator::Move(std::string fen) {
    const int KING_SCORE = 200;
    const int QUEEN_SCORE = 9;
    const int ROOK_SCORE = 5;
    const int KNIGHT_SCORE = 3;
    const int BISHOP_SCORE = 3;
    const int PAWN_SCORE = 1;

    // create your board based on the board string following the FEN notation
    // search for the best move using minimax / monte carlo tree search /
    // alpha-beta pruning / ... try to use nice heuristics to speed up the search
    // and have better results return the best move in UCI notation you will gain
    // extra points if you create your own board/move representation instead of
    // using the one provided by the library

    // here goes a random movement
    chess::Board board(fen);
    chess::Movelist moves;
    chess::movegen::legalmoves(moves, board);
    if(moves.size() == 0)
    return "";


    // get random move
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(0, moves.size() - 1);
    auto move = moves[dist(gen)];

    //Check moves for capture
    for (int i = 0; i < moves.size() - 1; i++)
    {
        //Adds score based on who you will capture
        if (board.isCapture(moves[i]))
        {
            chess::Piece type = board.at(moves[i].to());
            if (type == chess::PieceType::KING)
            {
                moves[i].setScore(moves[i].score() + KING_SCORE);
            }
            else if (type == chess::PieceType::QUEEN)
            {
                moves[i].setScore(moves[i].score() + QUEEN_SCORE);
            }
            else if (type == chess::PieceType::ROOK)
            {
                moves[i].setScore(moves[i].score() + ROOK_SCORE);
            }
            else if (type == chess::PieceType::KNIGHT)
            {
                moves[i].setScore(moves[i].score() + KNIGHT_SCORE);
            }
            else if (type == chess::PieceType::ROOK)
            {
                moves[i].setScore(moves[i].score() + ROOK_SCORE);
            }
            else
            {
                moves[i].setScore(moves[i].score() + PAWN_SCORE);
            }
        }
    }
    //Gets highest value move
    for (int i = 0; i < moves.size() - 1; i++)
    {
        if (moves[i].score() > move.score())
        {
            move = moves[i];
        }
    }

    //Returns chosen move
    return chess::uci::moveToUci(move);
}