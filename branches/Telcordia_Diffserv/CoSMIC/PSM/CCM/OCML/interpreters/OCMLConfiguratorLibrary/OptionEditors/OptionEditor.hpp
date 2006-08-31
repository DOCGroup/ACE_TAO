#ifndef OPTION_EDITOR_HPP
#define OPTION_EDITOR_HPP

#include <wx/wx.h>
#include "../Options/Option.hpp"
#include "../EditControls/ClearButton.hpp"

namespace OCML
{

  class OptionEditor: public ClearButtonClickListener
  {
  public:
    OptionEditor(wxWindow* parent, Option* option);

    wxPanel* panel();

    Option* option();

    int label_width();
    void label_width(int value);
    virtual void unfocus();

    // This method is remained as abstract.
    //  virtual void clear_button_clicked(ClearButton* control);
  protected:
    ClearButton* button();

  private:
    wxPanel* panel_;
    Option* option_;
    wxBoxSizer* sizer_;
    ClearButton* button_;
    wxStaticText* label_;
  };

} // namespace OCML

#include "OptionEditor.ipp"

#endif // OPTION_EDITOR_HPP
