#include "StructuredEventConsumer_i.h"
#include "tao/PortableServer/PS_CurrentC.h"
#include <iostream>

StructuredEventConsumer_i::StructuredEventConsumer_i (CORBA::ORB_ptr orb)
  : orb_ (CORBA::ORB::_duplicate (orb))
{
}

void
StructuredEventConsumer_i::push_structured_event (
  CosNotification::StructuredEvent const &event)
{
  char const *value {};
  CORBA::ULong const length = event.filterable_data.length ();
  for (CORBA::ULong i {}; i < length; ++i)
    {
      if (event.filterable_data[i].value >>= value)
        std::cout << event.filterable_data[i].name << "\t" << value << std::endl;
    }

  std::cerr << "MessengerConsumer: success" << std::endl;
}

void
StructuredEventConsumer_i::disconnect_structured_push_consumer ()
{
  CORBA::Object_var const obj = orb_->resolve_initial_references ("POACurrent");
  PortableServer::Current_var const current = PortableServer::Current::_narrow (obj.in ());
  PortableServer::POA_var const poa = current->get_POA ();
  PortableServer::ObjectId_var const object_id = current->get_object_id ();
  poa->deactivate_object (object_id.in ());
}

void
StructuredEventConsumer_i::offer_change (
  CosNotification::EventTypeSeq const &,
  CosNotification::EventTypeSeq const &)
{
}
