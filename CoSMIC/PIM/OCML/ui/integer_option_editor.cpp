// $Id$

#include "integer_option_editor.hpp"
#include <boost/lexical_cast.hpp>

#include <sstream>

using namespace OCML;

Integer_Option_Editor::Integer_Option_Editor(wxWindow* parent,
                                             Option_Interface* integer_option)
  : Option_Editor(parent, integer_option),
    focused_(false),
    value_changed_(false)
{
  editor_ = new Integer_Edit_Control(panel());

  if (option()->assigned())
    {
      std::ostringstream os;
      os << integer_option->str_value();
      editor_->SetValue(os.str().c_str());
      button()->Enable(true);
    }
  else
    {
      Basic_Option_Definition<Integer_Option_Trait>* definition =
        dynamic_cast<Basic_Option_Definition<Integer_Option_Trait>*> (integer_option->definition());
      std::ostringstream os;
      os << definition->default_value();
      editor_->SetValue(os.str().c_str());
    }

  editor_->Show(true);
  editor_->add_focus_listener(this);
  editor_->add_value_change_listener(this);
  panel()->GetSizer()->Add(editor_, 1, wxALL | wxADJUST_MINSIZE | wxEXPAND, 2);
    
  panel()->GetSizer()->SetSizeHints(panel());
}

Integer_Option_Editor::~Integer_Option_Editor()
{
}    

void
Integer_Option_Editor::add_focus_listener(Integer_Option_Editor_Focus_Listener* l)
{
  focus_listeners_.push_back(l);
}
    
void
Integer_Option_Editor::integer_edit_focus_gain(Integer_Edit_Control*)
{
  if (focused_)
    return;

  for (std::list<Integer_Option_Editor_Focus_Listener*>::iterator
	 iter = focus_listeners_.begin();
       iter != focus_listeners_.end(); ++iter)
    (*iter)->integer_editor_focused(this);

  focused_ = true;
  value_changed_ = false;
}

void
Integer_Option_Editor::integer_edit_focus_lost(Integer_Edit_Control*)
{
  if (!focused_ || !value_changed_)
    return;

  Integer_Option_Value val;
  val.str_value((const char*) editor_->GetValue());
  option()->value(&val);

  const Integer_Option_Value* basic_value =
    dynamic_cast<const Integer_Option_Value*>(option()->value());
  if (editor_->GetValue() != basic_value->value())
    editor_->SetValue(boost::lexical_cast<std::string>(basic_value->value()).c_str());

  focused_ = false;
}    

void
Integer_Option_Editor::integer_edit_value_changed(Integer_Edit_Control*)
{
  value_changed_ = true;
}

void
Integer_Option_Editor::unfocus()
{
  integer_edit_focus_lost(0);
}
  
void
Integer_Option_Editor::value_changed(bool flagged, Option_Value* value)
{
  Integer_Option_Value* basic_value = dynamic_cast<Integer_Option_Value*>(value);
  Integer_Option_Definition* definition =
    dynamic_cast<Integer_Option_Definition*> (option()->definition());
  Integer_Option_Trait::value_type default_value = definition->default_value();

  if (!value || value->is_null() || (basic_value->value() == default_value))
  {
    editor_->SetValue(boost::lexical_cast<std::string>(default_value).c_str());
    button()->Enable(false);
  }
  else
  {
    editor_->SetValue(boost::lexical_cast<std::string>(basic_value->value()).c_str());
    button()->Enable(true);
  }
}

