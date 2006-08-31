// $Id$

#ifndef STRING_OPTION_EDITOR_HPP
#define STRING_OPTION_EDITOR_HPP

#include "option_editor.hpp"
#include "string_edit_control.hpp"
#include <engine/option_interface.hpp>
#include <wx/window.h>

namespace OCML
{

  class String_Option_Editor;

  class String_Option_Editor_Focus_Listener
  {
  public:
    virtual void string_editor_focused(String_Option_Editor* editor) = 0;
  };
    
  class String_Option_Editor: public Option_Editor,
                              public String_Edit_Control_Focus_Listener,
                              public String_Edit_Control_Value_Change_Listener
  {
  public:
    String_Option_Editor(wxWindow* parent, Option_Interface* option);
    virtual ~String_Option_Editor();
    
    virtual void unfocus();

    void add_focus_listener(String_Option_Editor_Focus_Listener* l);
    
    virtual void string_edit_focus_gain(String_Edit_Control* control);
    virtual void string_edit_focus_lost(String_Edit_Control* control);
    virtual void string_edit_value_changed(String_Edit_Control* control);

    virtual void value_changed(bool flagged, Option_Value* value);

  private:
    bool focused_;
    bool value_changed_;

    String_Edit_Control* editor_;

    std::list<String_Option_Editor_Focus_Listener*> focus_listeners_;
  };

} // namespace OCML

#endif // STRING_OPTION_EDITOR_HPP
