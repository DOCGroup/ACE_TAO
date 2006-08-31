namespace OCML
{

  /**
   * Initializes the GUI controls; panel(where the edit control is places),
   * clear button, and label after that, initializes them.
   */
  inline
  Option_Editor::Option_Editor(wxWindow* parent, Option_Interface* option)
    : panel_(new wxPanel(parent, -1)), option_(option),
      sizer_(new wxBoxSizer(wxHORIZONTAL)),
      button_(new Clear_Button(panel())),
      label_(new wxStaticText(panel_, -1, option->definition()->name().c_str()))
  {
    panel_->Show(true);
    panel_->SetSizer(sizer_);

    button_->add_click_listener(this);
    button_->Enable(option_->assigned());
    sizer_->Add(button_, 0, wxALL, 2);

    label_->Show(true);
    sizer_->Add(label_, 0, wxALL, 2);

    listener_index_ = option->register_listener(this);
  }

  inline Option_Editor::~Option_Editor()
  {
    if (option_)
      option_->unregister_listener(listener_index_);
  }
  
  inline void Option_Editor::interface_destroyed()
  {
    option_ = 0;
  }

  inline void
  Option_Editor::unfocus()
  {
    // @todo: This is a virtual function thus no reason for making it inline.
  }

  inline wxPanel*
  Option_Editor::panel()
  {
    return panel_;
  }

  inline Option_Interface*
  Option_Editor::option()
  {
    return option_;
  }

  inline int
  Option_Editor::label_width()
  {
    return label_->GetSize().GetWidth();
  }

  inline void
  Option_Editor::label_width(int value)
  {
    label_->SetSize(value, -1);
  }

  inline Clear_Button*
  Option_Editor::button()
  {
    return button_;
  }

  inline void
  Option_Editor::clear_button_clicked(Clear_Button*)
  {
    option()->clear();
  }
  
} // namespace OCML
