/**
 * @file Option_Editor.hpp
 *
 * $Id$
 *
 * Defines the Option_Editor class.
 *
 * @author Emre Turkay <turkaye@dre.vanderbilt.edu>
 */

#ifndef OPTION_EDITOR_HPP
#define OPTION_EDITOR_HPP

#include <wx/wx.h>
#include <engine/option_interface.hpp>
#include "clear_button.hpp"

namespace OCML
{

  /**
   * Base class of all option editors.
   * Option editors are GUI components containing a panel and a specific
   * GUI widget on top of them which allows user to enter values.
   */
  class Option_Editor: public Clear_Button_Click_Listener,
                       public Option_Interface_Value_Change_Listener
  {
  public:
    /// Constructor accepts the wxWindows parent widget and option.
    Option_Editor(wxWindow* parent, Option_Interface* option);

    /// Destructor stops listening the value changes of option interface.
    ~Option_Editor();

    /// Getter function for the panel.
    wxPanel* panel();

    /// Getter function for the option.
    Option_Interface* option();

    /// Getter function for the witdth of the label.
    int label_width();

    /// Function for changing the width of the label.
    void label_width(int value);

    /// @todo: what is this.
    virtual void unfocus();

    virtual void interface_destroyed();

    virtual void clear_button_clicked(Clear_Button*);
  protected:
    /// Getter function for the button.
    Clear_Button* button();

  private:
    /// The GUI panel containing the edit control.
    wxPanel* panel_;
    /// The pointer to the option which this editor is associated with.
    Option_Interface* option_;
    /// The sizer of the panel.
    wxBoxSizer* sizer_;
    /// The button with the caption "clear".
    Clear_Button* button_;
    /// The label displaying the name of the option.
    wxStaticText* label_;
    /// Option editor listener index (listening option interface value change).
    Option_Interface::listener_index listener_index_;
  };

} // namespace OCML

#include "option_editor.ipp"

#endif // OPTION_EDITOR_HPP
