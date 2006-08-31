namespace OCML
{

  inline
  OptionEditor::OptionEditor(wxWindow* parent, Option* option)
    : panel_(new wxPanel(parent, -1)), option_(option),
      sizer_(new wxBoxSizer(wxHORIZONTAL)),
      button_(new ClearButton(panel())),
      label_(new wxStaticText(panel_, -1, option->name().c_str()))
  {
    panel_->Show(true);
    panel_->SetSizer(sizer_);

    button_->add_click_listener(this);
    button_->Enable(option_->assigned());
    sizer_->Add(button_, 0, wxALL, 2);

    label_->Show(true);
    sizer_->Add(label_, 0, wxALL, 2);
  }

  inline void
  OptionEditor::unfocus()
  {
    // TODO: This is a virtual function thus no reason for making it inline.
  }

  inline wxPanel*
  OptionEditor::panel()
  {
    return panel_;
  }

  inline Option*
  OptionEditor::option()
  {
    return option_;
  }

  inline int
  OptionEditor::label_width()
  {
    return label_->GetSize().GetWidth();
  }

  inline void
  OptionEditor::label_width(int value)
  {
    label_->SetSize(value, -1);
  }

  inline ClearButton*
  OptionEditor::button()
  {
    return button_;
  }

} // namespace OCML
