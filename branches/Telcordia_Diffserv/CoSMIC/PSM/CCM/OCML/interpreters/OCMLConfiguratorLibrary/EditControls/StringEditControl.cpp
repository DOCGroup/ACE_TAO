// $Id$

#include "StringEditControl.hpp"

StringEditControl::StringEditControl(wxWindow* parent)
  : wxTextCtrl(parent, -1, "", wxPoint(0, 0), wxSize(100, 40))
{
}

void
StringEditControl::add_focus_listener(StringEditControlFocusListener* l)
{
  focus_listeners_.push_back(l);
}

void
StringEditControl::
add_value_change_listener(StringEditControlValueChangeListener* l)
{
  value_change_listeners_.push_back(l);
}

void
StringEditControl::on_focus_gain(wxFocusEvent&)
{
  for (std::list<StringEditControlFocusListener*>::iterator
	       iter = focus_listeners_.begin();
       iter != focus_listeners_.end(); ++iter)
    (*iter)->string_edit_focus_gain(this);
}

void
StringEditControl::on_focus_lost(wxFocusEvent&)
{
  for (std::list<StringEditControlFocusListener*>::iterator
	       iter = focus_listeners_.begin();
       iter != focus_listeners_.end(); ++iter)
    (*iter)->string_edit_focus_lost(this);
}

void
StringEditControl::on_text_change(wxCommandEvent&)
{
  for (std::list<StringEditControlValueChangeListener*>::iterator
         iter = value_change_listeners_.begin();
       iter != value_change_listeners_.end(); ++iter)
    (*iter)->string_edit_value_changed(this);
}

BEGIN_EVENT_TABLE(StringEditControl, wxTextCtrl)
  EVT_SET_FOCUS(StringEditControl::on_focus_gain)
  EVT_KILL_FOCUS(StringEditControl::on_focus_lost)
  EVT_TEXT(wxID_ANY, StringEditControl::on_text_change)
END_EVENT_TABLE()
