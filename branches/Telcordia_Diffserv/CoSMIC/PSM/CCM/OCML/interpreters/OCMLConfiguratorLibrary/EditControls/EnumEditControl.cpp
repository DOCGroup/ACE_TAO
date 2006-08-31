#include "EnumEditControl.hpp"

#include "../../Common/Debug.hpp"

EnumEditControl::EnumEditControl(wxWindow* parent)
  : wxComboBox(parent, -1, "", wxPoint(0, 0), wxSize(100, 40),
	       0, NULL, wxCB_READONLY),
    events_enabled_(true)
{
}

void
EnumEditControl::add_select_listener(EnumEditControlSelectListener* l)
{
  select_listeners_.push_back(l);
}

void
EnumEditControl::add_focus_listener(EnumEditControlFocusListener* l)
{
  focus_listeners_.push_back(l);
}

void
EnumEditControl::on_select(wxCommandEvent&)
{
  if (events_enabled_)
    {
      std::string selection;
      void* client_data = NULL;
      
      int selection_index = GetSelection();
      if (selection_index != -1) // -1 means there is no selection
        {
          selection = (const char*) GetString(selection_index);
          client_data = GetClientData(selection_index);
        }
      else
        {
          // TODO: See if the user can select NONE
        }
      
      for (std::list<EnumEditControlSelectListener*>::iterator
	     iter=select_listeners_.begin();
           iter != select_listeners_.end(); ++iter)
        (*iter)->enum_edit_selected(selection, client_data);
    }
}

void
EnumEditControl::on_focus_gain(wxFocusEvent&)
{
  if (events_enabled_)
    for (std::list<EnumEditControlFocusListener*>::iterator
	   iter = focus_listeners_.begin();
         iter != focus_listeners_.end(); ++iter)
      (*iter)->enum_edit_focus_gain(this);
}    

void
EnumEditControl::on_focus_lost(wxFocusEvent&)
{
  if (events_enabled_)
    for (std::list<EnumEditControlFocusListener*>::iterator
	   iter = focus_listeners_.begin();
         iter != focus_listeners_.end(); ++iter)
      (*iter)->enum_edit_focus_lost(this);
}    

void
EnumEditControl::disable_events()
{
  events_enabled_ = false;
}

void
EnumEditControl::enable_events()
{
  events_enabled_ = true;
}

BEGIN_EVENT_TABLE(EnumEditControl, wxComboBox)
  EVT_TEXT(wxID_ANY, EnumEditControl::on_select)
  //  EVT_SET_FOCUS(EnumEditControl::on_focus_gain)
  //  EVT_KILL_FOCUS(EnumEditControl::on_focus_lost)
END_EVENT_TABLE()
