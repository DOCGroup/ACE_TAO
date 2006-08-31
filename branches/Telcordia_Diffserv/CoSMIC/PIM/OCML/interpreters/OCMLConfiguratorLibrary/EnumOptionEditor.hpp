#ifndef ENUM_OPTION_EDITOR_HPP
#define ENUM_OPTION_EDITOR_HPP

#include "OptionEditor.hpp"
#include "../EditControls/EnumEditControl.hpp"
#include "../Options/Options.hpp"

namespace OCML
{

  class EnumOptionEditor;

  class EnumOptionEditorFocusListener
  {
  public:
    virtual void enum_editor_focused(EnumOptionEditor* editor) = 0;
  };

  class EnumOptionEditor: public OptionEditor,
                          public EnumEditControlFocusListener,
                          public EnumEditControlSelectListener
  {
  public:
    EnumOptionEditor(wxWindow* parent, EnumOption* option);
    virtual ~EnumOptionEditor();
    
    void add_focus_listener(EnumOptionEditorFocusListener* l);
    
    virtual void enum_edit_selected(std::string value, void* client_data);
    virtual void enum_edit_focus_gain(EnumEditControl* control);
    virtual void enum_edit_focus_lost(EnumEditControl* control);
    virtual void clear_button_clicked(ClearButton* control);

  private:
    EnumEditControl* editor_;

    std::list<EnumOptionEditorFocusListener*> focus_listeners_;
  };

} // namespace OCML

#endif // ENUM_OPTION_EDITOR_HPP
