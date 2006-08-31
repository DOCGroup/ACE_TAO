/**
 * @file StringEditControl.hpp
 *
 * $Id$
 *
 * @author Emre Turkay <turkaye@dre.vanderbilt.edu>
 *
 * Definition of the string edit control.
 */

#ifndef STRING_EDIT_CONTROL_HPP
#define STRING_EDIT_CONTROL_HPP

#include <list>
#include <wx/wx.h>
#include "../extern_declarations.hpp"

class StringEditControl;

/// The classes which listens the focus gain/lost events should be derived
/// from this class.
class OCMLConfigurator_API StringEditControlFocusListener
{
public:
  /// When a focus gain event occurs this method is called.
  virtual void string_edit_focus_gain(StringEditControl* control) = 0;
  /// When a focus lost event occurs this method is called.
  virtual void string_edit_focus_lost(StringEditControl* control) = 0;
};

/// The classes which listens the text change events should be derived
/// from this class.
class OCMLConfigurator_API StringEditControlValueChangeListener
{
public:
  /// When a value change event occurs this method is called.
  virtual void string_edit_value_changed(StringEditControl* control) = 0;
};

/**
 * String edit control is a single line text input widget which provides
 * focus listener interface. When a focus event occurs, calls the registered
 * listeners.
 */
class OCMLConfigurator_API StringEditControl: public wxTextCtrl
{
public:
  /// WXWindow focus gain event handler.
  void on_focus_gain(wxFocusEvent&);
  /// WXWindow focus lost event handler.
  void on_focus_lost(wxFocusEvent&);
  /// WXWindow text change event handler.
  void on_text_change(wxCommandEvent&);

public:
  /// Constructor.
  StringEditControl(wxWindow* parent);
    
  /// Registers the given FocusListener as a focus listener.
  void add_focus_listener(StringEditControlFocusListener* l);

  /// Registers the given ValueChangeListener as a change listener.
  void add_value_change_listener(StringEditControlValueChangeListener* l);
    
private:
  /// The container for the focus listeners.
  std::list<StringEditControlFocusListener*> focus_listeners_;

  /// The container for the value change listeners.
  std::list<StringEditControlValueChangeListener*> value_change_listeners_;

  // Declares WXWindow event table.
  DECLARE_EVENT_TABLE()
};    

#endif // STRING_EDIT_CONTROL_HPP
