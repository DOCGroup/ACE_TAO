// $Id$

#include "Direct_Consumer.h"

ACE_RCSID(lib, TAO_Direct_Consumer, "$id$")

#include "LookupManager.h"

TAO_Notify_Tests_Direct_Consumer::TAO_Notify_Tests_Direct_Consumer (void)
{
}

TAO_Notify_Tests_Direct_Consumer::~TAO_Notify_Tests_Direct_Consumer ()
{
}

void
TAO_Notify_Tests_Direct_Consumer::connect (ACE_ENV_SINGLE_ARG_DECL)
{
  // Get the POA
  PortableServer::POA_var poa;
  LOOKUP_MANAGER->resolve (poa, this->poa_name_.c_str () ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  // set the POA
  this->set_poa (poa.in () ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  // Activate the consumer with the default_POA_.
  CosNotifyComm::StructuredPushConsumer_var consumer_ref =
    this->_this (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  // Register the activated object.
  LOOKUP_MANAGER->_register (consumer_ref.in (), this->name_.c_str () ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}
