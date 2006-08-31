// $Id$

#include "string_edit_control.hpp"

String_Edit_Control::String_Edit_Control(wxWindow* parent)
  : wxTextCtrl(parent, -1, "", wxPoint(0, 0), wxSize(100, 40))
{
}

void
String_Edit_Control::add_focus_listener(String_Edit_Control_Focus_Listener* l)
{
  focus_listeners_.push_back(l);
}

void
String_Edit_Control::
add_value_change_listener(String_Edit_Control_Value_Change_Listener* l)
{
  value_change_listeners_.push_back(l);
}

void
String_Edit_Control::on_focus_gain(wxFocusEvent&)
{
  for (std::list<String_Edit_Control_Focus_Listener*>::iterator
	       iter = focus_listeners_.begin();
       iter != focus_listeners_.end(); ++iter)
    (*iter)->string_edit_focus_gain(this);
}

void
String_Edit_Control::on_focus_lost(wxFocusEvent&)
{
  for (std::list<String_Edit_Control_Focus_Listener*>::iterator
	       iter = focus_listeners_.begin();
       iter != focus_listeners_.end(); ++iter)
    (*iter)->string_edit_focus_lost(this);
}

void
String_Edit_Control::on_text_change(wxCommandEvent&)
{
  for (std::list<String_Edit_Control_Value_Change_Listener*>::iterator
         iter = value_change_listeners_.begin();
       iter != value_change_listeners_.end(); ++iter)
    (*iter)->string_edit_value_changed(this);
}

BEGIN_EVENT_TABLE(String_Edit_Control, wxTextCtrl)
  EVT_SET_FOCUS(String_Edit_Control::on_focus_gain)
  EVT_KILL_FOCUS(String_Edit_Control::on_focus_lost)
  EVT_TEXT(wxID_ANY, String_Edit_Control::on_text_change)
END_EVENT_TABLE()
