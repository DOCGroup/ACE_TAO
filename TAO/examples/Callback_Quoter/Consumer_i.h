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

#if !defined (CONSUMER_I_H)
#define CONSUMER_I_H

#include "ConsumerS.h"

class Consumer_i : public POA_Callback_Quoter::Consumer
{
  // = TITLE
  //    Consumer object implementation
  //
  // = DESCRIPTION
  //    This class has methods that are called by the callback quoter server.
public:
  // = Initialization and termination methods.
  Consumer_i (void);
  // Constructor.

  ~Consumer_i (void);
  // Destructor.

  void push (const Callback_Quoter::Info & data,
	     CORBA_Environment& TAO_TRY_ENV);
  // gets the stock information from the supplier.

  virtual void shutdown (CORBA::Environment &TAO_TRY_ENV);
  // Called when the supplier-server is going away.  The client
  // implementation should shutdown the consumer-client in response to
  // this.

  void orb (CORBA::ORB_ptr o);
  // Set the ORB pointer.

  int done_;
  // The information needed received.

private:
  CORBA::ORB_var orb_;
  // ORB pointer.
};

#endif /* CONSUMER_I_H  */
