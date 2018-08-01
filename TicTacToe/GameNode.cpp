#include "stdafx.h"
#include "GameNode.h"
#include "BoardModel.h"
#include "GameInfo.h"


GameNode::GameNode(BoardModel model, bool isPlayersTurn, bool isRootNode) :
	model(model), playersTurn(isPlayersTurn), rootNode(isRootNode)
{
}

void GameNode::addChild(std::shared_ptr<GameNode> node)
{
	children.push_back(node);
}

std::vector<std::shared_ptr<GameNode>>& GameNode::getChildren()
{
	return children;
}

void GameNode::setParent(std::weak_ptr<GameNode> newParent)
{
	parent = newParent;
}

bool GameNode::isPlayersTurn()
{
	return playersTurn;
}

bool GameNode::isRootNode()
{
	return rootNode;
}

BoardModel& GameNode::getBoard()
{
	return model;
}

