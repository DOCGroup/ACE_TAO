// $Id$

#include "BooleanOptionEditor.hpp"

#include "../../Common/Debug.hpp"

using namespace OCML;

BooleanOptionEditor::BooleanOptionEditor(wxWindow* parent,
                                         BooleanOption* boolean_option)
  : OptionEditor(parent, boolean_option)
{
  editor_ = new BooleanEditControl(panel());

  if (option()->assigned())
    {
      editor_->SetValue(boolean_option->get());
      button()->Enable(true);
    }
  else
    {
      editor_->SetValue(boolean_option->default_value());
    }

  editor_->Show(true);
  editor_->add_focus_listener(this);
  editor_->add_click_listener(this);
  panel()->GetSizer()->Add(editor_, 1, wxALL | wxADJUST_MINSIZE | wxEXPAND, 2);

  panel()->GetSizer()->SetSizeHints(panel());
}

BooleanOptionEditor::~BooleanOptionEditor()
{
}    

void
BooleanOptionEditor::add_focus_listener(BooleanOptionEditorFocusListener* l)
{
  focus_listeners_.push_back(l);
}
    
void
BooleanOptionEditor::boolean_edit_focus_gain(BooleanEditControl*)
{
  for (std::list<BooleanOptionEditorFocusListener*>::iterator
	 iter = focus_listeners_.begin();
       iter != focus_listeners_.end(); ++iter)
    (*iter)->boolean_editor_focused(this);
}

void
BooleanOptionEditor::boolean_edit_focus_lost(BooleanEditControl*)
{
  // Nothing to do here
}    

void
BooleanOptionEditor::boolean_edit_clicked(BooleanEditControl*)
{
  BooleanOption* boolean_option = (BooleanOption*) option();
  boolean_option->set(editor_->GetValue());
  button()->Enable(true);
}

void
BooleanOptionEditor::clear_button_clicked(ClearButton*)
{
  option()->clear();
  button()->Enable(false);

  BooleanOption* boolean_option = (BooleanOption*) option();
  editor_->SetValue(boolean_option->default_value());
}
