#include "Board.h"

#include <sstream>

Board::Board(wxFrame *parent)
       : wxPanel(parent, wxID_ANY, wxDefaultPosition,wxDefaultSize, wxBORDER_NONE) {
    timer = new wxTimer(this, 1);

	m_stsbar = parent->CreateStatusBar();
	m_stsbar->SetStatusText(wxT("0"));

    init_board();

    Connect(wxEVT_PAINT, wxPaintEventHandler(Board::OnPaint));
    Connect(wxEVT_KEY_DOWN, wxKeyEventHandler(Board::OnKeyDown));
    Connect(wxEVT_TIMER, wxCommandEventHandler(Board::OnTimer));
	Connect(wxEVT_ERASE_BACKGROUND,wxEraseEventHandler(Board::OnErase));
	
}

Board::~Board() {
	delete timer;
}

void Board::Start() {
    if (isPaused)
        return;
	isStarted = true;
	score = 0;
	init_board();

	generate_line();
	timer->Start(0);
}

void Board::Pause()
{
    if (!isStarted)
        return;
    isPaused = !isPaused;
    if (isPaused) {
        timer->Stop();
        m_stsbar->SetStatusText(wxT("paused"));
    } else {
        timer->Start();
        wxString str;
        str.Printf(wxT("%d"), score);
        m_stsbar->SetStatusText(str);
    }
    Refresh();
}

void Board::renderFunc(wxDC& dc) {
	wxSize size = GetClientSize();
	const int window_width = size.GetWidth();
	const int window_height = size.GetHeight();
	wxBufferedDC bdc(&dc, size);
	const int block_width = window_width / BoardWidth;
	const int block_height = window_height / BoardHeight;
	for (int i = 0; i < BoardHeight; ++i) {
		for (int j = 0; j < BoardWidth; ++j) {
			const Block_Type block = board[BoardHeight - i - 1][j];
			DrawSquare(bdc, j * block_width,
				i * block_height, block_width, block_height, block);
		}
	}
}

void Board::OnPaint(wxPaintEvent& event) {
    wxPaintDC dc(this);
	renderFunc(dc);
}

void Board::OnKeyDown(wxKeyEvent& event) {
    if (!isStarted||isFinished) {
        event.Skip();
        return;
    }
    int keycode = event.GetKeyCode();

    if (keycode == 'p' || keycode == 'P') {
		Pause();
        return;
    }
    if (isPaused)
        return;

    switch (keycode) {
		case 'A':	case 'a':	case WXK_LEFT:
			move_left();
			break;
		case 'D':	case 'd':	case WXK_RIGHT:
			move_right();
			break;
		case 'S':	case 's':	case WXK_DOWN:
			drop();
			break;
		default:
			event.Skip();
    }
	Refresh();
}

void Board::OnTimer(wxCommandEvent& event) {

}

void Board::init_board() {
	for (int i=0;i<BoardWidth;i++) {
		for (int j=0;j<BoardHeight;j++) {
			board[j][i]=no_block;
		}
	}
	Board::generate_line();
}

void Board::generate_line() {
	for (int i = 0; i < BoardWidth; i++) {
		board[BoardHeight - 1][i] = Block::random();
	}
}

//This function is called when the user calls the move_left function
Board& Board::move_left() {
	Block temp = board[BoardHeight - 1][0];
	for (int i = 0; i < BoardWidth - 1; i++) {
		board[BoardHeight - 1][i] = board[BoardHeight - 1][i + 1];
	}
	board[BoardHeight - 1][BoardWidth - 1] = temp;
	Refresh();
	return *this;
}

//This function is called when the user calls the move_left function
Board& Board::move_right() {
	Block temp = board[BoardHeight - 1][BoardWidth - 1];
	for (int i = BoardWidth - 1; i > 0; i--) {
		board[BoardHeight - 1][i] = board[BoardHeight - 1][i - 1];
	}
	board[BoardHeight - 1][0] = temp;
	Refresh();
	return *this;
}

//This function proceeds the drop command and calculates the results
//Animated drop may be added here with the timer implemented
Board& Board::drop() {
	for (int i = 0; i < BoardWidth; i++) {
		if (board[BoardHeight-2][i]) {
			isFinished = true;
			return *this;
		}
	}
	for (int i = 0; i < BoardWidth; i++) {
		int j = BoardHeight - 2;
		while (j >= 0 && !board[j][i]) j--;
		std::swap(board[BoardHeight - 1][i], board[j + 1][i]);
	}
	Refresh();
	erase();
	Refresh();
	generate_line();
	Refresh();
	return *this;
}

