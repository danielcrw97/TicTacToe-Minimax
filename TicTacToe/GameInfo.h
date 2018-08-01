#pragma once

class GameInfo
{
public:
	static GameInfo& getInstance();
	void setPlayerFirst(bool playerFirst);
	void setPlayerO(bool playerO);
	bool isPlayerFirst();
	bool isPlayerO();

private:
	static std::unique_ptr<GameInfo> instance;
	bool playerFirst;
	bool playerO;

	GameInfo();
};

