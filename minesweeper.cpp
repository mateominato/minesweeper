#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <vector>
#include <chrono>
#include <thread>

#include "board.hpp"

using namespace std;

const int TILESIZE = 35;
const int INITIAL_X_BORDER = 10;
const int INITIAL_Y_BORDER = 100;
const int TEXT_SIZE = 25;
const int OUTLINE_THICKNESS = 1;
const int BOARD_SIZE = 22;

//Add flag functionality
//Add winstate management

Board::Board() {}

void Board::asyncTimer(Board::BoardType *board) {
	for (int i=0; i<999; i++) {
		board->time++;
		board->timer.setString(to_string(board->time));
    	this_thread::sleep_for(std::chrono::seconds(1));
	}
}

void Board::initBoard(Board::BoardType *board) {
	//Initilizing time seed, font, constants
	srand(time(NULL));
	board->time = 0;
	if(!board->font.loadFromFile("arial.ttf"))
	{
	    std::cout << "can't load font" << std::endl;
	}
	board->firstClick = true;

	//Stylistic borders
	board->leftBorder.setSize(sf::Vector2f(10, 880));
	board->leftBorder.setPosition(0, 0);
	board->leftBorder.setFillColor(sf::Color(70, 70, 70));
	board->rightBorder.setSize(sf::Vector2f(10, 880));
	board->rightBorder.setPosition(780, 0);
	board->rightBorder.setFillColor(sf::Color(70, 70, 70));
	board->upperBorder.setSize(sf::Vector2f(790, 10));
	board->upperBorder.setPosition(0, 90);
	board->upperBorder.setFillColor(sf::Color(70, 70, 70));
	board->lowerBorder.setSize(sf::Vector2f(790, 10));
	board->lowerBorder.setPosition(0, 870);
	board->lowerBorder.setFillColor(sf::Color(70, 70, 70));
	board->menuBorder.setSize(sf::Vector2f(770, 10));
	board->menuBorder.setPosition(10, 0);
	board->menuBorder.setFillColor(sf::Color(70, 70, 70));

	//Timer
	board->timer.setString("0");
	board->timer.setFont(board->font);
	board->timer.setCharacterSize(50);
	board->timer.setFillColor(sf::Color::Black);
	board->timer.setOutlineColor(sf::Color::White);
	board->timer.setPosition(sf::Vector2f(680, 20));

	//Class constant intializers
	board->boardSize = BOARD_SIZE;
	board->numberOfSpaces = (BOARD_SIZE*BOARD_SIZE);
	board->numberOfBombs = (board->numberOfSpaces)/5;
	board->potentialMoves = (board->numberOfSpaces)-(board->numberOfBombs);

	board->matrix = new int*[board->boardSize];
	for (int i = 0; i < board->boardSize; i++) {
		board->matrix[i] = new int[board->boardSize];
		for (int j = 0; j < board->boardSize; j++) {
			board->matrix[i][j] = 0; //Initialize all values to 0
		}
	}

	//Initialize the vector of rectangles to interact with
	board->spots.resize(BOARD_SIZE);
	for (int i=0; i<BOARD_SIZE; i++) {
		board->spots[i].resize(BOARD_SIZE);
	}

	for (int i=0;i<board->boardSize;i++) {
		for (int j=0;j<board->boardSize;j++) {
			board->spots[i][j].setOutlineColor(sf::Color::Black);
			board->spots[i][j].setOutlineThickness(OUTLINE_THICKNESS);
			board->spots[i][j].setFillColor(sf::Color(124, 124, 124));
			board->spots[i][j].setSize(sf::Vector2f(TILESIZE, TILESIZE));
			board->spots[i][j].setPosition(sf::Vector2f(i*TILESIZE+INITIAL_X_BORDER, j*TILESIZE+INITIAL_Y_BORDER));
		}
	}

	//Initialize matrix to determine clicked tiles
	board->clicked = new bool*[board->boardSize];
	for (int i = 0; i < board->boardSize; i++) {
		board->clicked[i] = new bool[board->boardSize];
		for (int j = 0; j < board->boardSize; j++) {
			board->clicked[i][j] = false; //Initialize all values to 0
		}
	}

	//Matrix to determine flag locations
	board->flags = new bool*[board->boardSize];
	for (int i = 0; i < board->boardSize; i++) {
		board->flags[i] = new bool[board->boardSize];
		for (int j = 0; j < board->boardSize; j++) {
			board->flags[i][j] = false; //Initialize all values to 0
		}
	}

	//Utility functions to place bombs and calculate numerical values for tiles
	placeBombs(board);
	assignValues(board);

	//Initialize the vector of numbers to display, if appropriate per the clicked matrix
	board->numbers.resize(BOARD_SIZE);
	for (int i=0; i<BOARD_SIZE; i++) {
		board->numbers[i].resize(BOARD_SIZE);
	}
	for (int i=0;i<board->boardSize;i++) {
		for (int j=0;j<board->boardSize;j++) {
			board->numbers[i][j].setString(to_string(board->matrix[i][j]));
			board->numbers[i][j].setFont(board->font);
			board->numbers[i][j].setCharacterSize(TEXT_SIZE);
			board->numbers[i][j].setFillColor(sf::Color::Black);
			board->numbers[i][j].setOutlineColor(sf::Color::White);
			board->numbers[i][j].setPosition(sf::Vector2f(((i*TILESIZE)+INITIAL_X_BORDER+10), ((j*TILESIZE)+INITIAL_Y_BORDER+2)));
		}
	}
}

