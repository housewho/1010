#ifndef MY1010_H_INCLUDED
#define MY1010_H_INCLUDED
#include <wx/wx.h>

#include "Board.h"
class My1010 : public wxFrame
{
public:
    My1010(const wxString& title);
	~My1010();
	wxStatusBar* sb;
	Board* board;
};


#endif // MY1010_H_INCLUDED
