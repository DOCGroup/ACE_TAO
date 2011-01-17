// $Id$

#include "Relay_Consumer.h"



#include "LookupManager.h"

TAO_Notify_Tests_Relay_Consumer::TAO_Notify_Tests_Relay_Consumer (ACE_CString& destination)
  : destination_ (destination)
{
}

TAO_Notify_Tests_Relay_Consumer::~TAO_Notify_Tests_Relay_Consumer ()
{
}

void
TAO_Notify_Tests_Relay_Consumer::connect (void)
{
  TAO_Notify_Tests_Direct_Consumer::connect ();

  // Resolve the destination object.
  LOOKUP_MANAGER->resolve (this->destination_object_, this->destination_.c_str ());
}

void
TAO_Notify_Tests_Relay_Consumer::push_structured_event (const CosNotification::StructuredEvent &notification)
{
  // The Periodic Consumer will record the run statistics.
  //TAO_Notify_Tests_Periodic_Consumer::push_structured_event (notification);

  // Forward the event.
  if (CORBA::is_nil (this->destination_object_.in ()))
    ACE_DEBUG ((LM_DEBUG, "(%P,%t) Cannot Relay - Destination object %s is nil", this->destination_.c_str ()));
  else
    this->destination_object_->push_structured_event (notification);
}
