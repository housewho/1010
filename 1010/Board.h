#ifndef BOARD_H_INCLUDED
#define BOARD_H_INCLUDED

#include <wx/wx.h>
#include <wx/dcbuffer.h>
#include "Block.h"

#include <vector>
#include <array>

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
	void OnErase(wxEraseEvent& event) {};

private:
	int					score{ 0 };
	static const int	BoardWidth = 6, 
						BoardHeight = 21;
	Block board[BoardHeight][BoardWidth];

	wxStatusBar* m_stsbar;
	wxTimer *timer;
	bool isStarted{ false };
	bool isPaused{ false };
	bool isFinished{ false };

	struct coords {
		int x, y;
	};

    void init_board();
	void generate_line();

	Board& move_left();
	Board& move_right();
	Board& drop();

#define MAX_SAME_BLOCK 3
	Board& erase();
	void drop_all();
	void find_set(const coords start, std::vector<coords>& found, std::array<std::array<bool, BoardWidth>, BoardHeight - 1>& buffer);

	void renderFunc(wxDC& dc);
	void DrawSquare(wxBufferedDC &dc, int x_pos, int y_pos, const int x_size, int const y_size, Block_Type block);
};



#endif // BOARD_H_INCLUDED
