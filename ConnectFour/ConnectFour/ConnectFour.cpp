// ConnectFour.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

/*
How board is constructed:
<-bot--------top->
[
[x, x, x, x, x, x]
[x, x, x, x, x, x]
[x, x, x, x, x, x]
[x, x, x, x, x, x]
[x, x, x, x, x, x]
[x, x, x, x, x, x]
[x, x, x, x, x, x]
]
*/

#include <iostream>
#include <string>
#include <array>
#include <tuple>

constexpr auto ROW_SIZE = 7;
constexpr auto COLUMN_SIZE = 6;

enum PlayerState
{
	NONE,
	PLAYER1,
	PLAYER2
};

enum PlayerMode
{
	HUMAN,
	COMPUTER
};

struct Coord2D
{
	int row;
	int elem;
};

class Board
{
private:
	std::array<std::array<PlayerState, COLUMN_SIZE>, ROW_SIZE> m_board = {};
	std::array<int, 7> m_rowStackPtr = {}; // make sure that the size of this array is equal to the length of m_board

public:
	Board()
	{
	}
	
	int getRowLen() { return ROW_SIZE; }
	bool checkRowFull(int row) { return (m_rowStackPtr[row] == COLUMN_SIZE); } // returns true if row is full, false otherwise
	
	bool checkBoardFull()
	{
		bool boardFull = false;
		for (int i = 0; i < ROW_SIZE; i++)
		{
			if (!checkRowFull(i))
			{
				return false;
			}
		}
		return true;
	}

	std::string cellStateToString(const PlayerState &state)
	{
		switch (state)
		{
		case(NONE):	return " ";
		case(PLAYER1):	return "O";
		case(PLAYER2):	return "X";
		default: return "!";
		}
	}

	void drawBoard()
	{
		for (int elem = COLUMN_SIZE-1; elem >= 0; elem--)
		{
			for (int row = 0; row < ROW_SIZE; row++)
			{
				std::cout << '|' << cellStateToString(m_board[row][elem]);
			}
			std::cout << "|\n";
		}
		for (int i = 0; i < ROW_SIZE; i++)
		{
			std::cout << '|' << i;
		}
		std::cout << "|\n";
	}

	int dropDisc(int row, PlayerState plState) // returns the columnar position of dripped disc
	{ 
		m_board[row][m_rowStackPtr[row]++] = plState; 
		return m_rowStackPtr[row]-1;
	}

	std::tuple<int, int, int> resetPtrAndLen(int rowIndex, int elemIndex)
	{
		return { rowIndex, elemIndex, 1 };
	}

	PlayerState checkWinner(PlayerState player, int rowIndex, int elemIndex)
	{
		int ptrRow;
		int ptrElem;
		int lineLen;

		// horizontal (across row) check
		std::tie(ptrRow, ptrElem, lineLen) = resetPtrAndLen(rowIndex, elemIndex);
		while (true) // direction: left
		{
			ptrRow--;
			if ( !(ptrRow >= 0 && m_board[ptrRow][ptrElem] == player) ) break;
			lineLen++;
		}
		ptrRow = rowIndex; // return row pointer to initial position
		while (true) // direction: right
		{
			ptrRow++;
			if ( !(ptrRow < ROW_SIZE && m_board[ptrRow][ptrElem] == player) ) break;
			lineLen++;
		}
		if (lineLen >= 4) return player;

		// vertical (along row) check
		std::tie(ptrRow, ptrElem, lineLen) = resetPtrAndLen(rowIndex, elemIndex);
		while (true) // direction: down
		{
			ptrElem--;
			if ( !(ptrElem >= 0 && m_board[ptrRow][ptrElem] == player) ) break;
			lineLen++;
		}
		ptrElem = elemIndex; // return elem pointer to initial position
		while (true) // direction: up
		{
			ptrElem++;
			if ( !(ptrElem < COLUMN_SIZE && m_board[ptrRow][ptrElem] == player) ) break;
			lineLen++;
		}
		if (lineLen >= 4) return player;

		// diagonal (/) check
		std::tie(ptrRow, ptrElem, lineLen) = resetPtrAndLen(rowIndex, elemIndex);
		while (true) // direction: left & down
		{
			ptrRow--;
			ptrElem--;
			if ( !((ptrRow >= 0 && ptrElem >= 0) && m_board[ptrRow][ptrElem] == player) ) break;
			lineLen++;
		}
		// return both pointer to initial position
		ptrRow = rowIndex;
		ptrElem = elemIndex;
		while (true) // direction: right & up
		{
			ptrRow++;
			ptrElem++;
			if ( !((ptrRow < ROW_SIZE && ptrElem < COLUMN_SIZE) && m_board[ptrRow][ptrElem] == player) ) break;
			lineLen++;
		}
		if (lineLen >= 4) return player;

		// diagonal (\) check
		std::tie(ptrRow, ptrElem, lineLen) = resetPtrAndLen(rowIndex, elemIndex);
		while (true) // direction: left & up
		{
			ptrRow--;
			ptrElem++;
			if ( !((ptrRow >= 0 && ptrElem < COLUMN_SIZE) && m_board[ptrRow][ptrElem] == player) ) break;
			lineLen++;
		}
		// return both pointer to initial position
		ptrRow = rowIndex;
		ptrElem = elemIndex;
		while (true) // direction: right & down
		{
			ptrRow++;
			ptrElem--;
			if ( !((ptrRow < ROW_SIZE && ptrElem >= 0) && m_board[ptrRow][ptrElem] == player) ) break;
			lineLen++;
		}
		if (lineLen >= 4) return player;

		return NONE;
	}
};

class Player
{
private:
	PlayerState m_playerState;
	PlayerMode m_playerMode;

public:
	Player(PlayerState plState, PlayerMode plMode)
		: m_playerState(plState), m_playerMode(plMode)
	{
	}

	PlayerState getPlayerState() { return m_playerState; }
	PlayerMode getPlayerMode() { return m_playerMode; }

	int chooseRow(Board b)
	{
		int row;

		if (m_playerMode == HUMAN)
		{
			while (true)
			{
				std::cout << "Enter the row you would like to drop your disc in: ";
				std::cin >> row;
				if (row >= b.getRowLen())
				{
					std::cout << "Row index cannot be higher than " << b.getRowLen() - 1 << ".\n";
					continue;
				}
				else if (b.checkRowFull(row))
				{
					std::cout << "The row is full!\n";
					continue;
				}
				break;
			}
		}
		else if (m_playerMode == COMPUTER)
		{
			// Best choice only
		}
		else
		{
			// throw error
		}
		return row;
	}
};

int main()
{
	// Initialisation
	Board board;

	std::array<Player, 2> players = { Player(PLAYER1, HUMAN), Player(PLAYER2, HUMAN) };

	PlayerState winner = NONE;
	bool isDraw = false;

	// Game
	while (!winner && !isDraw)
	{
		for (auto &player : players)
		{
			system("cls");
			std::cout << player.getPlayerState() << "'s turn:\n";
			board.drawBoard();
			int rowNo = player.chooseRow(board);
			int elemNo = board.dropDisc(rowNo, player.getPlayerState());
			winner = board.checkWinner(player.getPlayerState(), rowNo, elemNo);			
			isDraw = board.checkBoardFull();

			if (winner || isDraw) break;
		}
	}

	// End
	system("cls");
	board.drawBoard();
	if (winner != NONE)	std::cout << "Player " << winner << " is the winner!\n";
	else if (isDraw)	std::cout << "It's a draw!\n";

	return 0;

	return 0;
}