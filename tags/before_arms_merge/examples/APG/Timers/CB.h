/* -*- C++ -*- */
// $Id$

#if !defined(CB_H)
#define CB_H

#include "ace/Event_Handler.h"

#include "TimerDispatcher.h"

// Listing 1 code/ch20
class CB : public ACE_Event_Handler
{
public:
  CB ();

  // Set the timer id that is being handled by this instance.
  void setID (long timerID);

  // Get the timer id.
  long getID (void);

  // Handle the timeout.
  virtual int handle_timeout(const ACE_Time_Value &tv, 
			     const void *arg = 0);

  virtual int handle_close (ACE_HANDLE handle,
                            ACE_Reactor_Mask close_mask);

private:
  long timerID_;
  int count_;
};
// Listing 1

#endif /*CB_H*/
