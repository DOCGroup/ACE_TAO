#include "EventSequenceConsumer_i.h"
#include "tao/PortableServer/PS_CurrentC.h"

#include <iostream>

EventSequenceConsumer_i::EventSequenceConsumer_i (CORBA::ORB_ptr orb)
  : orb_ (CORBA::ORB::_duplicate (orb))
{
}

void
EventSequenceConsumer_i::push_structured_events (CosNotification::EventBatch const &events)
{
  std::cout << "events received " << std::endl;

  char const *value {};

  for (CORBA::ULong n {}; n < events.length (); ++n)
    for (CORBA::ULong i {}; i < events[n].filterable_data.length (); ++i)
      {
        events[n].filterable_data[i].value >>= value;
        std::cout << events[n].filterable_data[i].name.in () << "\t" << value << std::endl;
      }
}

void
EventSequenceConsumer_i::disconnect_sequence_push_consumer ()
{
  CORBA::Object_var const obj = orb_->resolve_initial_references ("POACurrent");
  PortableServer::Current_var current =
    PortableServer::Current::_narrow (obj.in ());
  PortableServer::POA_var const poa = current->get_POA ();
  PortableServer::ObjectId_var const object_id = current->get_object_id ();
  poa->deactivate_object (object_id.in ());
}

void
EventSequenceConsumer_i::offer_change (CosNotification::EventTypeSeq const &,
                                       CosNotification::EventTypeSeq const &)
{
}