void Board::find_set(
	const Board::coords start,
	std::vector<coords>& found,
	std::array<std::array<bool, BoardWidth>, BoardHeight - 1>& buffer) {
	if (start.x > 0
		&& board[start.x][start.y] == board[start.x - 1][start.y]
		&& !buffer[start.x - 1][start.y]) {
		const coords new_coord = { start.x - 1,start.y };
		buffer[new_coord.x][new_coord.y] = true;
		found.push_back(new_coord);
		find_set(new_coord, found, buffer);
	}
	if (start.x+1 < BoardHeight-1
		&& board[start.x][start.y] == board[start.x + 1][start.y] 
		&& !buffer[start.x + 1][start.y]) {
		const coords new_coord = { start.x + 1,start.y };
		buffer[new_coord.x][new_coord.y] = true;
		found.push_back(new_coord);
		find_set(new_coord, found, buffer);
	}
	if (start.y > 0
		&& board[start.x][start.y] == board[start.x][start.y - 1]
		&& !buffer[start.x][start.y - 1]) {
		const coords new_coord = { start.x, start.y - 1 };
		buffer[new_coord.x][new_coord.y] = true;
		found.push_back(new_coord);
		find_set(new_coord, found, buffer);
	}
	if (start.y + 1 < BoardWidth
		&& board[start.x][start.y] == board[start.x][start.y + 1]
		&& !buffer[start.x][start.y + 1]) {
		const coords new_coord = { start.x, start.y + 1 };
		buffer[new_coord.x][new_coord.y] = true;
		found.push_back(new_coord);
		find_set(new_coord, found, buffer);
	}
}

//This function searches for sections with more than 3 same blocks
//and then erase them, let other blocks drop, and recurse
//returns true if erased at lease one set and requires more erase
//Another animation of erasing and dropping may be added here 
//with the timer implemented
Board& Board::erase() {
	bool erased = false;
	std::array<std::array<bool, BoardWidth>, BoardHeight - 1> passed{};
	for (int i = 0; i < BoardHeight - 1; i++) {
		for (int j = 0; j < BoardWidth; j++) {
			if (!(passed[i][j])) {
				passed[i][j] = true;
				if (board[i][j]) {
					std::vector<coords> found;
					found.push_back({ i,j });
					find_set({ i,j }, found, passed);
					int size = found.size();
					if (size >= MAX_SAME_BLOCK) {
						erased = true;
						for (int n = 0; n < size; n++) {
							Block::erase(board[found[n].x][found[n].y]);
							score++;
						}
						std::ostringstream oss;
						oss << score;
						m_stsbar->SetStatusText(oss.str());
					}
				}
			}
		}
	}
	if (erased) { 
		drop_all();
		erase();
	}
	return *this;
}

//Drops all of the blocks.
//Can implement some effects here
void Board::drop_all() {
	for (int j = 0; j < BoardWidth; j++) {
		bool changed = true;
		int start = 0;
		while (changed) {
			changed = false;
			for (int i = start; i < BoardHeight - 1; i++) {
				if (board[i + 1][j] && !board[i][j]) {
					changed = true;
					std::swap(board[i][j], board[i + 1][j]);
				}
				if (board[start][j]) start++;
			}
		}
	}
}

//following draws the given rectangle with a single color fill
//and lines on the edge.
//Textures may be applied, and shader code may be used to optimize and add features here
void Board::DrawSquare(wxBufferedDC& dc, int x_pos, int y_pos, const int x_size, const int y_size, Block_Type block) {

	//Draws the facet
	dc.SetPen(*wxTRANSPARENT_PEN);
	dc.SetBrush(wxBrush(colors[int(block)]));
	dc.DrawRectangle(x_pos, y_pos, x_size, y_size);

	//Draws the top-left corner
    wxPen pen(light[int(block)]);
    pen.SetCap(wxCAP_PROJECTING);
    dc.SetPen(pen);

    dc.DrawLine(x_pos, y_pos + y_size - 1, x_pos, y_pos);
    dc.DrawLine(x_pos, y_pos, x_pos + x_size - 1, y_pos);

	//move the cordinates for easier drawing
	x_pos += x_size;
	y_pos += y_size;

	//Draws the bottom-right corner
    wxPen darkpen(dark[int(block)]);
    darkpen.SetCap(wxCAP_PROJECTING);
    dc.SetPen(darkpen);

	dc.DrawLine(x_pos - 1, y_pos - 1, x_pos - 1, y_pos - y_size + 1);
	dc.DrawLine(x_pos - x_size + 1, y_pos - 1, x_pos - 1, y_pos - 1);
}
