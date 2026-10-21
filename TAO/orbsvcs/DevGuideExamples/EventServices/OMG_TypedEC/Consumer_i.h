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
    Consumer_i (Consumer_i const &) = delete;
    Consumer_i (Consumer_i &&) = delete;
    Consumer_i &operator= (Consumer_i const &) = delete;
    Consumer_i &operator= (Consumer_i &&) = delete;

    CORBA::Object_ptr get_typed_consumer () override;

    void push (CORBA::Any const & data) override;

    void disconnect_push_consumer () override;

  private:
    CORBA::ORB_var orb_;
    CORBA::Object_var object_;
};

#endif // _Consumer_i_h_
