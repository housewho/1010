#ifndef SHAPE_H_INCLUDED
#define SHAPE_H_INCLUDED

#define Block_type_num 8
enum Block_Type { no_block, red, orange, yellow, green, light_blue, dark_blue, purple };
class Block {
public :
	Block() { pieceblock = no_block; }

    static void set_random_colour(Block& target);
	static void erase_block(Block& target);

	operator Block_Type() { return pieceblock; }

    Block_Type pieceblock;

	//move operations
};

#endif // SHAPE_H_INCLUDED
