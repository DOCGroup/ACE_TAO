#include "configurator_ui.hpp"
#include "main_window.hpp"

#include <gtkmm/main.h>

Main_Window::Main_Window()
  : m_button("Hello World")
{
  set_border_width(10);

  m_button.signal_clicked().connect(
      sigc::mem_fun(*this, &Main_Window::on_button_clicked));

  add(m_button);

  m_button.show();
}

Main_Window::~Main_Window()
{
}

void Main_Window::on_button_clicked()
{
  Gtk::Main::quit();
}

std::string Configurator_Dialog::show(const std::string&,
                                      const std::string&,
                                      const std::string&)
{
  Gtk::Main kit(0, 0); // no command line arguments

  Main_Window main_window;
  Gtk::Main::run(main_window);
  return "";
}
