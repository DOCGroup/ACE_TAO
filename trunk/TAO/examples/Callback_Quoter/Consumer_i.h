/* -*- C++ -*- */
// $Id$

// ===========================================================
//
// = LIBRARY
//    TAO/examples/Callback_Quoter
//
// = FILENAME
//    Consumer_i.h
//
// = DESCRIPTION
//    Defines the implementation header for the Consumer interface.
//
// = AUTHOR
//    Kirthika Parameswaran <kirthika@cs.wustl.edu>
//
// ===========================================================

#ifndef CONSUMER_I_H
#define CONSUMER_I_H
#include "ConsumerS.h"
#include "ConsumerC.h"
#include "NotifierS.h"

class Consumer_i : public POA_Callback_Quoter::Consumer
{
  // = TITLE
  //    Consumer object implementation.
  //
  // = DESCRIPTION
  //    This class has methods that are called by the callback quoter
  //    server.
public:
  // = Initialization and termination methods.
  Consumer_i (void);
  // Constructor.

  ~Consumer_i (void);
  // Destructor.

  void push (const Callback_Quoter::Info & data,
             CORBA_Environment& TAO_TRY_ENV);
  // Gets the stock information from the Notifier.

  virtual void shutdown (CORBA::Environment &TAO_TRY_ENV);
  // Used to get the consumer to shut down.

  void orb (CORBA::ORB_ptr o);
  // Set the ORB pointer.

private:
    int quit_;
  // If 1 denotes that the consumer is dead else alive.
 
  CORBA::ORB_var orb_;
  // ORB pointer.

  // @@ Please rename to Notifier.
  Notifier_var server_;
  // Smart pointer to the Notifier object.
};

#endif /* CONSUMER_I_H  */
