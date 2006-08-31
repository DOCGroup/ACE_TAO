#ifndef BOOLEAN_OPTION_EDITOR_HPP
#define BOOLEAN_OPTION_EDITOR_HPP

#include "OptionEditor.hpp"
#include "../EditControls/BooleanEditControl.hpp"
#include "../Options/BasicOptions.hpp"

namespace OCML
{

  class BooleanOptionEditor;

  class BooleanOptionEditorFocusListener
  {
  public:
    virtual void boolean_editor_focused(BooleanOptionEditor* editor) = 0;
  };
    
  class BooleanOptionEditor: public OptionEditor,
                             public BooleanEditControlFocusListener,
                             public BooleanEditControlClickListener
  {
  public:
    BooleanOptionEditor(wxWindow* parent, BooleanOption* option);
    virtual ~BooleanOptionEditor();

    void add_focus_listener(BooleanOptionEditorFocusListener* l);
    
    virtual void boolean_edit_clicked(BooleanEditControl* control);
    virtual void boolean_edit_focus_gain(BooleanEditControl* control);
    virtual void boolean_edit_focus_lost(BooleanEditControl* control);
    virtual void clear_button_clicked(ClearButton* control);

  private:
    BooleanEditControl* editor_;

    std::list<BooleanOptionEditorFocusListener*> focus_listeners_;
  };

} // namespace OCML

#endif // BOOLEAN_OPTION_EDITOR_HPP
