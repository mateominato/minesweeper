#ifndef BLOCK_H
#define BLOCK_H

class Block {
public: // Initialize the board with the user-provided input
	Block();
	struct BlockType {
        double xpos;
        double ypos;
		sf::Color color;

        block.setSize(sf::Vector2f(width, height));
        block.setFillColor(color);
        block.setPosition(position);
	}
};

#endif /* BLOCK_H */