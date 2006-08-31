#ifndef ENUM_OPTION_EDITOR_HPP
#define ENUM_OPTION_EDITOR_HPP

#include "Option_Editor.hpp"
#include <enum_edit_control.hpp>

namespace OCML
{

  class Enum_Option_Editor;

  class Enum_Option_Editor_Focus_Listener
  {
  public:
    virtual void enum_editor_focused(Enum_Option_Editor* editor) = 0;
  };

  class Enum_Option_Editor: public Option_Editor,
                            public Enum_Edit_Control_Focus_Listener,
                            public Enum_Edit_Control_Select_Listener
  {
  public:
    Enum_Option_Editor(wxWindow* parent, Enum_Option* option);
    virtual ~Enum_Option_Editor();
    
    void add_focus_listener(Enum_Option_Editor_Focus_Listener* l);
    
    virtual void enum_edit_selected(std::string value, void* client_data);
    virtual void enum_edit_focus_gain(EnumEditControl* control);
    virtual void enum_edit_focus_lost(EnumEditControl* control);
    virtual void clear_button_clicked(Clear_Button* control);

  private:
    EnumEditControl* editor_;

    std::list<Enum_Option_Editor_Focus_Listener*> focus_listeners_;
  };

} // namespace OCML

#endif // ENUM_OPTION_EDITOR_HPP
