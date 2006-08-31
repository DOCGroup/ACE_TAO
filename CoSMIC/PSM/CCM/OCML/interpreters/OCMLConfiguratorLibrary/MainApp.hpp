#ifndef MAIN_HPP
#define MAIN_HPP

#include <wx/wx.h>

#include "MainFrame.hpp"

#include "extern_declarations.hpp"

class OCMLConfigurator_API MainApp: public wxApp
{
public:
  MainApp();
  ~MainApp();

  virtual bool OnInit();
  void new_instance(const char* values, size_t values_size,
                    MainFrameValueChangeListener* l,
                    const char* tree_file);
    
  OCML::OptionCategory* initialize_options();

private:
  OCML::OptionEditorPanel* panel;
  wxTextCtrl* console;
};

#endif // MAIN_HPP
