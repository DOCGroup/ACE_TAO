#ifndef BOOLEAN_OPTION_EDITOR_HPP
#define BOOLEAN_OPTION_EDITOR_HPP

#include "Option_Editor.hpp"
#include "boolean_edit_control.hpp"
#include <engine/option_interface.hpp>

namespace OCML
{

  class Boolean_Option_Editor;

  class Boolean_Option_Editor_Focus_Listener
  {
  public:
    virtual void boolean_editor_focused(Boolean_Option_Editor* editor) = 0;
  };
    
  class Boolean_Option_Editor: public Option_Editor,
                               public Boolean_Edit_Control_Focus_Listener,
                               public Boolean_Edit_Control_Click_Listener
  {
  public:
    Boolean_Option_Editor(wxWindow* parent, Option_Interface* option);
    virtual ~Boolean_Option_Editor();

    void add_focus_listener(Boolean_Option_Editor_Focus_Listener* l);
    
    virtual void boolean_edit_clicked(Boolean_Edit_Control* control);
    virtual void boolean_edit_focus_gain(Boolean_Edit_Control* control);
    virtual void boolean_edit_focus_lost(Boolean_Edit_Control* control);

    virtual void value_changed(bool flagged, Option_Value* value);

  private:
    Boolean_Edit_Control* editor_;

    std::list<Boolean_Option_Editor_Focus_Listener*> focus_listeners_;
  };

} // namespace OCML

#endif // BOOLEAN_OPTION_EDITOR_HPP
