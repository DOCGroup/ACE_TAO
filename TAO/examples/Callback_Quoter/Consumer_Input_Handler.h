/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    Consumer_Input_Handler.h
 *
 *  Definition of the Callback_Qouter Consumer Client class, Consumer_Input_Handler.
 *
 *  @author Kirthika Parameswaran <kirthika@cs.wustl.edu>
 */
//=============================================================================


#ifndef CONSUMER_INPUT_HANDLER_H
#define CONSUMER_INPUT_HANDLER_H

#include "ace/Reactor.h"
#include "ace/Event_Handler.h"
#include "Consumer_Handler.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

// Creating a class to handle input events.
// Since only inputs need to be handled, only the handle_input
// method is overlaoded.
class Consumer_Handler;

/**
 * @class Consumer_Input_Handler
 *
 * @brief Callback Quoter Consumer Client class.
 *
 * Connects to the Callback Quoter server and
 * registers the Consumer object with the it
 * and receives the stock status from the Notifier.
 */
class Consumer_Input_Handler : public ACE_Event_Handler
{
public:
  /// Constructor.
  Consumer_Input_Handler (Consumer_Handler *consumer_handler);

  /// Handle the user input.
  int handle_input (ACE_HANDLE) override;

  /// Registration with the notifier.
  int register_consumer ();

  /// Cancelling the registration with the notifier.
  int unregister_consumer ();

  /// Ends the consumer process.
  int quit_consumer_process ();

  ///   A set of values for the execution of the consumer.
  ///
  ///   Used so that the process of registering, unregistering
  ///   and exiting neednt be dependent on 'r' 'u' and 'q'.
  ///   Also, #define clutters up the global namespace.
enum
  {
    /// The character that the user must type to register the consumer with
    /// the Notifier_server.
    REGISTER = 'r',

    /// The character that the user must type to unregister the consumer with
    /// the Notifier_server.
    UNREGISTER = 'u',

    /// The character the user must type to quit the consumer client
    /// application.
    EXIT = 'q'
  };

protected:
  /// Destructor.
  ~Consumer_Input_Handler () = default;

private:
  /// The Consumer_Handler object.
  Consumer_Handler *consumer_handler_;
};

#endif /* CONSUMER_INPUT_HANDLER_H */
