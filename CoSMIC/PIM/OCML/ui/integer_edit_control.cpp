// $Id$

#include "integer_edit_control.hpp"

Integer_Edit_Control::Integer_Edit_Control(wxWindow* parent)
  : wxTextCtrl(parent, -1)
{
}

void
Integer_Edit_Control::add_focus_listener(Integer_Edit_Control_Focus_Listener* l)
{
  focus_listeners_.push_back(l);
}

void
Integer_Edit_Control::
add_value_change_listener(Integer_Edit_Control_Value_Change_Listener* l)
{
  value_change_listeners_.push_back(l);
}

void
Integer_Edit_Control::on_focus_gain(wxFocusEvent&)
{
  for (std::list<Integer_Edit_Control_Focus_Listener*>::iterator
	 iter = focus_listeners_.begin();
       iter != focus_listeners_.end(); ++iter)
    (*iter)->integer_edit_focus_gain(this);
}    

void
Integer_Edit_Control::on_focus_lost(wxFocusEvent&)
{
  for (std::list<Integer_Edit_Control_Focus_Listener*>::iterator
	 iter = focus_listeners_.begin();
       iter != focus_listeners_.end(); ++iter)
    (*iter)->integer_edit_focus_lost(this);
}    

void
Integer_Edit_Control::on_text_change(wxCommandEvent&)
{
  for (std::list<Integer_Edit_Control_Value_Change_Listener*>::iterator
         iter = value_change_listeners_.begin();
       iter != value_change_listeners_.end(); ++iter)
    (*iter)->integer_edit_value_changed(this);
}

BEGIN_EVENT_TABLE(Integer_Edit_Control, wxTextCtrl)
  EVT_SET_FOCUS(Integer_Edit_Control::on_focus_gain)
  EVT_KILL_FOCUS(Integer_Edit_Control::on_focus_lost)
  EVT_TEXT(wxID_ANY, Integer_Edit_Control::on_text_change)
END_EVENT_TABLE()
