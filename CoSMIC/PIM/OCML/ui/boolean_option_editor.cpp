// $Id$

#include "boolean_option_editor.hpp"

using namespace OCML;

Boolean_Option_Editor::Boolean_Option_Editor(wxWindow* parent,
                                             Option_Interface* boolean_option)
  : Option_Editor(parent, boolean_option)
{
  editor_ = new Boolean_Edit_Control(panel());

  if (option()->assigned())
    {
      editor_->SetValue(Boolean_Option_Trait::parse_string(boolean_option->str_value().c_str()));
      button()->Enable(true);
    }
  else
    {
      Basic_Option_Definition<Boolean_Option_Trait>* definition =
        dynamic_cast<Basic_Option_Definition<Boolean_Option_Trait>*> (boolean_option->definition());
      editor_->SetValue(definition->default_value());
    }

  editor_->Show(true);
  editor_->add_focus_listener(this);
  editor_->add_click_listener(this);
  panel()->GetSizer()->Add(editor_, 1, wxALL | wxADJUST_MINSIZE | wxEXPAND, 2);

  panel()->GetSizer()->SetSizeHints(panel());
}

Boolean_Option_Editor::~Boolean_Option_Editor()
{
}    

void
Boolean_Option_Editor::add_focus_listener(Boolean_Option_Editor_Focus_Listener* l)
{
  focus_listeners_.push_back(l);
}
    
void
Boolean_Option_Editor::boolean_edit_focus_gain(Boolean_Edit_Control*)
{
  for (std::list<Boolean_Option_Editor_Focus_Listener*>::iterator
	 iter = focus_listeners_.begin();
       iter != focus_listeners_.end(); ++iter)
    (*iter)->boolean_editor_focused(this);
}

void
Boolean_Option_Editor::boolean_edit_focus_lost(Boolean_Edit_Control*)
{
  // Nothing to do here
}

void
Boolean_Option_Editor::boolean_edit_clicked(Boolean_Edit_Control*)
{
  option()->str_value(Boolean_Option_Trait::as_string(editor_->GetValue()).c_str());

  const Boolean_Option_Value* basic_value =
    dynamic_cast<const Boolean_Option_Value*>(option()->value());
  if (editor_->GetValue() != basic_value->value())
    editor_->SetValue(basic_value->value());
}

void
Boolean_Option_Editor::value_changed(bool flagged, Option_Value* value)
{
  Boolean_Option_Value* basic_value = dynamic_cast<Boolean_Option_Value*>(value);
  Boolean_Option_Definition* definition =
    dynamic_cast<Boolean_Option_Definition*> (option()->definition());
  Boolean_Option_Trait::value_type default_value = definition->default_value();

  if (!value || value->is_null() || (basic_value->value() == default_value))
  {
    editor_->SetValue(default_value);
    button()->Enable(false);
  }
  else
  {
    editor_->SetValue(basic_value->value());
    button()->Enable(true);
  }
}

