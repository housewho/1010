#include <cstdlib>
#include <algorithm>
#include "Block.h"

using namespace std;

//Hard-coded facet colors for each type of square
const wxColour colors[Block_type_num]{
	wxColour(0xFF, 0xFF, 0xFF, 0xFF), wxColour(204, 102, 102, 0x00),
	wxColour(102, 204, 102, 0x00), wxColour(102, 102, 204, 0x00),
	wxColour(204, 204, 102, 0x00), wxColour(204, 102, 204, 0x00),
	wxColour(102, 204, 204, 0x00), wxColour(218, 170, 0, 0x00) };

//Hard-coded line colors for line in the top-left corner 
const wxColour light[Block_type_num]{
	wxColour(0xFF, 0xFF, 0xFF, 0xFF), wxColour(248, 159, 171),
	wxColour(121, 252, 121, 0x00), wxColour(121, 121, 252, 0x00),
	wxColour(252, 252, 121, 0x00), wxColour(252, 121, 252, 0x00),
	wxColour(121, 252, 252, 0x00), wxColour(252, 198, 0, 0x00) };

//Hard-coded line colors for line in the bottom-right corner 
const wxColour dark[Block_type_num]{
	wxColour(0xFF, 0xFF, 0xFF, 0xFF), wxColour(128, 59, 59),
	wxColour(59, 128, 59, 0x00), wxColour(59, 59, 128, 0x00),
	wxColour(128, 128, 59, 0x00), wxColour(128, 59, 128, 0x00),
	wxColour(59, 128, 128, 0x00), wxColour(128, 98, 0, 0x00) };



Block Block::random() {
	return Block_Type(
		std::min(
			Block_type_num - 1, 
			int((double(std::rand()) / RAND_MAX)*(Block_type_num - 1)) + 1
		)
	);
}

void Block::erase(Block& target) {
	target.pieceblock = no_block;
}