void Board::placeBombs(Board::BoardType *board) {
	//Place the bombs at random
	int row, column, i;
	for (i=board->numberOfBombs; i>0; i--) {
		row = rand()%(board->boardSize);
		column = rand()%(board->boardSize);
		if (board->matrix[row][column] != -1) {
			board->matrix[row][column] = -1;
		} else { //This spot was already a bomb--choose another
			i++;
		}
	}
	//Menu build
	board->bombCounter.setString(to_string(board->numberOfBombs));
	board->bombCounter.setFont(board->font);
	board->bombCounter.setCharacterSize(50);
	board->bombCounter.setFillColor(sf::Color::Black);
	board->bombCounter.setOutlineColor(sf::Color::White);
	board->bombCounter.setPosition(sf::Vector2f(30, 20));
}

void Board::assignValues(Board::BoardType *board) {
	int row, column, bombCount;
	for (row=0; row<board->boardSize; row++) {
		for (column=0; column<board->boardSize; column++) {
			if (board->matrix[row][column] != -1) {
				bombCount = 0;
				//Upper left
				if (checkInBounds(board->boardSize, row-1, column-1) && board->matrix[row-1][column-1] == -1) {
					bombCount++;
				}
				//Straight up
				if (checkInBounds(board->boardSize, row-1, column) && board->matrix[row-1][column] == -1) {
					bombCount++;
				}
				//Upper right
				if (checkInBounds(board->boardSize, row-1, column+1) && board->matrix[row-1][column+1] == -1) {
					bombCount++;
				}
				//Left
				if (checkInBounds(board->boardSize, row, column-1) && board->matrix[row][column-1] == -1) {
					bombCount++;
				}
				//Right
				if (checkInBounds(board->boardSize, row, column+1) && board->matrix[row][column+1] == -1) {
					bombCount++;
				}
				//Lower left
				if (checkInBounds(board->boardSize, row+1, column-1) && board->matrix[row+1][column-1] == -1) {
					bombCount++;
				}
				//Straight down
				if (checkInBounds(board->boardSize, row+1, column) && board->matrix[row+1][column] == -1) {
					bombCount++;
				}
				//Lower right
				if (checkInBounds(board->boardSize, row+1, column+1) && board->matrix[row+1][column+1] == -1) {
					bombCount++;
				}
				board->matrix[row][column] = bombCount;
			}
		}
	}
}

bool Board::checkWinState(Board::BoardType *board) { //Determine when the game is over
	for (int i = 0; i < board->boardSize; i++) {
		for (int j = 0; j < board->boardSize; j++) {
			if (board->matrix[i][j] != -1 && !board->clicked[i][j]) {
				return false;
			}
		}
	}
	return true;
}

void Board::debugBoard(Board::BoardType *board) { //Purely for testing purposes
	cout << "  ";
	for (int i=0; i<board->boardSize; i++) {
		cout << " " << i;
	}
	cout << "\n";
	for (int i=0;i<board->boardSize;i++) {
		cout << " " << i;
		for (int j=0;j<board->boardSize;j++) {
			cout << " " << board->matrix[j][i];
		}
		cout << "\n";
	}
	cout << "\n";
}

void Board::bombClicked(Board::BoardType *board, int xpos, int ypos) {
	if (board->firstClick) {
		//auto future = async(&Board::asyncTimer, this, board);
		thread timerThread(&Board::asyncTimer, this, board);
		timerThread.detach();
		board->firstClick = false;
	}

	int row = ((xpos-INITIAL_X_BORDER)/TILESIZE);
	int column = ((ypos-INITIAL_Y_BORDER)/TILESIZE);
	if (checkInBounds(board->boardSize, row, column) && board->matrix[row][column] == -1) { //If a bomb is clicked
		cout << "You hit a bomb! You lose!";
		exit(0);
	} else if (checkInBounds(board->boardSize, row, column) && board->matrix[row][column] == 0 && !board->flags[row][column]) {
		//If the spot clicked has no adjacent bombs, open up the appropriate number of spaces
		makeMove(board, row, column);
	} else if (checkInBounds(board->boardSize, row, column) && !board->flags[row][column]) {
		//Otherwise, just reveal the current spot
		board->clicked[row][column] = true;
	}
}

