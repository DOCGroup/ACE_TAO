// $Id$

#ifndef INTEGER_OPTION_EDITOR_HPP
#define INTEGER_OPTION_EDITOR_HPP

#include "OptionEditor.hpp"
#include "../EditControls/IntegerEditControl.hpp"
#include "../Options/BasicOptions.hpp"

namespace OCML
{

  class IntegerOptionEditor;

  class IntegerOptionEditorFocusListener
  {
  public:
    virtual void integer_editor_focused(IntegerOptionEditor* editor) = 0;
  };
    
  class IntegerOptionEditor: public OptionEditor,
                             public IntegerEditControlFocusListener,
                             public IntegerEditControlValueChangeListener
  {
  public:
    IntegerOptionEditor(wxWindow* parent, IntegerOption* option);
    virtual ~IntegerOptionEditor();
    
    virtual void unfocus();

    void add_focus_listener(IntegerOptionEditorFocusListener* l);
    
    virtual void integer_edit_focus_gain(IntegerEditControl* control);
    virtual void integer_edit_focus_lost(IntegerEditControl* control);
    virtual void integer_edit_value_changed(IntegerEditControl* control);
    virtual void clear_button_clicked(ClearButton* control);

  private:
    bool focused_;
    bool value_changed_;

    IntegerEditControl* editor_;

    std::list<IntegerOptionEditorFocusListener*> focus_listeners_;
  };

} // namespace OCML

#endif // INTEGER_OPTION_EDITOR_HPP
