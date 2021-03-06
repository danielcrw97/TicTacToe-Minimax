// TicTacToe.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <algorithm>
#include <iostream>
#include "GameInfo.h"
#include "BoardModel.h"
#include "GameNode.h"
#include <map>

typedef std::pair<int, int> IntPair;

bool parseYesOrNo(std::string firstInput)
{
	std::transform(firstInput.begin(), firstInput.end(), firstInput.begin(), tolower);
	if (firstInput == "y" || firstInput == "yes" || firstInput == "yeah")
	{
		return true;
	}
	else if (firstInput == "n" || firstInput == "no" || firstInput == "nope")
	{
		return false;
	}
	else
	{
		std::cout << "\n I didn't catch that. Please enter 'yes' or 'no'.\n";
		std::cin >> firstInput;
		return parseYesOrNo(firstInput);
	}
}

IntPair parsePlayerMove(std::string input, std::vector<IntPair> possibleMoves)
{
	std::transform(input.begin(), input.end(), input.begin(), tolower);
	if (input.size() == 2)
	{
		try
		{
			int x = input[0] - '0';
			int y = input[1] - '0';
			std::pair<int, int> move(x, y);
			if (std::find(possibleMoves.begin(), possibleMoves.end(), move) != possibleMoves.end())
			{
				return move;
			}
		}
		catch (std::exception& e)
		{
			std::cout << "Invalid coordinates" << std::endl;
		}
	}

	std::cout << "Please enter a 'x' value followed by a 'y' value moving from the top left corner." << std::endl;
	std::cout << "X/Y values start from 0, e.g. for the bottom right square enter '22'." << std::endl;
	std::cin >> input;
	return parsePlayerMove(input, possibleMoves);
}

void addChildren(std::shared_ptr<GameNode> rootNode, bool isPlayersTurn)
{
	BoardModel& rootModel = rootNode->getBoard();
	std::vector<IntPair> remainingMoves = rootModel.getRemainingMoves();

	for (auto move : remainingMoves)
	{
		BoardModel model;
		if (isPlayersTurn)
		{
			model = BoardModel(rootModel, move, BoardEntry::PLAYER);
		}
		else
		{
			model = BoardModel(rootModel, move, BoardEntry::AI);
		}
		std::shared_ptr<GameNode> child = std::make_shared<GameNode>(GameNode(std::move(model), !(rootNode->isPlayersTurn()), false));
		rootNode->addChild(child);
		child->setParent(rootNode);
	}
}

int miniMaxNode(std::shared_ptr<GameNode> node)
{
	addChildren(node, node->isPlayersTurn());
	BoardModel& board = node->getBoard();
	if (board.getWinner() == GameWinner::PLAYER)
	{
		return -1;
	}
	else if (board.getWinner() == GameWinner::AI)
	{
		return 1;
	}
	else if (board.getRemainingMoves().size() == 0)
	{
		return 0;
	}
	std::vector<std::shared_ptr<GameNode>> children = node->getChildren();
	std::map<std::shared_ptr<GameNode>, int> childValues;
	for (size_t i = 0; i < children.size(); ++i)
	{
		std::shared_ptr<GameNode> child = children.at(i);
		int value = miniMaxNode(child);
		childValues.insert(std::pair<std::shared_ptr<GameNode>, int>(child, value));
	}

	if (!node->isPlayersTurn())
	{
		int max = -2;
		for (std::shared_ptr<GameNode> child : children)
		{
			int childValue = childValues.at(child);
			if (childValue > max)
			{
				max = childValue;
			}
		}
		return max;
	}
	else
	{
		int min = 2;
		for (std::shared_ptr<GameNode> child : children)
		{
			int childValue = childValues.at(child);
			if (childValue < min)
			{
				min = childValue;
			}
		}
		return min;
	}
}

IntPair getNextMove(std::shared_ptr<GameNode> rootNode)
{
	addChildren(rootNode, false);
	std::vector<std::shared_ptr<GameNode>> children = rootNode->getChildren();

	// Could return [0, 0] for first go to save time, but feels like cheating.

	int index = 0;
	int max = -1;
	for (size_t i = 0; i < children.size(); ++i)
	{
		int value = miniMaxNode(children.at(i));
		if (value > max)
		{
			max = value;
			index = i;
		}
	}

	IntPair nextMove = rootNode->getBoard().getRemainingMoves().at(index);

	return nextMove;
}

void gameLoop()
{
	std::string openString = "Welcome to Tic-Tac-Toe!\n"
		"Would you like to go first?\n";
	std::cout << openString;

	std::string goFirst;
	std::cin >> goFirst;
	std::cout << std::endl;
	GameInfo::getInstance().setPlayerFirst(parseYesOrNo(goFirst));
	
	std::cout << "Would you like to be O?" << std::endl;
	std::string playerO;
	std::cin >> playerO;
	GameInfo::getInstance().setPlayerO(parseYesOrNo(playerO));
	std::cout << std::endl;

	BoardModel currentBoard;
	GameWinner winner;
	bool gameOver = false;
	bool isPlayersTurn = GameInfo::getInstance().isPlayerFirst();
	while (!gameOver)
	{
		currentBoard.printBoard();
		if (isPlayersTurn)
		{
			std::cout << "Enter your next move:" << std::endl;
			std::string playerMove;
			std::cin >> playerMove;
			std::pair<int, int> move = parsePlayerMove(playerMove, currentBoard.getRemainingMoves());
			currentBoard = BoardModel(currentBoard, move, BoardEntry::PLAYER);
		}
		else
		{
			std::cout << "AI is choosing..." << std::endl;
			std::shared_ptr<GameNode> root = std::make_shared<GameNode>(GameNode(currentBoard, false, true));
			IntPair aiMove = getNextMove(root);
			currentBoard = BoardModel(currentBoard, aiMove, BoardEntry::AI);
		}
		std::cout << std::endl;
		if ((currentBoard.getWinner() != GameWinner::NONE) || (currentBoard.getRemainingMoves().size() == 0))
		{
			winner = currentBoard.getWinner();
			gameOver = true;
		}
		isPlayersTurn = !isPlayersTurn;
	}

	currentBoard.printBoard();
	if (winner == GameWinner::PLAYER)
	{
		std::cout << "Congratulations! You won!" << std::endl;
	}
	else if (winner == GameWinner::AI)
	{
		std::cout << "You lost!" << std::endl;
	}
	else
	{
		std::cout << "It was a draw." << std::endl;
	}

	std::cout << "Would you like to play again?" << std::endl;

	std::string playAgain;
	std::cin >> playAgain;

	if (parseYesOrNo(playAgain))
	{
		std::cout << std::endl;
		gameLoop();
	}
}

int main()
{
	gameLoop();
    return 0;
}

