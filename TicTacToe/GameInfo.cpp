#include "stdafx.h"
#include "GameInfo.h"

std::unique_ptr<GameInfo> GameInfo::instance = nullptr;

GameInfo& GameInfo::getInstance()
{
	if (instance.get() == nullptr)
	{
		instance.reset(new GameInfo());
	}
	return *instance;
}

void GameInfo::setPlayerFirst(bool playerFirst)
{
	this->playerFirst = playerFirst;
}

void GameInfo::setPlayerO(bool playerO)
{
	this->playerO = playerO;
}

bool GameInfo::isPlayerFirst()
{
	return this->playerFirst;
}

bool GameInfo::isPlayerO()
{
	return this->playerO;
}

GameInfo::GameInfo()
{
}

