#ifndef BOARD_H_INCLUDED
#define BOARD_H_INCLUDED

#include "Shape.h"
#include <wx/wx.h>


class Board : public wxPanel
{
public:
    Board(wxFrame *parent);
	~Board();
    void Start();
    void Pause();
	void set_status_bar(wxStatusBar* sb) {
		m_stsbar = sb;
	};
protected:
    void OnPaint(wxPaintEvent& event);
    void OnKeyDown(wxKeyEvent& event);
    void OnTimer(wxCommandEvent& event);

private:
    enum { BoardWidth = 6, BoardHeight = 21 };

    Block_Type & block_at(int x,int y){return board [x][y];}

    int square_width(){return GetClientSize().GetWidth() / BoardWidth;}
    int square_height(){return GetClientSize().GetHeight() / BoardHeight;}

    void ClearBoard();
    void PieceDropped();
    void DropDown();
    void OneLineDown();
    void generate_block();
    bool try_move(const Block& new_piece,int newX ,int newY);
    void DrawSquare(wxPaintDC &dc, int x_pos, int y_pos, const int x_size, int const y_size, Block_Type block);

    wxTimer *timer;

	bool isStarted{false};
	bool isPaused{false};
	bool isFallingFinished{false};
	int curX{0};
	int curY{0};
	int numLinesRemoved{0};

    Block cur_piece;
    Block_Type board[BoardWidth][BoardHeight];
    wxStatusBar* m_stsbar;

};

#endif // BOARD_H_INCLUDED
