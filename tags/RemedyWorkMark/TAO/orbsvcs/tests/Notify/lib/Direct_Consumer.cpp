// $Id$

#include "Direct_Consumer.h"



#include "LookupManager.h"

TAO_Notify_Tests_Direct_Consumer::TAO_Notify_Tests_Direct_Consumer (void)
{
}

TAO_Notify_Tests_Direct_Consumer::~TAO_Notify_Tests_Direct_Consumer ()
{
}

void
TAO_Notify_Tests_Direct_Consumer::connect (void)
{
  // Get the POA
  PortableServer::POA_var poa;
  LOOKUP_MANAGER->resolve (poa, this->poa_name_.c_str ());

  // set the POA
  this->set_poa (poa.in ());

  // Activate the consumer with the default_POA_.
  CosNotifyComm::StructuredPushConsumer_var consumer_ref =
    this->_this ();

  // Register the activated object.
  LOOKUP_MANAGER->_register (consumer_ref.in (), this->name_.c_str ());
}
