/* -*- C++ -*- */
// $Id$

// ===========================================================
//
// = LIBRARY
//    TAO/examples/Callback_Quoter
//
// = FILENAME
//    Supplier_i.h
//
// = DESCRIPTION
//    Defines the implementation header for the Supplier interface.
//
// = AUTHOR
//    Kirthika Parameswaran <kirthika@cs.wustl.edu>
//
// ===========================================================

#if !defined (SUPPLIER_I_H)
#define SUPPLIER_I_H

#include "SupplierS.h"

#include "ConsumerC.h"


#include "tao/TAO.h"
#include "ace/Containers.h"
#include "ace/SString.h"

class Supplier_i : public POA_Supplier
{
  // = TITLE
  //    The implementation of the Supplier class, which is the
  //    servant object for the callback  quoter server.
public:
  // = Initialization and termination methods.
  Supplier_i (void);
  // Constructor.

  ~Supplier_i (void);
  // Destructor.

  // Register a distributed callback handler that is invoked
  // when the given stock reaches the desired threshold value.

  virtual void register_callback (const char *stock_name,
			          CORBA::Long threshold_value,
			          Callback_Quoter::Consumer_ptr consumer_handler,
				  CORBA::Environment &TAO_TRY_ENV);

  // Remove the handler.

  virtual void unregister_callback (Callback_Quoter::Consumer_ptr consumer_handler,
				    CORBA::Environment &TAO_TRY_ENV);
  // get market status.

  virtual void market_status (const char *stock_name,
			      CORBA::Long stock_value,
			      CORBA::Environment &TAO_TRY_ENV);

  void orb (CORBA::ORB_ptr orb);

  //shutdown the supplier.
  virtual void shutdown (CORBA::Environment &);
private:

  CORBA::ORB_var orb_;
  // The ORB manager.

  // Saves the Consumer_var and the threshold stock value.
  struct Consumer_Data
  {
    int operator== (const Consumer_Data &rhs);
    // Comparison

    Callback_Quoter::Consumer_var consumer_;
    // Stores the client reference.

    CORBA::Long desired_value_;
    // Stores the stock threshold value.
  };

  typedef ACE_Unbounded_Set<Consumer_Data> CONSUMERS;
  typedef ACE_Hash_Map_Manager<ACE_CString, CONSUMERS *, ACE_Null_Mutex> CONSUMER_MAP;

  CONSUMER_MAP consumer_map_;
};

#endif /* SUPPLIER_I_H */
