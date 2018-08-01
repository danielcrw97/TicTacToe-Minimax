#pragma once
enum class BoardEntry {
	EMPTY,
	PLAYER,
	AI
};

enum class GameWinner {
	PLAYER,
	AI,
	NONE
};

typedef std::array<std::array<BoardEntry, 3>, 3> Size3Matrix;

class BoardModel
{	
public:
	BoardModel();

	BoardModel(BoardModel const& model, std::pair<int, int> const& pair, BoardEntry entry);

	BoardModel(Size3Matrix model);

	GameWinner getWinner();

	std::vector<std::pair<int,int>> getRemainingMoves();

	Size3Matrix getBoard() const;

	void printBoard();

private:
	Size3Matrix board;

	void makeMove(std::pair<int, int> move, BoardEntry entry);

	GameWinner getArrayWinner(std::array<BoardEntry, 3> column);

	GameWinner getWinnerFromEntry(BoardEntry entry);

	static Size3Matrix reverseBoard(Size3Matrix board);
};

