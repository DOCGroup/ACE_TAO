/**
 * @file Clear_Button.hpp
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

class Clear_Button;

class Clear_Button_Click_Listener
{
public:
  virtual void clear_button_clicked(Clear_Button* control) = 0;
};

/**
 * Clear button is a wrapper facade for a GUI button with "clear" text on it.
 * Provides callback mechanism for on click messages.
 */
class Clear_Button: public wxButton
{
public:
  /// The wxWindows on_click event listener.
  void on_click(wxCommandEvent&);

public:
  /// The constructor accepts the wxWindows parent control.
  Clear_Button(wxWindow* parent);
  
  /// Registers the given listeners.
  void add_click_listener(Clear_Button_Click_Listener* l);
    
private:
  /// The container for holding the click event listerns.
  std::list<Clear_Button_Click_Listener*> click_listeners_;
    
  DECLARE_EVENT_TABLE()
};    

#endif // CLEAR_BUTTON_HPP
