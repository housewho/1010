#include "my1010.h"
#include "Board.h"

My1010::My1010(const wxString& title)
       : wxFrame(NULL, wxID_ANY, title, wxDefaultPosition, wxSize(360, 1080)) {

//	sb = CreateStatusBar();
//	sb->SetStatusText(wxT("0"));
	board = new Board(this);
//	board->set_status_bar(sb);
	board->SetFocus();
	board->Start();
}

My1010::~My1010() {
	delete board;
	Destroy();
}



