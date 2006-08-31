// $Id$

#ifndef INTEGER_OPTION_EDITOR_HPP
#define INTEGER_OPTION_EDITOR_HPP

#include "option_editor.hpp"
#include "integer_edit_control.hpp"
#include <engine/option_interface.hpp>

namespace OCML
{

  class Integer_Option_Editor;

  class Integer_Option_Editor_Focus_Listener
  {
  public:
    virtual void integer_editor_focused(Integer_Option_Editor* editor) = 0;
  };
    
  class Integer_Option_Editor: public Option_Editor,
                               public Integer_Edit_Control_Focus_Listener,
                               public Integer_Edit_Control_Value_Change_Listener
  {
  public:
    Integer_Option_Editor(wxWindow* parent, Option_Interface* option);
    virtual ~Integer_Option_Editor();
    
    virtual void unfocus();

    void add_focus_listener(Integer_Option_Editor_Focus_Listener* l);
    
    virtual void integer_edit_focus_gain(Integer_Edit_Control* control);
    virtual void integer_edit_focus_lost(Integer_Edit_Control* control);
    virtual void integer_edit_value_changed(Integer_Edit_Control* control);

    virtual void value_changed(bool flagged, Option_Value* value);

  private:
    bool focused_;
    bool value_changed_;

    Integer_Edit_Control* editor_;

    std::list<Integer_Option_Editor_Focus_Listener*> focus_listeners_;
  };

} // namespace OCML

#endif // INTEGER_OPTION_EDITOR_HPP
