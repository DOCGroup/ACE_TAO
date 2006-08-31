// $Id$

#include "IntegerEditControl.hpp"

IntegerEditControl::IntegerEditControl(wxWindow* parent)
  : wxTextCtrl(parent, -1)
{
}

void
IntegerEditControl::add_focus_listener(IntegerEditControlFocusListener* l)
{
  focus_listeners_.push_back(l);
}

void
IntegerEditControl::
add_value_change_listener(IntegerEditControlValueChangeListener* l)
{
  value_change_listeners_.push_back(l);
}

void
IntegerEditControl::on_focus_gain(wxFocusEvent&)
{
  for (std::list<IntegerEditControlFocusListener*>::iterator
	 iter = focus_listeners_.begin();
       iter != focus_listeners_.end(); ++iter)
    (*iter)->integer_edit_focus_gain(this);
}    

void
IntegerEditControl::on_focus_lost(wxFocusEvent&)
{
  for (std::list<IntegerEditControlFocusListener*>::iterator
	 iter = focus_listeners_.begin();
       iter != focus_listeners_.end(); ++iter)
    (*iter)->integer_edit_focus_lost(this);
}    

void
IntegerEditControl::on_text_change(wxCommandEvent&)
{
  for (std::list<IntegerEditControlValueChangeListener*>::iterator
         iter = value_change_listeners_.begin();
       iter != value_change_listeners_.end(); ++iter)
    (*iter)->integer_edit_value_changed(this);
}

BEGIN_EVENT_TABLE(IntegerEditControl, wxTextCtrl)
  EVT_SET_FOCUS(IntegerEditControl::on_focus_gain)
  EVT_KILL_FOCUS(IntegerEditControl::on_focus_lost)
  EVT_TEXT(wxID_ANY, IntegerEditControl::on_text_change)
END_EVENT_TABLE()
