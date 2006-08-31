#include "editor_panel.hpp"
#include <engine/configurator.hpp>

namespace OCML {

Editor_Panel::Editor_Panel(wxWindow* parent, Option_Category* category, Configurator* configurator)
  :  wxScrolledWindow(parent, -1, wxPoint(0, 0), wxSize(400, 400)),
     sizer_(new wxBoxSizer(wxVERTICAL)),
     configurator_(configurator)
{
  this->SetScrollRate(5, 5);
  this->FitInside();
  this->SetSizer(sizer_);

  set_category(category);
}

Editor_Panel::~Editor_Panel()
{
  for (std::list<Option_Editor*>::iterator iter = editors_.begin();
      iter != editors_.end(); ++iter)
  {
    delete (*iter);
  }
}

void
Editor_Panel::set_category(Option_Category* category)
{
  for (std::list<Option_Editor*>::iterator e_iter = editors_.begin();
      e_iter != editors_.end(); ++e_iter)
  {
    (*e_iter)->unfocus();
  }

  DestroyChildren();
  while (sizer_->Remove(0));
  sizer_->Layout();

  for (Option_Category::option_iterator o_iter = category->begin_options();
      o_iter != category->end_options(); ++o_iter)
  {
    Option_Definition* option = *o_iter;
    Option_Editor* editor = NULL;
    Option_Interface* interface = configurator_->interface(option);

    if (dynamic_cast<Basic_Option_Definition<Boolean_Option_Trait>*>(option))
      {
        Boolean_Option_Editor* boolean_editor =
          new Boolean_Option_Editor(this, interface);
        editor = boolean_editor;
        boolean_editor->add_focus_listener(this);
      }
    else
      if (dynamic_cast<Basic_Option_Definition<String_Option_Trait>*>(option))
      {
        String_Option_Editor* string_editor =
          new String_Option_Editor(this, interface);
        editor = string_editor;
        string_editor->add_focus_listener(this);
      }
    else
      if (dynamic_cast<Basic_Option_Definition<Integer_Option_Trait>*>(option))
      {
        Integer_Option_Editor* integer_editor =
          new Integer_Option_Editor(this, interface);
        editor = integer_editor;
        integer_editor->add_focus_listener(this);
      }
/*    if (dynamic_cast<Basic_Option_Definition<Boolean_Option_Trait>*> (option()->definition()))
      case Option::ovk_enum:
      {
        Enum_Option_Editor_Focus_Listener* focus_listener =
          (Enum_Option_Editor_Focus_Listener*) this;
        Enum_Option* enum_option =
          (Enum_Option*) option;

        Enum_Option_Editor* enum_editor =
          new Enum_Option_Editor(this, enum_option);
        editor = enum_editor;
        enum_editor->add_focus_listener(focus_listener);
      }
*/

    editor->panel()->Show(true);
    sizer_->Add(editor->panel(), 0, wxEXPAND | wxALL | wxADJUST_MINSIZE, 2);
    editors_.push_back(editor);
  }
  sizer_->SetVirtualSizeHints(this);
  sizer_->Layout();
  this->FitInside();
}

void
Editor_Panel::add_focus_listener(Editor_Panel_Focus_Listener* l)
{
  focus_listeners_.push_back(l);
}

void
Editor_Panel::boolean_editor_focused(Boolean_Option_Editor* editor)
{
  for (std::list<Editor_Panel_Focus_Listener*>::iterator
      iter = focus_listeners_.begin();
      iter != focus_listeners_.end(); ++iter)
  {
    (*iter)->editor_focused(editor);
  }
}

void
Editor_Panel::string_editor_focused(String_Option_Editor* editor)
{
  for (std::list<Editor_Panel_Focus_Listener*>::iterator
      iter = focus_listeners_.begin();
      iter != focus_listeners_.end(); ++iter)
  {
    (*iter)->editor_focused(editor);
  }
}

void
Editor_Panel::integer_editor_focused(Integer_Option_Editor* editor)
{
  for (std::list<Editor_Panel_Focus_Listener*>::iterator
      iter = focus_listeners_.begin();
      iter != focus_listeners_.end(); ++iter)
  {
    (*iter)->editor_focused(editor);
  }
}
/*
void
Editor_Panel::enum_editor_focused(Enum_Option_Editor* editor)
{
  for (std::list<Editor_Panel_Focus_Listener*>::iterator
      iter = focus_listeners_.begin();
      iter != focus_listeners_.end(); ++iter)
  {
    (*iter)->editor_focused(editor);
  }
}
*/
} // namespace OCML