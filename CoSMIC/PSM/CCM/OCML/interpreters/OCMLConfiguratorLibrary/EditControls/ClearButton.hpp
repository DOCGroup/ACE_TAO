#ifndef CLEAR_BUTTON_HPP
#define CLEAR_BUTTON_HPP

#include <list>
#include <wx/wx.h>
#include "../extern_declarations.hpp"

class ClearButton;

class OCMLConfigurator_API ClearButtonClickListener
{
public:
  virtual void clear_button_clicked(ClearButton* control) = 0;
};
    
class OCMLConfigurator_API ClearButton: public wxButton
{
public:
  void on_click(wxCommandEvent&);

public:
  ClearButton(wxWindow* parent);
    
  void add_click_listener(ClearButtonClickListener* l);
    
private:
  std::list<ClearButtonClickListener*> click_listeners_;
    
  DECLARE_EVENT_TABLE()
};    

#endif // CLEAR_BUTTON_HPP
