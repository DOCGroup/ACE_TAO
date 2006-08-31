/**
 * @file Integer_Edit_Control.hpp
 *
 * $Id$
 *
 * @author Emre Turkay <turkaye@dre.vanderbilt.edu>
 *
 * Definition of the integer edit control.
 */

#ifndef INTEGER_EDIT_CONTROL_HPP
#define INTEGER_EDIT_CONTROL_HPP

#include <list>
#include <wx/wx.h>

class Integer_Edit_Control;

/// The classes which listens the focus gain/lost events should be derived
/// from this class.
class Integer_Edit_Control_Focus_Listener
{
public:
  /// When a focus gain event occurs this method is called.
  virtual void integer_edit_focus_gain(Integer_Edit_Control* control) = 0;
  /// When a focus lost event occurs this method is called.
  virtual void integer_edit_focus_lost(Integer_Edit_Control* control) = 0;
};

/// The classes which listens the text change events should be derived
/// from this class.
class Integer_Edit_Control_Value_Change_Listener
{
public:
  /// When a value change event occurs this method is called.
  virtual void integer_edit_value_changed(Integer_Edit_Control* control) = 0;
};

/**
 * Integer edit control is a single line text input widget which provides
 * focus listener interface. When a focus event occurs, calls the registered
 * listeners.
 */
class Integer_Edit_Control: public wxTextCtrl
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
  Integer_Edit_Control(wxWindow* parent);
    
  /// Registers the given FocusListener as a focus listener.
  void add_focus_listener(Integer_Edit_Control_Focus_Listener* l);

  /// Registers the given ValueChangeListener as a change listener.
  void add_value_change_listener(Integer_Edit_Control_Value_Change_Listener* l);
    
private:
  /// The container for the focus listeners.
  std::list<Integer_Edit_Control_Focus_Listener*> focus_listeners_;
    
  /// The container for the value change listeners.
  std::list<Integer_Edit_Control_Value_Change_Listener*> value_change_listeners_;

  // Declares WXWindow event table.
  DECLARE_EVENT_TABLE()
};    

#endif // INTEGER_EDIT_CONTROL_HPP
