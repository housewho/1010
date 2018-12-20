#include "Board.h"

Board::Board(wxFrame *parent)
       : wxPanel(parent, wxID_ANY, wxDefaultPosition,wxDefaultSize, wxBORDER_NONE)
{
    timer = new wxTimer(this, 1);

    m_stsbar = parent->GetStatusBar();

    ClearBoard();

    Connect(wxEVT_PAINT, wxPaintEventHandler(Board::OnPaint));
    Connect(wxEVT_KEY_DOWN, wxKeyEventHandler(Board::OnKeyDown));
    Connect(wxEVT_TIMER, wxCommandEventHandler(Board::OnTimer));
}

Board::~Board() {
	delete timer;
}

void Board::Start() {
    if (isPaused)
        return;
	isStarted = true;
	isFallingFinished = false;
	numLinesRemoved = 0;
	ClearBoard();

	generate_block();
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
        timer->Start(3000);
        wxString str;
        str.Printf(wxT("%d"), numLinesRemoved);
        m_stsbar->SetStatusText(str);
    }
    Refresh();
}


void Board::OnPaint(wxPaintEvent& event) {
    wxPaintDC dc(this);
    wxSize size = GetClientSize();
	const int window_width = size.GetWidth();
	const int window_height = size.GetHeight();
	const int block_width = window_width / BoardWidth;
	const int block_height = window_height / BoardHeight;
    for (int i = 0; i < BoardHeight; ++i) {
        for (int j = 0; j < BoardWidth; ++j) {
            const Block_Type block = board[j] [BoardHeight - i - 1];
			DrawSquare(dc, j * block_width,
				i * block_height, block_width, block_height, block);
        }
    }

}

void Board::OnKeyDown(wxKeyEvent& event)
{
    if (!isStarted || cur_piece == no_block) {
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
    case WXK_LEFT:
        try_move(cur_piece, curX - 1, curY);
        break;
    case WXK_RIGHT:
        try_move(cur_piece, curX + 1, curY);
        break;
    case 'D':case 'd':case WXK_DOWN:
        OneLineDown();
        break;
    default:
        event.Skip();
    }

}

void Board::OnTimer(wxCommandEvent& event)
{
    if (isFallingFinished) {
        isFallingFinished = false;
        generate_block();
    } else {
        OneLineDown();
    }
}

void Board::ClearBoard()
{
    for (int i=0;i<BoardWidth;i++)
    {
        for (int j=0;j<BoardHeight-1;j++)
        {
            board[i][j]=no_block;
        }
    }
}

void Board::DropDown()
{
    int newY = curY;
    while (newY > 0) {
        if (!try_move(cur_piece, curX, newY - 1))
            {break;}
        --newY;
    }
    PieceDropped();
}

void Board::OneLineDown()
{
    if (!try_move(cur_piece, curX, curY - 1))
        PieceDropped();
}

void Board::PieceDropped()
{

        int x = curX ;
        int y = curY ;
       block_at(x, y) = cur_piece;


    //RemoveFullLines();

    if (!isFallingFinished)
        generate_block();
}

void Board::generate_block()
{
    for (int i=0;i<BoardWidth;i++)
    {
        Block::set_random_colour(cur_piece);
        board[i][BoardHeight-1]=cur_piece.pieceblock;
    }
}

bool Board::try_move(const Block& new_piece,int newX ,int newY)
{
    if (newX < 0 || newX >= BoardWidth || newY < 0 || newY >= BoardHeight-1)
            return false;
    if (block_at(newX, newY) != no_block)
            return false;

    cur_piece = new_piece;
    curX = newX;
    curY = newY;
    Refresh();
    return true;
}


//following draws the given rectangle with a single color fill
//and lines on the edge.
//Textures may be applied, and shader code may be used to optimize and add features here
void Board::DrawSquare(wxPaintDC& dc, int x_pos, int y_pos, const int x_size, const int y_size, Block_Type block) {

	//Hard-coded facet colors for each type of square
    static const wxColour colors[] = {
		wxColour(0xFF, 0xFF, 0xFF, 0xFF), wxColour(204, 102, 102, 0x00),
		wxColour(102, 204, 102, 0x00), wxColour(102, 102, 204, 0x00),
		wxColour(204, 204, 102, 0x00), wxColour(204, 102, 204, 0x00),
		wxColour(102, 204, 204, 0x00), wxColour(218, 170, 0, 0x00) };

	//Hard-coded line colors for line in the top-left corner 
    static const wxColour light[] = {
		wxColour(0xFF, 0xFF, 0xFF, 0xFF), wxColour(248, 159, 171),
		wxColour(121, 252, 121, 0x00), wxColour(121, 121, 252, 0x00),
		wxColour(252, 252, 121, 0x00), wxColour(252, 121, 252, 0x00),
		wxColour(121, 252, 252, 0x00), wxColour(252, 198, 0, 0x00) };

	//Hard-coded line colors for line in the bottom-right corner 
    static const wxColour dark[] = {
		wxColour(0xFF, 0xFF, 0xFF, 0xFF), wxColour(128, 59, 59),
		wxColour(59, 128, 59, 0x00), wxColour(59, 59, 128, 0x00),
		wxColour(128, 128, 59, 0x00), wxColour(128, 59, 128, 0x00),
		wxColour(59, 128, 128, 0x00), wxColour(128, 98, 0, 0x00) };

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
