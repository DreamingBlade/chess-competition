#include "chess-simulator.h"
// disservin's lib. drop a star on his hard work!
// https://github.com/Disservin/chess-library
#include "chess.hpp"
#include <random>
using namespace ChessSimulator;


const int KING_SCORE = 200;
const int QUEEN_SCORE = 9;
const int ROOK_SCORE = 5;
const int KNIGHT_SCORE = 3;
const int BISHOP_SCORE = 3;
const int PAWN_SCORE = 1;
const float MOVE_SCORE_REDUCTION = 0.5f;

std::string ChessSimulator::Move(std::string fen) {

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

    //Evaluates moves for being good
    for (int i = 0; i < moves.size() - 1; i++)
    {
        //Adds score based on who you will capture
        moves[i].setScore(ScoreToAdd(board, moves[i]) + moves[i].score());

        
        //Gets all the moves this move would allow
        board.makeMove(moves[i]);
        chess::Movelist nextMoves;
        chess::movegen::legalmoves(nextMoves, board);

        //Checks if any of them would allow you to capture
        int nextScore = 0;
        for (int i = 0; i < nextMoves.size() - 1; i++)
        {
            if (nextMoves[i].from() == moves[i].to())
            {
                nextScore += ScoreToAdd(board, nextMoves[i]);
            }
        }
        moves[i].setScore(nextScore * MOVE_SCORE_REDUCTION + moves[i].score());

        //Resets board to actual current state
        board.unmakeMove(moves[i]);
        
    }
    //Gets highest value move
    for (int i = moves.size() - 1; i >= 0; i--)
    {
        if (moves[i].score() > move.score())
        {
            move = moves[i];
        }
    }

    std::cout << move.score() << "\n";

    //Returns chosen move
    return chess::uci::moveToUci(move);
}


int ScoreToAdd(chess::Board board, auto move)
{
    int score = 0;
    //Adds score based on who you will capture
    if (board.isCapture(move))
    {
        chess::Piece type = board.at(move.to());
        if (type == chess::PieceType::KING)
        {
            score += KING_SCORE;
        }
        else if (type == chess::PieceType::QUEEN)
        {
            score += QUEEN_SCORE;
        }
        else if (type == chess::PieceType::ROOK)
        {
            score += ROOK_SCORE;
        }
        else if (type == chess::PieceType::KNIGHT)
        {
            score += KNIGHT_SCORE;
        }
        else if (type == chess::PieceType::BISHOP)
        {
            score += BISHOP_SCORE;
        }
        else
        {
            score += PAWN_SCORE;
        }
    }
    return score;
}