// $Id$

/**
 * @file Pulser.h
 *
 * Header file for the Pulser class.
 *
 * @author Nanbor Wang <nanbor@cse.wustl.edu>
 */

#ifndef PULSER_H
#define PULSER_H

#include "tao/corba.h"
#include "ace/Thread_Manager.h"
#include "ace/Task.h"

namespace PTImpl
{
  // Forward decl.
  class Pulse_Handler
  {
  public:
    /// Helper function to be called back by Pulse_Handler
    virtual void pulse (void) = 0;
  };

  /**
   * @brief Active pulse generater
   */
  class Pulser : public ACE_Task_Base
  {
  public:
    // Default constructor
    Pulser (Pulse_Handler *cb);
    ~Pulser ();

    int open (void *args = 0);

    int close (u_long flags = 0);

    int start (CORBA::Long hertz);

    int stop (void);

    int active (void);

    // Handle the timeout.
    virtual int handle_timeout (const ACE_Time_Value &tv,
                                const void *arg);

    // Called when <Time_Handler> is removed.
    virtual int handle_close (ACE_HANDLE handle,
                              ACE_Reactor_Mask close_mask);

    virtual int svc (void);

  private:
    /// Tracking whether we are actively generating pulse or not.
    long active_;

    /// Flag to indicate completion of this active object.
    int done_;

    /// The timer id we are waiting.
    int tid_;

    Pulse_Handler *pulser_callback_;

    ACE_Thread_Manager thr_mgr_;
  };

}

#endif /* PULSER_H */
