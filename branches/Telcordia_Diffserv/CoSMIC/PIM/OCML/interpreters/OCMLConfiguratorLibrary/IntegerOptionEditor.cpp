// $Id$

#include "IntegerOptionEditor.hpp"

#include <sstream>

using namespace OCML;

IntegerOptionEditor::IntegerOptionEditor(wxWindow* parent,
                                         IntegerOption* integer_option)
  : OptionEditor(parent, integer_option),
    focused_(false),
    value_changed_(false)
{
  editor_ = new IntegerEditControl(panel());

  if (option()->assigned())
    {
      std::ostringstream os;
      os << integer_option->get();
      editor_->SetValue(os.str().c_str());
      button()->Enable(true);
    }
  else
    {
      std::ostringstream os;
      os << integer_option->default_value();
      editor_->SetValue(os.str().c_str());
    }

  editor_->Show(true);
  editor_->add_focus_listener(this);
  editor_->add_value_change_listener(this);
  panel()->GetSizer()->Add(editor_, 1, wxALL | wxADJUST_MINSIZE | wxEXPAND, 2);
    
  panel()->GetSizer()->SetSizeHints(panel());
}

IntegerOptionEditor::~IntegerOptionEditor()
{
}    

void
IntegerOptionEditor::add_focus_listener(IntegerOptionEditorFocusListener* l)
{
  focus_listeners_.push_back(l);
}
    
void
IntegerOptionEditor::integer_edit_focus_gain(IntegerEditControl*)
{
  if (focused_)
    return;

  for (std::list<IntegerOptionEditorFocusListener*>::iterator
	 iter = focus_listeners_.begin();
       iter != focus_listeners_.end(); ++iter)
    (*iter)->integer_editor_focused(this);

  focused_ = true;
  value_changed_ = false;
}

void
IntegerOptionEditor::integer_edit_focus_lost(IntegerEditControl*)
{
  if (!focused_ || !value_changed_)
    return;

  IntegerOption* integer_option = (IntegerOption*) option();

  std::istringstream is((const char*) editor_->GetValue());
  int val;
  is >> val;
  integer_option->set(val);
  button()->Enable(true);

  focused_ = false;
}    

void
IntegerOptionEditor::integer_edit_value_changed(IntegerEditControl*)
{
  value_changed_ = true;
}

void
IntegerOptionEditor::clear_button_clicked(ClearButton*)
{
  option()->clear();
  button()->Enable(false);

  IntegerOption* integer_option = (IntegerOption*) option();
  std::ostringstream os;
  os << integer_option->default_value();
  editor_->SetValue(os.str().c_str());
}

void
IntegerOptionEditor::unfocus()
{
  integer_edit_focus_lost(0);
}
