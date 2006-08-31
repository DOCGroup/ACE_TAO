/**
 * @file BooleanEditControl.hpp
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
#include "../extern_declarations.hpp"

class BooleanEditControl;

/// The classes which listens the click event should be derived from
/// this class.
class BooleanEditControlClickListener
{
public:
  /// When a click event occurs this method is called.
  virtual void boolean_edit_clicked(BooleanEditControl* control) = 0;
};

/// The classes which listens the focus gain/lost events should be derived
/// from this class.
class BooleanEditControlFocusListener
{
public:
  /// When a focus gain event occurs this method is called.
  virtual void boolean_edit_focus_gain(BooleanEditControl* control) = 0;
  /// When a focus lost event occurs this method is called.
  virtual void boolean_edit_focus_lost(BooleanEditControl* control) = 0;
};

/**
 * Boolean edit control is a CheckBox which provides click & Focus listener
 * interfaces. When a click event or focus event occurs, calls the registered
 * listeners.
 */
class OCMLConfigurator_API BooleanEditControl: public wxCheckBox
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
  BooleanEditControl(wxWindow* parent);

  /// Registers the given ClickListener as a click listener for this widget.
  void add_click_listener(BooleanEditControlClickListener* l);
  /// Registers the given FocusListener as a focus listener for this widget.
  void add_focus_listener(BooleanEditControlFocusListener* l);
    
private:
  /// The container for the click listeners.
  std::list<BooleanEditControlClickListener*> click_listeners_;
  /// The container for the focus listeners.
  std::list<BooleanEditControlFocusListener*> focus_listeners_;
    
  // Declares WXWindow event table.
  DECLARE_EVENT_TABLE()
};    

#endif // BOOLEAN_EDIT_CONTROL_HPP
