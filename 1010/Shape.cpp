#include <cstdlib>
#include <algorithm>
#include "Shape.h"

using namespace std;

void Block::set_random_colour(Block& target) {
	int x = std::min(Block_type_num - 1, int((double(std::rand()) / RAND_MAX)*(Block_type_num - 1)) + 1);
	target.pieceblock = Block_Type(x);
}

void Block::erase_block(Block& target) {
	target.pieceblock = no_block;
}

