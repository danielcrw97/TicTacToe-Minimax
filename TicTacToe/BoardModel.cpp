#include "stdafx.h"
#include "BoardModel.h"
#include "GameInfo.h"
#include <iostream>

typedef std::array<std::array<BoardEntry, 3>, 3> Size3Matrix;

BoardModel::BoardModel()
{
	for (size_t i = 0; i < board.size(); ++i)
	{
		for (size_t j = 0; j < board.size(); ++j)
		{
			board[i][j] = BoardEntry::EMPTY;
		}
	}
}

BoardModel::BoardModel(const BoardModel& model, std::pair<int, int> const& pair, BoardEntry entry)
{
	Size3Matrix newModel = model.getBoard();
	newModel[pair.first][pair.second] = entry;
	this->board = newModel;
}

BoardModel::BoardModel(Size3Matrix model) :
	board(model)
{
}

GameWinner BoardModel::getWinner()
{
	// Check colummns 
	for (size_t i = 0; i < board.size(); ++i)
	{
		// Check nth column
		std::array<BoardEntry, 3> column = board[i];
		GameWinner colWinner = getArrayWinner(column);	
		if (colWinner != GameWinner::NONE)
		{
			return colWinner;
		}
	}

	// Flip here
	Size3Matrix flippedBoard = reverseBoard(this->board);

	// Check of columns of flipped board (i.e. rows)
	for (size_t i = 0; i < board.size(); ++i)
	{
		// Check nth row
		std::array<BoardEntry, 3> row = flippedBoard[i];
		GameWinner rowWinner = getArrayWinner(row);
		if (rowWinner != GameWinner::NONE)
		{
			return rowWinner;
		}
	}


	// Check downwards diagonal.
	BoardEntry firstEntry = board[0][0];
	if (firstEntry != BoardEntry::EMPTY)
	{
		bool downDiagonalWin = true;
		for (size_t i = 1; i < board.size(); ++i)
		{
			int j = i;
			if (board[i][j] != firstEntry)
			{
				downDiagonalWin = false;
				break;
			}
		}
		if (downDiagonalWin)
		{
			return getWinnerFromEntry(firstEntry);
		}
	}

	// Check upwards diagonal
	firstEntry = board[0][board.size() - 1];
	if (firstEntry != BoardEntry::EMPTY)
	{
		bool upDiagonalWin = true;
		for (size_t i = 1; i < board.size(); ++i)
		{
			int j = board.size() - (1 + i);
			if (board[i][j] != firstEntry)
			{
				upDiagonalWin = false;
				break;
			}
		}
		if (upDiagonalWin)
		{
			return getWinnerFromEntry(firstEntry);
		}
	}

	return GameWinner::NONE;
}

std::vector<std::pair<int,int>> BoardModel::getRemainingMoves()
{
	std::vector<std::pair<int, int>> moves;
	for (size_t i = 0; i < board.size(); ++i)
	{
		for (size_t j = 0; j < board.size(); ++j)
		{
			if (board[i][j] == BoardEntry::EMPTY)
			{
				moves.push_back(std::pair<int, int>(i, j));
			}
		}
	}
	return moves;
}

Size3Matrix BoardModel::getBoard() const
{
	return board;
}

void BoardModel::printBoard()
{
	std::string player = "O";
	std::string ai = "X";
	if (!GameInfo::getInstance().isPlayerO())
	{
		player = 'X';
		ai = 'O';
	}

	Size3Matrix reversedBoard = reverseBoard(this->board);

	for (size_t i = 0; i < board.size(); ++i)
	{
		std::cout << "||";
		for (size_t j = 0; j < board.size(); ++j)
		{
			if (reversedBoard[i][j] == BoardEntry::AI)
			{
				std::cout << (" " + ai + " ||");
			}
			else if (reversedBoard[i][j] == BoardEntry::PLAYER)
			{
				std::cout << (" " + player + " ||");
			}
			else
			{
				std::cout << "   ||";
			}
		}
		std::cout << "\n" << std::endl;
	}
}

void BoardModel::makeMove(std::pair<int, int> move, BoardEntry entry)
{
	board[move.first][move.second] = entry;
}

GameWinner BoardModel::getArrayWinner(std::array<BoardEntry, 3> column)
{
	BoardEntry firstEntry = column[0];
	bool allEqual = true;

	if (firstEntry == BoardEntry::EMPTY)
	{
		return GameWinner::NONE;
	}
	else
	{
		for (size_t j = 1; j < board.size(); ++j)
		{
			if (column[j] != firstEntry)
			{
				allEqual = false;
			}
		}
	}

	if (allEqual)
	{
		return getWinnerFromEntry(firstEntry);
	}
	else
	{
		return GameWinner::NONE;
	}
}

GameWinner BoardModel::getWinnerFromEntry(BoardEntry entry)
{
	if (entry == BoardEntry::AI)
	{
		return GameWinner::AI;
	}
	else
	{
		return GameWinner::PLAYER;
	}
}

Size3Matrix BoardModel::reverseBoard(Size3Matrix board)
{
	Size3Matrix reversedBoard = board;
	for (size_t i = 0; i < board.size(); ++i)
	{
		for (size_t j = 0; j < board.size(); ++j)
		{
			reversedBoard[i][j] = board[j][i];
		}
	}
	return reversedBoard;
}
