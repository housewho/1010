#include "main.h"
#include "my1010.h"
#include <ctime>

IMPLEMENT_APP(MyApp)


bool MyApp::OnInit() {
    std::srand(std::time(NULL));
	std::rand();
    game = new My1010(wxT("my1010"));
    game->Centre();
    game->Show(true);
    return true;
}

int MyApp::OnExit() {
	return 0;
}
