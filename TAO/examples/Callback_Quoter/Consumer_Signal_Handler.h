/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    Consumer_Input_Handler.h
 *
 *  $Id$
 *
 *  Definition of the Consumer_Signal_Handler class.
 *
 *
 *  @author Kirthika Parameswaran <kirthika@cs.wustl.edu>
 */
//=============================================================================


#ifndef CONSUMER_SIGNAL_HANDLER_H
#define CONSUMER_SIGNAL_HANDLER_H

#include "ace/Reactor.h"
#include "ace/Event_Handler.h"
#include "Consumer_Handler.h"

class Consumer_Handler;

/**
 * @class Consumer_Signal_Handler
 *
 * @brief Creating a class to handle signal events.
 *
 * Since only signals need to be handled, only the <handle_signal> method
 * is overlaoded.
 */
class Consumer_Signal_Handler : public ACE_Event_Handler
{
public:

  /// The consumer_handler refernce will be used to access the servant
  /// methods.
  Consumer_Signal_Handler (Consumer_Handler *consumer_handler);

  /// This method takes action on an signal event.
  int handle_signal (int signum,
                     siginfo_t*,
                     ucontext_t*);

  /**
   * For removal of the signal handler from the dispatch tables.  When
   * the handle_signal () returns < 0 this method will be executed
   * automatically.
   */
  int handle_close (ACE_HANDLE handle,
                    ACE_Reactor_Mask close_mask);

protected:
  /// Protected destructor so that the signal handler is always created
  /// dynamically and hence the heap doesnt get corrupted.
  ~Consumer_Signal_Handler (void);

private:
  /// Exit gracefully on a signal.
  int quit_on_signal (void);

  /// Reference to the Consumer_Handler which is used in accessing the
  /// servant methods.
  Consumer_Handler *consumer_handler_;
};

#endif /* CONSUMER_SIGNAL_HANDLER_H */
