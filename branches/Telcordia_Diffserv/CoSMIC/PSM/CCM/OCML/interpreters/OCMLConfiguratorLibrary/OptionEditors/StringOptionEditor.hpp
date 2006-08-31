// $Id$

#ifndef STRING_OPTION_EDITOR_HPP
#define STRING_OPTION_EDITOR_HPP

#include "OptionEditor.hpp"
#include "../EditControls/StringEditControl.hpp"
#include "../Options/BasicOptions.hpp"

namespace OCML
{

  class StringOptionEditor;

  class StringOptionEditorFocusListener
  {
  public:
    virtual void string_editor_focused(StringOptionEditor* editor) = 0;
  };
    
  class StringOptionEditor: public OptionEditor,
                            public StringEditControlFocusListener,
                            public StringEditControlValueChangeListener
  {
  public:
    StringOptionEditor(wxWindow* parent, StringOption* option);
    virtual ~StringOptionEditor();
    
    virtual void unfocus();

    void add_focus_listener(StringOptionEditorFocusListener* l);
    
    virtual void string_edit_focus_gain(StringEditControl* control);
    virtual void string_edit_focus_lost(StringEditControl* control);
    virtual void string_edit_value_changed(StringEditControl* control);
    virtual void clear_button_clicked(ClearButton* control);

  private:
    bool focused_;
    bool value_changed_;

    StringEditControl* editor_;

    std::list<StringOptionEditorFocusListener*> focus_listeners_;
  };

} // namespace OCML

#endif // STRING_OPTION_EDITOR_HPP
