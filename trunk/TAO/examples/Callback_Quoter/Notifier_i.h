/* -*- C++ -*- */
// $Id$

// ===========================================================
//
// = LIBRARY
//    TAO/examples/Callback_Quoter
//
// = FILENAME
//    Notifier.h
//
// = DESCRIPTION
//    Defines the implementation header for the Supplier interface.
//
// = AUTHOR
//    Kirthika Parameswaran <kirthika@cs.wustl.edu>
//
// ===========================================================

#if !defined (NOTIFIER_I_H)
#define NOTIFIER_I_H

#include "NotifierS.h"
#include "ConsumerC.h"
#include "tao/TAO.h"
#include "ace/Containers.h"
#include "ace/SString.h"

class Notifier_i : public POA_Notifier
{
  // = TITLE
  //    The implementation of the Notifier class, which is the servant
  //    object for the callback quoter server.
public:
  // = Initialization and termination methods.
  Notifier_i (void);
  // Constructor.

  ~Notifier_i (void);
  // Destructor.

  virtual void register_callback (const char *stock_name,
			          CORBA::Long threshold_value,
			          Callback_Quoter::Consumer_ptr consumer_handler,
				  CORBA::Environment &TAO_TRY_ENV);
  // Register a distributed callback handler that is invoked when the
  // given stock reaches the desired threshold value.

  virtual void unregister_callback (Callback_Quoter::Consumer_ptr consumer_handler,
				    CORBA::Environment &TAO_TRY_ENV);
  // Remove the consumer object.

  virtual void market_status (const char *stock_name,
			      CORBA::Long stock_value,
			      CORBA::Environment &TAO_TRY_ENV);
  // Get the market status.

  void orb (CORBA::ORB_ptr orb);
  // Get the orb pointer.

  virtual void shutdown (CORBA::Environment &);
  // Shutdown the Notifier.

  // CONSUMER_MAP* get_consumer_map_ptr ();
  // Returns the consumer map ptr.

  //private:
public:
  CORBA::ORB_var orb_;
  // The ORB manager.

  class Consumer_Data
  {
    // = TITLE
    //   Saves the Consumer_var and the threshold stock value.
  public:
    int operator== (const Consumer_Data &rhs);
    // Comparison operator.

    Callback_Quoter::Consumer_var consumer_;
    // Stores the client reference.

    CORBA::Long desired_value_;
    // Stores the stock threshold value.
  };

  typedef ACE_Unbounded_Set<Consumer_Data>
          CONSUMERS;

  typedef ACE_Hash_Map_Manager<ACE_CString, CONSUMERS *, ACE_Null_Mutex>
          CONSUMER_MAP;

  CONSUMER_MAP consumer_map_;
  // @@ Please add a comment.
};

#endif /* NOTIFIER_I_H */
