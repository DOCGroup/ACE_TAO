
//=============================================================================
/**
 *  @file   Control.h
 *
 *  @author Balachandran  Natarajan <bala@cs.wustl.edu>
 */
//=============================================================================


#ifndef CONTROL_H
#define CONTROL_H
#include "Client.h"


#include <Xm/Xm.h>
#include <Xm/PushB.h>
#include <Xm/Label.h>
#include <Xm/RowColumn.h>
#include <Xm/Frame.h>


class Control
{
 public:
  /// Stopwatch constructor
  Control (Widget &parent);

  /// Destructor
  virtual ~Control ();

  /// Manage the widget
  virtual void manage ();

  /// Return the start widget
  Widget& startwidget ();

  /// Return the stop widget..
  Widget& stopwidget ();

 private:
  /// A frame widget...
  Widget frame_;

  /// A row column widget..
  Widget rowcolumn_;

  /// Pushbutton  Widget that is used to start the  timer..
  Widget startwidget_;

  /// Pushbutton widget that is used to stop the timer..
  Widget stopwidget_;
};


#endif /* CONTROL_H */
