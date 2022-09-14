/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    Consumer_i.h
 *
 *  Defines the implementation header for the Consumer interface.
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
  /// Constructor.
  Consumer_i () = default;

  /// Destructor.
  ~Consumer_i () = default;

  /// Gets the stock information from the Notifier.
  void push (const Callback_Quoter::Info & data) override;

  /// Used to get the consumer to shut down.
  void shutdown () override;

  /// Set the ORB pointer.
  void orb (CORBA::ORB_ptr o);

private:
  /// ORB pointer.
  CORBA::ORB_var orb_;

  // @@ Please rename to Notifier.
  /// Smart pointer to the Notifier object.
  Notifier_var server_;
};

#endif /* CONSUMER_I_H  */
