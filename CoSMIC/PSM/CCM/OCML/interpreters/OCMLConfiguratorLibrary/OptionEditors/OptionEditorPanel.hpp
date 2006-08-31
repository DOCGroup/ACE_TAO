#ifndef OPTION_EDITOR_PANEL_HPP
#define OPTION_EDITOR_PANEL_HPP

#include <wx/wx.h>

#include "BooleanOptionEditor.hpp"
#include "StringOptionEditor.hpp"
#include "IntegerOptionEditor.hpp"
#include "EnumOptionEditor.hpp"

#include "../Options/OptionCategory.hpp"

namespace OCML
{

  class OptionEditorPanelFocusListener
  {
  public:
    virtual void editor_focused(OptionEditor* editor) = 0;
  };
    
  /**
   * Creates a panel and inserts corresponding wxWindows into the panel for
   * the options contained in the given option category.
   */
  class OCMLConfigurator_API OptionEditorPanel
    : public wxScrolledWindow,
      public BooleanOptionEditorFocusListener,
      public StringOptionEditorFocusListener,
      public IntegerOptionEditorFocusListener,
      public EnumOptionEditorFocusListener
  {
  public:
    OptionEditorPanel(wxWindow* parent, OptionCategory* category);
    ~OptionEditorPanel();

    void add_focus_listener(OptionEditorPanelFocusListener* l);

    void set_category(OptionCategory* category);

    virtual void boolean_editor_focused(BooleanOptionEditor* editor);
    virtual void string_editor_focused(StringOptionEditor* editor);
    virtual void integer_editor_focused(IntegerOptionEditor* editor);
    virtual void enum_editor_focused(EnumOptionEditor* editor);

  private:
    wxBoxSizer* sizer_;

    std::list<OptionEditor*> editors_;
    std::list<OptionEditorPanelFocusListener*> focus_listeners_;
  };

} // namespace OCML

#endif // OPTION_EDITOR_PANEL_HPP
