// $Id$

#include "EventSequenceConsumer_i.h"
#include "tao/PortableServer/PS_CurrentC.h"
#include <iostream>

EventSequenceConsumer_i::EventSequenceConsumer_i(CORBA::ORB_ptr orb)
: orb_(CORBA::ORB::_duplicate(orb))
{
}

void
EventSequenceConsumer_i::push_structured_events (
  const CosNotification::EventBatch& events
  )
{

  std::cout << "events received " << std::endl;

  const char* value = 0;

  for (unsigned int n = 0; n < events.length(); ++n) {
    for (unsigned int i = 0; i < events[n].filterable_data.length(); ++i) {
      events[n].filterable_data[i].value >>= value;
      std::cout << events[n].filterable_data[i].name.in() << "\t" << value << std::endl;
    }
  }
}

void
EventSequenceConsumer_i::disconnect_sequence_push_consumer ()
{

  CORBA::Object_var obj = orb_->resolve_initial_references ("POACurrent");
  PortableServer::Current_var current =
    PortableServer::Current::_narrow (obj.in());
  PortableServer::POA_var poa = current->get_POA ();
  PortableServer::ObjectId_var objectId = current->get_object_id();
  poa->deactivate_object(objectId.in());

}

void
EventSequenceConsumer_i::offer_change (
   const CosNotification::EventTypeSeq &,
   const CosNotification::EventTypeSeq &
   )
{
}
