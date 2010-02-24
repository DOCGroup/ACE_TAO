// $Id$

// Consumer_i.h
// Implements a TypedPushConsumer.

#ifndef _Consumer_i_h_
#define _Consumer_i_h_

#include "orbsvcs/CosTypedEventCommS.h"// for POA_CosTypedEventComm::TypedPushConsumer

class Consumer_i
: public virtual POA_CosTypedEventComm::TypedPushConsumer
{
  public:
    // Constructor
    Consumer_i(CORBA::ORB_ptr orb,
               CORBA::Object_ptr obj);

    // Override operations from TypedPushConsumer interface.
    virtual CORBA::Object_ptr get_typed_consumer ();

    virtual void push(const CORBA::Any & data) ;

    virtual void disconnect_push_consumer();

  private:
    CORBA::ORB_var orb_;
    CORBA::Object_var object_;
};

#endif // _Consumer_i_h_
