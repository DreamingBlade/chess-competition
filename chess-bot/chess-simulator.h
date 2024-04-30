#pragma once
#include <string>
#include "chess-simulator.h"
#include "chess.hpp"
#include <random>

namespace ChessSimulator {
/**
 * @brief Move a piece on the board
 *
 * @param fen The board as FEN
 * @return std::string The move as UCI
 */
std::string Move(std::string fen);
} // namespace ChessSimulator
int ScoreToAdd(chess::Board board, auto move);
int PieceValue(chess::Piece type);
