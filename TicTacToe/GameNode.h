#pragma once

#include "BoardModel.h"



/**
* A simple wrapper around a BoardModel that contains pointers to children/parent
* nodes, allowing the AI to form a tree of all possible board states.
*/
class GameNode
{
public:
	GameNode(BoardModel model, bool isPlayersTurn, bool isRootNode);
	void addChild(std::shared_ptr<GameNode> node);
	std::vector<std::shared_ptr<GameNode>>& getChildren();
	void setParent(std::weak_ptr<GameNode> parent);
	bool isPlayersTurn();
	bool isRootNode();
	BoardModel& getBoard();

private:
	std::weak_ptr<GameNode> parent;
	std::vector<std::shared_ptr<GameNode>> children;

	BoardModel model;
	bool playersTurn;
	bool rootNode;
};

