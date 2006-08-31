#include "ClearButton.hpp"

ClearButton::ClearButton(wxWindow* parent)
  : wxButton(parent, -1, "Clear")
{
}

void
ClearButton::add_click_listener(ClearButtonClickListener* l)
{
  click_listeners_.push_back(l);
}

/**
 * Calls back the registered listeners when the GUI click event is received.
 */
void
ClearButton::on_click(wxCommandEvent&)
{
  for (std::list<ClearButtonClickListener*>::iterator
	 iter = click_listeners_.begin();
       iter != click_listeners_.end(); ++iter)
    (*iter)->clear_button_clicked(this);
}

BEGIN_EVENT_TABLE(ClearButton, wxButton)
  EVT_BUTTON(wxID_ANY, ClearButton::on_click)
END_EVENT_TABLE()
