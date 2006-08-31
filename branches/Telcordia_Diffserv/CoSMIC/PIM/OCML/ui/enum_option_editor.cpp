// $Id$

#include "enum_option_editor.hpp"

using namespace OCML;

Enum_Option_Editor::Enum_Option_Editor(wxWindow* parent, Enum_Option* enum_option)
  : Option_Editor(parent, enum_option)
{
  editor_ = new EnumEditControl(panel());
  editor_->disable_events();
  editor_->Show(true);
  editor_->add_focus_listener(this);
  editor_->add_select_listener(this);
  panel()->GetSizer()->Add(editor_, 1, 0, 
                           /*wxALL | wxADJUST_MINSIZE | wxEXPAND,*/ 2);

  std::string default_line;
  if (enum_option->has_default())
    {
      Enum_Option::item_iterator default_item = enum_option->get_default();
      default_line = std::string("default - ") + *default_item;
    }
  else
    {
      default_line = std::string("-");
    }
  editor_->Append(default_line.c_str());

  for (Enum_Option::item_iterator iter = enum_option->begin_items();
       iter != enum_option->end_items(); ++iter)
    {
      editor_->Append(iter->c_str(), (void*) 0);
    }

  if (option()->assigned())
    {
      editor_->SetValue(enum_option->get().c_str());
    }
  else
    {
      editor_->SetValue(default_line.c_str());
    }
    
  editor_->enable_events();
    
  panel()->GetSizer()->SetSizeHints(panel());
}

Enum_Option_Editor::~Enum_Option_Editor()
{
}    

void
Enum_Option_Editor::add_focus_listener(Enum_Option_Editor_Focus_Listener* l)
{
  focus_listeners_.push_back(l);
}
    
void
Enum_Option_Editor::enum_edit_focus_gain(EnumEditControl*)
{
  for (std::list<Enum_Option_Editor_Focus_Listener*>::iterator
	 iter = focus_listeners_.begin();
       iter != focus_listeners_.end(); ++iter)
    (*iter)->enum_editor_focused(this);
}

void
Enum_Option_Editor::enum_edit_focus_lost(EnumEditControl*)
{
  // Nothing to do here
}    

void
Enum_Option_Editor::enum_edit_selected(std::string value, void*)
{
  editor_->disable_events();
  if (editor_->GetSelection() == 0)
    {
      option()->clear();
      button()->Enable(false);
      editor_->SetSelection(0);
    }
  else
    {
      Enum_Option* enum_option = (Enum_Option*) option();
      enum_option->set(value.c_str());
      button()->Enable(true);
    }
  editor_->enable_events();
}

void
Enum_Option_Editor::clear_button_clicked(Clear_Button*)
{
  option()->clear();
  button()->Enable(false);
  editor_->SetSelection(0);
}
