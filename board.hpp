#ifndef BOARD_H
#define BOARD_H

#include <SFML/Graphics.hpp>
#include <time.h>

class Board {
public: // Initialize the board with the user-provided input
	Board();
	struct BoardType {
		int boardSize, numberOfSpaces, numberOfBombs, potentialMoves;
		int **matrix;
		bool **clicked;
		bool **flags;
		std::vector< std::vector<sf::RectangleShape> > spots;
		std::vector< std::vector<sf::Text> > numbers;
		sf::Font font;

		//Style
		sf::RectangleShape leftBorder;
		sf::RectangleShape rightBorder;
		sf::RectangleShape upperBorder;
		sf::RectangleShape lowerBorder;
		sf::RectangleShape menuBorder;

		//Additional style/non-core functionality elements
		sf::Text bombCounter;
		sf::Text timer;
		int time;
		bool firstClick;
	};

	// Asyncronous timer function
	void asyncTimer(Board::BoardType *board); 

	// Initialize the board data structure 
	void initBoard(Board::BoardType *board);

	// For development purposes
	void debugBoard(Board::BoardType *board);

	// Place the bombs at random
	void placeBombs(Board::BoardType *board);

	// Initialize the numerical values corresponding to adjacent bombs
	void assignValues(Board::BoardType *board);

	// Check the games winstate
	bool checkWinState(Board::BoardType *board);

	// Manages the logic for the humans turn
	void bombClicked(Board::BoardType *board, int xpos, int ypos);

	// Manage the location of and logic behind flags
	void flagPlaced(Board::BoardType *board, int xpos, int ypos);

	// Manages the actual move logic
	void makeMove(Board::BoardType *board, int row, int column);

	// Utility function to determine if a space is on the board. Used to prevent indexing errors in conditional logic
	bool checkInBounds(int boardSize, int row, int column);

	// Clean up memory (as needed)
	void cleanGame(Board::BoardType *board);
};

#endif /* BOARD_H */