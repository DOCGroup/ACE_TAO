// $Id$
// ============================================================================
//
// = LIBRARY
//   TAO/tests/Xt_Stopwatch/Stopwatch_server
//
// = FILENAME
//   Stopwatch.h
//
// = AUTHOR
//   Balachandran  Natarajan <bala@cs.wustl.edu>
//
// ============================================================================
#ifndef STOPWATCH_H
#define STOPWATCH_H
#include <Xm/Xm.h>
#include <Xm/Frame.h>
#include <Xm/Label.h>

class Stopwatch_display
{
  //  friend Control; // Let Control call protected Stopwatch_display functions

  public:
  
  Stopwatch_display (Widget &parent);
  // Stopwatch_display constructor
  
  virtual ~Stopwatch_display();
  // Destructor

  virtual void manage();   
  // Manage the widget

  //virtual void unmanage();
  // Unmanage the widget
    
 private:
  Widget frame_;
  // A frame widget...
  Widget label_;
  // Label Widget that is used to display time..
  
 protected:
    
  //  virtual void timerStarted();   // Subclass hooks called when
  //virtual void timerStopped();   // timer starts and stops
  //float elapsedTime() { return _timer->elapsedTime(); }
};
#endif
