/**
 * @file OptionEditor.hpp
 *
 * $Id$
 *
 * Defines the OptionEditor class.
 *
 * @author Emre Turkay <turkaye@dre.vanderbilt.edu>
 */

#ifndef OPTION_EDITOR_HPP
#define OPTION_EDITOR_HPP

#include <wx/wx.h>
#include "../Options/Option.hpp"
#include "../EditControls/ClearButton.hpp"

namespace OCML
{

  /**
   * Base class of all option editors.
   * Option editors are GUI components containing a panel and a specific
   * GUI widget on top of them which allows user to enter values.
   */
  class OptionEditor: public ClearButtonClickListener
  {
  public:
    /// Constructor accepts the wxWindows parent widget and option.
    OptionEditor(wxWindow* parent, Option* option);

    /// Getter function for the panel.
    wxPanel* panel();

    /// Getter function for the option.
    Option* option();

    /// Getter function for the witdth of the label.
    int label_width();

    /// Function for changing the width of the label.
    void label_width(int value);

    /// @todo: what is this.
    virtual void unfocus();

    // This method is remained as abstract.
    //  virtual void clear_button_clicked(ClearButton* control);
  protected:
    /// Getter function for the button.
    ClearButton* button();

  private:
    /// The GUI panel containing the edit control.
    wxPanel* panel_;
    /// The pointer to the option which this editor is associated with.
    Option* option_;
    /// The sizer of the panel.
    wxBoxSizer* sizer_;
    /// The button with the caption "clear".
    ClearButton* button_;
    /// The label displaying the name of the option.
    wxStaticText* label_;
  };

} // namespace OCML

#include "OptionEditor.ipp"

#endif // OPTION_EDITOR_HPP
