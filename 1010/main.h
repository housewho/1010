#ifndef MAIN_H_INCLUDED
#define MAIN_H_INCLUDED

#include <wx/wx.h>

#include "my1010.h"

class MyApp : public wxApp
{
  public:
    virtual bool OnInit() override;
	virtual int OnExit() override;
	My1010* game;
};

#endif // MAIN_H_INCLUDED
