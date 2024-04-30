#include "chess-simulator.h"
// disservin's lib. drop a star on his hard work!
// https://github.com/Disservin/chess-library
#include "chess.hpp"
#include <random>
using namespace ChessSimulator;


const int KING_SCORE = 2000;
const int QUEEN_SCORE = 90;
const int ROOK_SCORE = 50;
const int KNIGHT_SCORE = 30;
const int BISHOP_SCORE = 30;
const int PAWN_SCORE = 10;
const float MOVE_SCORE_REDUCTION = 0.1f; //Multiplies this by the effects of future moves
const float DEFENSIVE_WEIGHT = 0.0f; //Multiplies this by the score loss of losing a piece

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

        int currentPieceValue = PieceValue(board.at(moves[i].from()));
        
        //Makes the move, then the enemy's random move, so that it can check what this move allows
        chess::Board nextBoard = board;
        nextBoard.makeMove(moves[i]);
        chess::Movelist enemyMoves;
        chess::movegen::legalmoves(enemyMoves, nextBoard);

        //Checks if any of them would allow you to capture
        int nextScore = 0;
        if (enemyMoves.size() > 0)
        {
            nextBoard.makeMove(enemyMoves[0]);
            chess::Movelist nextMoves;
            chess::movegen::legalmoves(nextMoves, nextBoard);
            for (int j = 0; j < nextMoves.size() - 1; j++)
            {
                if (nextMoves[j].from() == moves[i].to())
                {
                    nextScore += ScoreToAdd(nextBoard, nextMoves[j]);
                }
            }
        }

        //Checks whether this move would be in range of any enemy piece
        int lossScore = 0;
        for (int j = 0; j < enemyMoves.size()-1; j++)
        {
            if (enemyMoves[j].to() == moves[i].to())
            {
                lossScore = currentPieceValue * -1;
                break;
            }
        }

        moves[i].setScore((nextScore * MOVE_SCORE_REDUCTION) + (lossScore * DEFENSIVE_WEIGHT) + moves[i].score());

    }
    //Gets highest value move
    for (int i = moves.size() - 1; i >= 0; i--)
    {
        if (moves[i].score() > move.score())
        {
            move = moves[i];
        }
    }

    //Outputs score
    //std::cout << move.score() << "\n";

    //Returns chosen move
    return chess::uci::moveToUci(move);
}


int PieceValue(chess::Piece type)
{
    int score = 0;
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
    return score;
}

int ScoreToAdd(chess::Board board, auto move)
{
    //Adds score based on who you will capture
    if (board.isCapture(move))
    {
        return PieceValue(board.at(move.to()));
    }
    return 0;
}