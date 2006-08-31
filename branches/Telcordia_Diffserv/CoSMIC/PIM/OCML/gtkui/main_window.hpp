#ifndef OCML_MAIN_WINDOW_HPP
#define OCML_MAIN_WINDOW_HPP

#include <gtkmm/button.h>
#include <gtkmm/window.h>

class Main_Window : public Gtk::Window
{

public:
  Main_Window();
  virtual ~Main_Window();

protected:
  virtual void on_button_clicked();

  Gtk::Button m_button;
};

#endif //  OCML_MAIN_WINDOW_HPP

