#ifndef EDITOR_PANEL_HPP
#define EDITOR_PANEL_HPP

#include <wx/wx.h>

#include "boolean_option_editor.hpp"
#include "string_option_editor.hpp"
#include "integer_option_editor.hpp"
//#include "enum_option_editor.hpp"

#include <engine/definitions.hpp>

#include <list>

namespace OCML
{

  class Editor_Panel_Focus_Listener
  {
  public:
    virtual void editor_focused(Option_Editor* editor) = 0;
  };

  class Configurator;
  
  /**
   * Creates a panel and inserts corresponding wxWindows into the panel for
   * the options contained in the given option category.
   */
  class Editor_Panel
    : public wxScrolledWindow,
      public Boolean_Option_Editor_Focus_Listener,
      public String_Option_Editor_Focus_Listener,
      public Integer_Option_Editor_Focus_Listener//,
//      public Enum_Option_Editor_Focus_Listener
  {
  public:
    Editor_Panel(wxWindow* parent, Option_Category* category,
                 Configurator* configurator);
    ~Editor_Panel();

    void add_focus_listener(Editor_Panel_Focus_Listener* l);

    void set_category(Option_Category* category);

    virtual void boolean_editor_focused(Boolean_Option_Editor* editor);
    virtual void string_editor_focused(String_Option_Editor* editor);
    virtual void integer_editor_focused(Integer_Option_Editor* editor);
//    virtual void enum_editor_focused(Enum_Option_Editor* editor);

  private:
    wxBoxSizer* sizer_;

    std::list<Option_Editor*> editors_;
    std::list<Editor_Panel_Focus_Listener*> focus_listeners_;
    OCML::Configurator* configurator_;
  };

} // namespace OCML

#endif // EDITOR_PANEL_HPP
