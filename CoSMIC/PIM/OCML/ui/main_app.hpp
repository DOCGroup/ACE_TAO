#ifndef MAIN_APP_HPP
#define MAIN_APP_HPP

#include <wx/wx.h>

#include "main_frame.hpp"

class MainApp: public wxApp
{
public:
  MainApp();
  ~MainApp();

  virtual bool OnInit();
  void show_dialog(const char* tree_file,
                   const char* values, size_t values_size,
                   const char* rules, size_t rules_size,
                   MainFrameValueChangeListener* l);
};

#endif // MAIN_APP_HPP
