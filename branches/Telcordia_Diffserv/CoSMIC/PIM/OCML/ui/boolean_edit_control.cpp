#include "Boolean_Edit_Control.hpp"

Boolean_Edit_Control::Boolean_Edit_Control(wxWindow* parent)
  : wxCheckBox(parent, -1, "")
{
}

void
Boolean_Edit_Control::add_click_listener(Boolean_Edit_Control_Click_Listener* l)
{
  click_listeners_.push_back(l);
}

void
Boolean_Edit_Control::add_focus_listener(Boolean_Edit_Control_Focus_Listener* l)
{
  focus_listeners_.push_back(l);
}

void
Boolean_Edit_Control::on_click(wxCommandEvent&)
{
  for (std::list<Boolean_Edit_Control_Click_Listener*>::iterator
	 iter = click_listeners_.begin();
       iter != click_listeners_.end(); ++iter)
  {
    (*iter)->boolean_edit_clicked(this);
  }
}

void
Boolean_Edit_Control::on_focus_gain(wxFocusEvent&)
{
  for (std::list<Boolean_Edit_Control_Focus_Listener*>::iterator
	 iter = focus_listeners_.begin();
       iter != focus_listeners_.end(); ++iter)
  {
    (*iter)->boolean_edit_focus_gain(this);
  }
}    

void
Boolean_Edit_Control::on_focus_lost(wxFocusEvent&)
{
  for (std::list<Boolean_Edit_Control_Focus_Listener*>::iterator
	 iter = focus_listeners_.begin();
       iter != focus_listeners_.end(); ++iter)
  {
    (*iter)->boolean_edit_focus_lost(this);
  }
}    

BEGIN_EVENT_TABLE(Boolean_Edit_Control, wxCheckBox)
  EVT_CHECKBOX(wxID_ANY, Boolean_Edit_Control::on_click)
  EVT_SET_FOCUS(Boolean_Edit_Control::on_focus_gain)
  EVT_KILL_FOCUS(Boolean_Edit_Control::on_focus_lost)
END_EVENT_TABLE()
