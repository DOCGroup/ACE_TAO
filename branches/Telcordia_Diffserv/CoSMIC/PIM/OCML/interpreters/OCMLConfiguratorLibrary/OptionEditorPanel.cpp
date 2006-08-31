#include "OptionEditorPanel.hpp"

#include "../../Common/Debug.hpp"

using namespace OCML;

OptionEditorPanel::OptionEditorPanel(wxWindow* parent,
                                     OptionCategory* category)
  :  wxScrolledWindow(parent, -1, wxPoint(0, 0), wxSize(400, 400)),
     sizer_(new wxBoxSizer(wxVERTICAL))
{
  this->SetScrollRate(5, 5);
  this->FitInside();
  this->SetSizer(sizer_);
  
  set_category(category);
}

OptionEditorPanel::~OptionEditorPanel()
{
  for (std::list<OptionEditor*>::iterator iter = editors_.begin();
       iter != editors_.end(); ++iter)
    delete (*iter);
}

void
OptionEditorPanel::set_category(OptionCategory* category)
{
  for (std::list<OptionEditor*>::iterator e_iter = editors_.begin();
       e_iter != editors_.end(); ++e_iter)
    (*e_iter)->unfocus();

  DestroyChildren();
  while (sizer_->Remove(0));
  sizer_->Layout();

  for (OptionCategory::option_iterator o_iter = category->begin_options();
       o_iter != category->end_options(); ++o_iter)
    {
      Option* option = *o_iter;
      OptionEditor* editor = NULL;
      
      switch (option->value_kind())
        {
        case Option::ovk_boolean:
	  {
	    BooleanOptionEditorFocusListener* focus_listener =
	      (BooleanOptionEditorFocusListener*) this;
	    BooleanOption* boolean_option =
	      (BooleanOption*) option;
	    
	    BooleanOptionEditor* boolean_editor =
	      new BooleanOptionEditor(this, boolean_option);
	    editor = boolean_editor;
	    boolean_editor->add_focus_listener(focus_listener);
	    break;
	  }
        case Option::ovk_string:
	  {
	    StringOptionEditorFocusListener* focus_listener =
	      (StringOptionEditorFocusListener*) this;
	    StringOption* string_option =
	      (StringOption*) option;

	    StringOptionEditor* string_editor =
	      new StringOptionEditor(this, string_option);
	    editor = string_editor;
	    string_editor->add_focus_listener(focus_listener);
	    break;
	  }
        case Option::ovk_integer:
	  {
	    IntegerOptionEditorFocusListener* focus_listener =
	      (IntegerOptionEditorFocusListener*) this;
	    IntegerOption* integer_option =
	      (IntegerOption*) option;

	    IntegerOptionEditor* integer_editor =
	      new IntegerOptionEditor(this, integer_option);
	    editor = integer_editor;
	    integer_editor->add_focus_listener(focus_listener);
	    break;
	  }
        case Option::ovk_enum:
	  {
	    EnumOptionEditorFocusListener* focus_listener =
	      (EnumOptionEditorFocusListener*) this;
	    EnumOption* enum_option =
	      (EnumOption*) option;

	    EnumOptionEditor* enum_editor =
              new EnumOptionEditor(this, enum_option);
	    editor = enum_editor;
	    enum_editor->add_focus_listener(focus_listener);
	    break;
	  }
        default:
          break;
        }

      editor->panel()->Show(true);
      sizer_->Add(editor->panel(), 0, wxEXPAND | wxALL | wxADJUST_MINSIZE, 2);
      editors_.push_back(editor);
    }
  sizer_->SetVirtualSizeHints(this);
  sizer_->Layout();
  this->FitInside();
}

void
OptionEditorPanel::add_focus_listener(OptionEditorPanelFocusListener* l)
{
  focus_listeners_.push_back(l);
}
    
void
OptionEditorPanel::boolean_editor_focused(BooleanOptionEditor* editor)
{
  for (std::list<OptionEditorPanelFocusListener*>::iterator
	 iter = focus_listeners_.begin();
       iter != focus_listeners_.end(); ++iter)
    (*iter)->editor_focused(editor);
}

void
OptionEditorPanel::string_editor_focused(StringOptionEditor* editor)
{
  for (std::list<OptionEditorPanelFocusListener*>::iterator
	 iter = focus_listeners_.begin();
       iter != focus_listeners_.end(); ++iter)
    (*iter)->editor_focused(editor);
}

void
OptionEditorPanel::integer_editor_focused(IntegerOptionEditor* editor)
{
  for (std::list<OptionEditorPanelFocusListener*>::iterator
	 iter = focus_listeners_.begin();
       iter != focus_listeners_.end(); ++iter)
    (*iter)->editor_focused(editor);
}

void
OptionEditorPanel::enum_editor_focused(EnumOptionEditor* editor)
{
  for (std::list<OptionEditorPanelFocusListener*>::iterator
	 iter = focus_listeners_.begin();
       iter != focus_listeners_.end(); ++iter)
    (*iter)->editor_focused(editor);
}
