// $Id$

#include "string_option_editor.hpp"

using namespace OCML;

String_Option_Editor::String_Option_Editor(wxWindow* parent,
                                           Option_Interface* string_option)
  : Option_Editor(parent, string_option),
    focused_(false),
    value_changed_(false)
{
  editor_ = new String_Edit_Control(panel());

  if (option()->assigned())
    {
      editor_->SetValue(string_option->str_value().c_str());
      button()->Enable(true);
    }
  else
    {
      Basic_Option_Definition<String_Option_Trait>* definition =
        dynamic_cast<Basic_Option_Definition<String_Option_Trait>*> (string_option->definition());
      editor_->SetValue(definition->default_value().c_str());
    }

  editor_->Show(true);
  editor_->add_focus_listener(this);
  editor_->add_value_change_listener(this);
  panel()->GetSizer()->Add(editor_, 1, wxALL | wxADJUST_MINSIZE | wxEXPAND, 2);
    
  panel()->GetSizer()->SetSizeHints(panel());
}

String_Option_Editor::~String_Option_Editor()
{
}    

void
String_Option_Editor::add_focus_listener(String_Option_Editor_Focus_Listener* l)
{
  focus_listeners_.push_back(l);
}
    
void
String_Option_Editor::string_edit_focus_gain(String_Edit_Control*)
{
  if (focused_)
    return;

  for (std::list<String_Option_Editor_Focus_Listener*>::iterator
	 iter = focus_listeners_.begin();
       iter != focus_listeners_.end(); ++iter)
    (*iter)->string_editor_focused(this);

  focused_ = true;
  value_changed_ = false;
}

void
String_Option_Editor::string_edit_focus_lost(String_Edit_Control*)
{
  if (!focused_ || !value_changed_)
    return;

  option()->str_value((const char*) editor_->GetValue());

  const String_Option_Value* basic_value =
    dynamic_cast<const String_Option_Value*>(option()->value());
  if (editor_->GetValue() != basic_value->value().c_str())
    editor_->SetValue(basic_value->value().c_str());

  focused_ = false;
}    

void
String_Option_Editor::string_edit_value_changed(String_Edit_Control*)
{
  value_changed_ = true;
}

void
String_Option_Editor::unfocus()
{
  string_edit_focus_lost(0);
}

void
String_Option_Editor::value_changed(bool flagged, Option_Value* value)
{
  String_Option_Value* basic_value = dynamic_cast<String_Option_Value*>(value);
  String_Option_Definition* definition =
    dynamic_cast<String_Option_Definition*> (option()->definition());
  String_Option_Trait::value_type default_value = definition->default_value();

  if (!value || value->is_null() || (basic_value->value() == default_value))
  {
    editor_->SetValue(default_value.c_str());
    button()->Enable(false);
  }
  else
  {
    editor_->SetValue(basic_value->value().c_str());
    button()->Enable(true);
  }
}

