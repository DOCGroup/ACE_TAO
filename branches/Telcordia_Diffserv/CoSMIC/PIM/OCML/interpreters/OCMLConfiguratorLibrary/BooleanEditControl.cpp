#include "BooleanEditControl.hpp"

BooleanEditControl::BooleanEditControl(wxWindow* parent)
  : wxCheckBox(parent, -1, "")
{
}

void
BooleanEditControl::add_click_listener(BooleanEditControlClickListener* l)
{
  click_listeners_.push_back(l);
}

void
BooleanEditControl::add_focus_listener(BooleanEditControlFocusListener* l)
{
  focus_listeners_.push_back(l);
}

void
BooleanEditControl::on_click(wxCommandEvent&)
{
  for (std::list<BooleanEditControlClickListener*>::iterator
	 iter = click_listeners_.begin();
       iter != click_listeners_.end(); ++iter)
    (*iter)->boolean_edit_clicked(this);
}

void
BooleanEditControl::on_focus_gain(wxFocusEvent&)
{
  for (std::list<BooleanEditControlFocusListener*>::iterator
	 iter = focus_listeners_.begin();
       iter != focus_listeners_.end(); ++iter)
    (*iter)->boolean_edit_focus_gain(this);
}    

void
BooleanEditControl::on_focus_lost(wxFocusEvent&)
{
  for (std::list<BooleanEditControlFocusListener*>::iterator
	 iter = focus_listeners_.begin();
       iter != focus_listeners_.end(); ++iter)
    (*iter)->boolean_edit_focus_lost(this);
}    

BEGIN_EVENT_TABLE(BooleanEditControl, wxCheckBox)
  EVT_CHECKBOX(wxID_ANY, BooleanEditControl::on_click)
  EVT_SET_FOCUS(BooleanEditControl::on_focus_gain)
  EVT_KILL_FOCUS(BooleanEditControl::on_focus_lost)
END_EVENT_TABLE()
