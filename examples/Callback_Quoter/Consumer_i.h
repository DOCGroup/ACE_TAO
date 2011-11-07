/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    Consumer_i.h
 *
 *  $Id$
 *
 *  Defines the implementation header for the Consumer interface.
 *
 *
 *  @author Kirthika Parameswaran <kirthika@cs.wustl.edu>
 */
//=============================================================================


#ifndef CONSUMER_I_H
#define CONSUMER_I_H
#include "ConsumerS.h"
#include "ConsumerC.h"
#include "NotifierS.h"

/**
 * @class Consumer_i
 *
 * @brief Consumer object implementation.
 *
 * This class has methods that are called by the callback quoter
 * server.
 */
class Consumer_i : public POA_Callback_Quoter::Consumer
{
public:
  // = Initialization and termination methods.
  /// Constructor.
  Consumer_i (void);

  /// Destructor.
  ~Consumer_i (void);

  /// Gets the stock information from the Notifier.
  void push (const Callback_Quoter::Info & data);

  /// Used to get the consumer to shut down.
  virtual void shutdown (void);

  /// Set the ORB pointer.
  void orb (CORBA::ORB_ptr o);

private:
  /// ORB pointer.
  CORBA::ORB_var orb_;

  /// If 1 denotes that the consumer is dead else alive.
  int quit_;

  // @@ Please rename to Notifier.
  /// Smart pointer to the Notifier object.
  Notifier_var server_;
};

#endif /* CONSUMER_I_H  */