void Board::flagPlaced(Board::BoardType *board, int xpos, int ypos) {
	if (board->firstClick) {
		//auto future = async(&Board::asyncTimer, this, board);
		thread timerThread(&Board::asyncTimer, this, board);
		timerThread.detach();
		board->firstClick = false;
	}

	int row = ((xpos-INITIAL_X_BORDER)/TILESIZE);
	int column = ((ypos-INITIAL_Y_BORDER)/TILESIZE);
	if (checkInBounds(board->boardSize, row, column)) {
		if (!board->flags[row][column]) { //If theres no flag at the clicked square, place a flag
			board->flags[row][column] = true;
			board->numberOfBombs--;
			board->bombCounter.setString(to_string(board->numberOfBombs));
			board->spots[row][column].setFillColor(sf::Color::Red);
		} else { //Otherwise, unselect the clicked square (make it no longer a flagged location)
			board->flags[row][column] = false;
			board->numberOfBombs++;
			board->bombCounter.setString(to_string(board->numberOfBombs));
			board->spots[row][column].setFillColor(sf::Color(124, 124, 124));
		}
	}
}

void Board::makeMove(Board::BoardType *board, int row, int column) {
	//Logic for which tiles to show if a zero spot is clicked
	board->clicked[row][column] = true;
	//Upper left
	if (checkInBounds(board->boardSize, row-1, column-1) && !board->clicked[row-1][column-1] && !board->flags[row-1][column-1]) {
		board->clicked[row][column] = true;
		if (board->matrix[row][column] == 0) {
			makeMove(board, row-1, column-1);
		}
	}
	//Straight up
	if (checkInBounds(board->boardSize, row-1, column) && !board->clicked[row-1][column] && !board->clicked[row-1][column]) {
		board->clicked[row][column] = true;
		if (board->matrix[row][column] == 0) {
			makeMove(board, row-1, column);
		}
	}
	//Upper right
	if (checkInBounds(board->boardSize, row-1, column+1) && !board->clicked[row-1][column+1] && !board->clicked[row-1][column+1]) {
		board->clicked[row][column] = true;
		if (board->matrix[row][column] == 0) {
			makeMove(board, row-1, column+1);
		}
	}
	//Left
	if (checkInBounds(board->boardSize, row, column-1) && !board->clicked[row][column-1] && !board->clicked[row][column-1]) {
		board->clicked[row][column] = true;
		if (board->matrix[row][column] == 0) {
			makeMove(board, row, column-1);
		}
	}
	//Right
	if (checkInBounds(board->boardSize, row, column+1) && !board->clicked[row][column+1] && !board->clicked[row][column+1]) {
		board->clicked[row][column] = true;
		if (board->matrix[row][column] == 0) {
			makeMove(board, row, column+1);
		}
	}
	//Lower left
	if (checkInBounds(board->boardSize, row+1, column-1) && !board->clicked[row+1][column-1] && !board->clicked[row+1][column-1]) {
		board->clicked[row][column] = true;
		if (board->matrix[row][column] == 0) {
			makeMove(board, row+1, column-1);
		}
	}
	//Straight down
	if (checkInBounds(board->boardSize, row+1, column) && !board->clicked[row+1][column] && !board->clicked[row+1][column]) {
		board->clicked[row][column] = true;
		if (board->matrix[row][column] == 0) {
			makeMove(board, row+1, column);
		}
	}
	//Lower right
	if (checkInBounds(board->boardSize, row+1, column+1) && !board->clicked[row+1][column+1] && !board->clicked[row+1][column+1]) {
		board->clicked[row][column] = true;
		if (board->matrix[row][column] == 0) {
			makeMove(board, row+1, column+1);
		}
	}
}

bool Board::checkInBounds(int boardSize, int row, int column) {
	if (column >= 0 && column < boardSize && row >= 0 && row < boardSize) {
		return true;
	}
	return false;
}

void Board::cleanGame(Board::BoardType *board) {
	for (int i = 0; i < board->boardSize; i++) delete []board->matrix[i];
	delete []board->matrix;
	for (int i = 0; i < 2; i++) delete []board->clicked[i];
	delete []board->clicked;
}