/**
 * @file ClearButton.hpp
 *
 * $Id$
 *
 * Definition of the clear button.
 *
 * @author Emre Turkay <turkaye@dre.vanderbilt.edu>
 */

#ifndef CLEAR_BUTTON_HPP
#define CLEAR_BUTTON_HPP

#include <list>
#include <wx/wx.h>
#include "../extern_declarations.hpp"

class ClearButton;

class OCMLConfigurator_API ClearButtonClickListener
{
public:
  virtual void clear_button_clicked(ClearButton* control) = 0;
};

/**
 * Clear button is a wrapper facade for a GUI button with "clear" text on it.
 * Provides callback mechanism for on click messages.
 */
class OCMLConfigurator_API ClearButton: public wxButton
{
public:
  /// The wxWindows on_click event listener.
  void on_click(wxCommandEvent&);

public:
  /// The constructor accepts the wxWindows parent control.
  ClearButton(wxWindow* parent);
  
  /// Registers the given listeners.
  void add_click_listener(ClearButtonClickListener* l);
    
private:
  /// The container for holding the click event listerns.
  std::list<ClearButtonClickListener*> click_listeners_;
    
  DECLARE_EVENT_TABLE()
};    

#endif // CLEAR_BUTTON_HPP
