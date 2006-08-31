// $Id$

#include "EnumOptionEditor.hpp"

using namespace OCML;

EnumOptionEditor::EnumOptionEditor(wxWindow* parent, EnumOption* enum_option)
  : OptionEditor(parent, enum_option)
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
      EnumOption::item_iterator default_item = enum_option->get_default();
      default_line = std::string("default - ") + *default_item;
    }
  else
    {
      default_line = std::string("-");
    }
  editor_->Append(default_line.c_str());

  for (EnumOption::item_iterator iter = enum_option->begin_items();
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

EnumOptionEditor::~EnumOptionEditor()
{
}    

void
EnumOptionEditor::add_focus_listener(EnumOptionEditorFocusListener* l)
{
  focus_listeners_.push_back(l);
}
    
void
EnumOptionEditor::enum_edit_focus_gain(EnumEditControl*)
{
  for (std::list<EnumOptionEditorFocusListener*>::iterator
	 iter = focus_listeners_.begin();
       iter != focus_listeners_.end(); ++iter)
    (*iter)->enum_editor_focused(this);
}

void
EnumOptionEditor::enum_edit_focus_lost(EnumEditControl*)
{
  // Nothing to do here
}    

void
EnumOptionEditor::enum_edit_selected(std::string value, void*)
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
      EnumOption* enum_option = (EnumOption*) option();
      enum_option->set(value.c_str());
      button()->Enable(true);
    }
  editor_->enable_events();
}

void
EnumOptionEditor::clear_button_clicked(ClearButton*)
{
  option()->clear();
  button()->Enable(false);
  editor_->SetSelection(0);
}
