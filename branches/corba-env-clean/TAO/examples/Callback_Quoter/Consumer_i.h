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

  void push (const Callback_Quoter::Info & data
             TAO_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));
  // Gets the stock information from the Notifier.

  virtual void shutdown (TAO_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));
  // Used to get the consumer to shut down.

  void orb (CORBA::ORB_ptr o);
  // Set the ORB pointer.

private:
  CORBA::ORB_var orb_;
  // ORB pointer.

  int quit_;
  // If 1 denotes that the consumer is dead else alive.

  // @@ Please rename to Notifier.
  Notifier_var server_;
  // Smart pointer to the Notifier object.
};

#endif /* CONSUMER_I_H  */
