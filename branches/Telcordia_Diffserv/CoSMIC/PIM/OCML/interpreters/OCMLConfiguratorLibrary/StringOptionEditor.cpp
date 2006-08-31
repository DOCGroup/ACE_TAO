// $Id$

#include "StringOptionEditor.hpp"

using namespace OCML;

StringOptionEditor::StringOptionEditor(wxWindow* parent,
                                       StringOption* string_option)
  : OptionEditor(parent, string_option),
    focused_(false),
    value_changed_(false)
{
  editor_ = new StringEditControl(panel());

  if (option()->assigned())
    {
      editor_->SetValue(string_option->get().c_str());
      button()->Enable(true);
    }
  else
    {
      editor_->SetValue(string_option->default_value().c_str());
    }

  editor_->Show(true);
  editor_->add_focus_listener(this);
  editor_->add_value_change_listener(this);
  panel()->GetSizer()->Add(editor_, 1, wxALL | wxADJUST_MINSIZE | wxEXPAND, 2);
    
  panel()->GetSizer()->SetSizeHints(panel());
}

StringOptionEditor::~StringOptionEditor()
{
}    

void
StringOptionEditor::add_focus_listener(StringOptionEditorFocusListener* l)
{
  focus_listeners_.push_back(l);
}
    
void
StringOptionEditor::string_edit_focus_gain(StringEditControl*)
{
  if (focused_)
    return;

  for (std::list<StringOptionEditorFocusListener*>::iterator
	 iter = focus_listeners_.begin();
       iter != focus_listeners_.end(); ++iter)
    (*iter)->string_editor_focused(this);

  focused_ = true;
  value_changed_ = false;
}

void
StringOptionEditor::string_edit_focus_lost(StringEditControl*)
{
  if (!focused_ || !value_changed_)
    return;

  StringOption* string_option = (StringOption*) option();

  string_option->set((const char*) editor_->GetValue());
  button()->Enable(true);

  focused_ = false;
}    

void
StringOptionEditor::string_edit_value_changed(StringEditControl*)
{
  value_changed_ = true;
}

void
StringOptionEditor::clear_button_clicked(ClearButton*)
{
  option()->clear();
  button()->Enable(false);

  StringOption* string_option = (StringOption*) option();
  editor_->SetValue(string_option->default_value().c_str());
}

void
StringOptionEditor::unfocus()
{
  string_edit_focus_lost(0);
}
