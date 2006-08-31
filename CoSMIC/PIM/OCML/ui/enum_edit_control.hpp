/**
 * @file EnumEditControl.hpp
 *
 * $Id$
 *
 * @author Emre Turkay <turkaye@dre.vanderbilt.edu>
 *
 * Definition of the enum edit control.
 */

#ifndef ENUM_EDIT_CONTROL_HPP
#define ENUM_EDIT_CONTROL_HPP

#include <list>
#include <string>
#include <wx/wx.h>

class EnumEditControl;

/// The classes which listens the select event should be derived from
/// this class.
class OCMLConfigurator_API Enum_Edit_Control_Select_Listener
{
public:
  /// When a select event occurs this method is called.
  virtual void enum_edit_selected(std::string value, void* client_data) = 0;
};
    
/// The classes which listens the focus gain/lost events should be derived
/// from this class.
class OCMLConfigurator_API Enum_Edit_Control_Focus_Listener
{
public:
  /// When a focus gain event occurs this method is called.
  virtual void enum_edit_focus_gain(EnumEditControl* control) = 0;
  /// When a focus lost event occurs this method is called.
  virtual void enum_edit_focus_lost(EnumEditControl* control) = 0;
};

/**
 * Enum edit control is a Combo Box which provides selection & Focus listener
 * interfaces. When a click event or focus event occurs, calls the registered
 * listeners.
 */
class OCMLConfigurator_API EnumEditControl: public wxComboBox
{
public:
  /// WXWindow select event handler.
  void on_select(wxCommandEvent&);
  /// WXWindow focus gain event handler.
  void on_focus_gain(wxFocusEvent&);
  /// WXWindow focus lost event handler.
  void on_focus_lost(wxFocusEvent&);

public:
  /// Constructor.
  EnumEditControl(wxWindow* parent);

  /// Registers the given SelectListener as a select listener for this widget.
  void add_select_listener(Enum_Edit_Control_Select_Listener* l);
  /// Registers the given FocusListener as a focus listener for this widget.
  void add_focus_listener(Enum_Edit_Control_Focus_Listener* l);
    
  /// Disables events so that no event handlers are called.
  void disable_events();
  /// Enables events which are disabled with disable_events call.
  void enable_events();

private:
  /// When false no event handlers are called.
  bool events_enabled_;

  /// The container for the select listeners.
  std::list<Enum_Edit_Control_Select_Listener*> select_listeners_;
  /// The container for the focus listeners.
  std::list<Enum_Edit_Control_Focus_Listener*> focus_listeners_;

  // Declares WXWindow event table.
  DECLARE_EVENT_TABLE()
};    

#endif // ENUM_EDIT_CONTROL_HPP
