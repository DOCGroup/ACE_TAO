#include "main_app.hpp"

IMPLEMENT_APP_NO_MAIN(MainApp)

MainApp::MainApp()
{
}

bool
MainApp::OnInit()
{
  return true;
}

void
MainApp::show_dialog(const char* tree_file,
                     const char* values, size_t values_size,
                     const char* rules, size_t rules_size,
                     MainFrameValueChangeListener* l)
{
  MainFrame *win = new MainFrame("Frame", wxPoint (100, 100),
                                 wxSize(500, 500),
                                 tree_file,
                                 rules, rules_size,
                                 values, values_size);
  win->add_value_change_listener(l);
  win->Show(TRUE);
  wxGetApp().MainLoop();
}

MainApp::~MainApp()
{
}
