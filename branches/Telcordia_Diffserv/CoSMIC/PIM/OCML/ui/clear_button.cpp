#include "clear_button.hpp"

Clear_Button::Clear_Button(wxWindow* parent)
  : wxButton(parent, -1, "Clear")
{
}

void
Clear_Button::add_click_listener(Clear_Button_Click_Listener* l)
{
  click_listeners_.push_back(l);
}

/**
 * Calls back the registered listeners when the GUI click event is received.
 */
void
Clear_Button::on_click(wxCommandEvent&)
{
  for (std::list<Clear_Button_Click_Listener*>::iterator
	 iter = click_listeners_.begin();
       iter != click_listeners_.end(); ++iter)
  {
    (*iter)->clear_button_clicked(this);
  }
}

BEGIN_EVENT_TABLE(Clear_Button, wxButton)
  EVT_BUTTON(wxID_ANY, Clear_Button::on_click)
END_EVENT_TABLE()
