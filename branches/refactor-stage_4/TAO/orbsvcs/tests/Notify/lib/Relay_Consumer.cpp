// $Id$

#include "Relay_Consumer.h"

ACE_RCSID(Notify, TAO_NS_Relay_Consumer, "$id$")

#include "LookupManager.h"

TAO_NS_Relay_Consumer::TAO_NS_Relay_Consumer (ACE_CString& destination)
  : destination_ (destination)
{
}

TAO_NS_Relay_Consumer::~TAO_NS_Relay_Consumer ()
{
}

void
TAO_NS_Relay_Consumer::connect (ACE_ENV_SINGLE_ARG_DECL)
{
  TAO_NS_Direct_Consumer::connect (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  // Resolve the destination object.
  LOOKUP_MANAGER->resolve (this->destination_object_, this->destination_.c_str () ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

void
TAO_NS_Relay_Consumer::push_structured_event (const CosNotification::StructuredEvent &notification
                                              ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   CosEventComm::Disconnected))
{
  // The Periodic Consumer will record the run statistics.
  //TAO_NS_Periodic_Consumer::push_structured_event (notification ACE_ENV_ARG_PARAMETER);
  //ACE_CHECK;

  // Forward the event.
  if (CORBA::is_nil (this->destination_object_.in ()))
    ACE_DEBUG ((LM_DEBUG, "(%P,%t) Cannot Relay - Destination object %s is nil", this->destination_.c_str ()));
  else
    this->destination_object_->push_structured_event (notification ACE_ENV_ARG_PARAMETER);
}
