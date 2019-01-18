#ifndef SHAPE_H_INCLUDED
#define SHAPE_H_INCLUDED

#define Block_type_num 8

#include <wx/wx.h>

enum Block_Type { no_block, red, orange, yellow, green, light_blue, dark_blue, purple };

//Hard-coded facet colors for each type of square
extern const wxColour colors[Block_type_num];

//Hard-coded line colors for line in the top-left corner 
extern const wxColour light[Block_type_num];

//Hard-coded line colors for line in the bottom-right corner 
extern const wxColour dark[Block_type_num];


class Block {
public :
	Block() = default;
	Block(const Block_Type& type) : pieceblock(type) {};
    static Block random();
	static void erase(Block& target);

	operator Block_Type() { return pieceblock; }

	Block_Type pieceblock{ no_block };

	//move operations
};

#endif // SHAPE_H_INCLUDED
