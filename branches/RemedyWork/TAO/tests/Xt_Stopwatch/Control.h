
//=============================================================================
/**
 *  @file   Control.h
 *
 *  $Id$
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
  virtual ~Control (void);

  /// Manage the widget
  virtual void manage (void);

  /// Return the start widget
  Widget& startwidget (void);

  /// Return the stop widget..
  Widget& stopwidget (void);

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
