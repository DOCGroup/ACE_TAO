/**
 * @file Boolean_Edit_Control.hpp
 *
 * $Id$
 *
 * Definition of the boolean edit control.
 *
 * @author Emre Turkay <turkaye@dre.vanderbilt.edu>
 */

#ifndef BOOLEAN_EDIT_CONTROL_HPP
#define BOOLEAN_EDIT_CONTROL_HPP

#include <list>
#include <wx/wx.h>

class Boolean_Edit_Control;

/// The classes which listens the click event should be derived from
/// this class.
class Boolean_Edit_Control_Click_Listener
{
public:
  /// When a click event occurs this method is called.
  virtual void boolean_edit_clicked(Boolean_Edit_Control* control) = 0;
};

/// The classes which listens the focus gain/lost events should be derived
/// from this class.
class Boolean_Edit_Control_Focus_Listener
{
public:
  /// When a focus gain event occurs this method is called.
  virtual void boolean_edit_focus_gain(Boolean_Edit_Control* control) = 0;
  /// When a focus lost event occurs this method is called.
  virtual void boolean_edit_focus_lost(Boolean_Edit_Control* control) = 0;
};

/**
 * Boolean edit control is a CheckBox which provides click & Focus listener
 * interfaces. When a click event or focus event occurs, calls the registered
 * listeners.
 */
class Boolean_Edit_Control: public wxCheckBox
{
public:
  /// WXWindow click event handler.
  void on_click(wxCommandEvent&);
  /// WXWindow focus gain event handler.
  void on_focus_gain(wxFocusEvent&);
  /// WXWindow focus lost event handler.
  void on_focus_lost(wxFocusEvent&);

public:
  /// Constructor.
  Boolean_Edit_Control(wxWindow* parent);

  /// Registers the given ClickListener as a click listener for this widget.
  void add_click_listener(Boolean_Edit_Control_Click_Listener* l);
  /// Registers the given FocusListener as a focus listener for this widget.
  void add_focus_listener(Boolean_Edit_Control_Focus_Listener* l);
    
private:
  /// The container for the click listeners.
  std::list<Boolean_Edit_Control_Click_Listener*> click_listeners_;
  /// The container for the focus listeners.
  std::list<Boolean_Edit_Control_Focus_Listener*> focus_listeners_;
    
  // Declares WXWindow event table.
  DECLARE_EVENT_TABLE()
};    

#endif // BOOLEAN_EDIT_CONTROL_HPP
