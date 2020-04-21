#include "board.hpp"
#include <iostream>

#include <SFML/Graphics.hpp>

int main(int argc, char** argv) {
	Board::BoardType board;
	Board minesweeper;

	minesweeper.initBoard(&board);
	//minesweeper.debugBoard(&board); //For testing purposes only
	sf::RenderWindow window(sf::VideoMode(790, 880), "Minesweeper", sf::Style::Titlebar | sf::Style::Close);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }

            if (event.type == sf::Event::MouseButtonPressed) {
            	if (event.mouseButton.button == sf::Mouse::Left) {
            		minesweeper.bombClicked(&board, event.mouseButton.x, event.mouseButton.y);
            		if (minesweeper.checkWinState(&board)) {
            			std::cout << "Congratulations! You win! Your time was " << board.time;
            			exit(0);
            		}
        		}
        		if (event.mouseButton.button == sf::Mouse::Right) {
            		minesweeper.flagPlaced(&board, event.mouseButton.x, event.mouseButton.y);
        		}
            }
		}

		window.clear(sf::Color(181, 181, 181));
		window.draw(board.leftBorder);
		window.draw(board.rightBorder);
		window.draw(board.upperBorder);
		window.draw(board.lowerBorder);
		window.draw(board.menuBorder);
		window.draw(board.bombCounter);
		window.draw(board.timer);
		
        for (int i=0;i<board.boardSize;i++) {
			for (int j=0;j<board.boardSize;j++) {
				window.draw(board.spots[i][j]);
				if (board.clicked[i][j]) {
					board.spots[i][j].setFillColor(sf::Color::Transparent);
					window.draw(board.numbers[i][j]);
				}
			}
		}
        window.display();
    }
    minesweeper.cleanGame(&board);

	return 0;
}



