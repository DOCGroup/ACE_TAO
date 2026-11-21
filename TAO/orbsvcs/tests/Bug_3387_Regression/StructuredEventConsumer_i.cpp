#include "StructuredEventConsumer_i.h"
#include "tao/PortableServer/PS_CurrentC.h"
#include <iostream>

int consumerStop();

StructuredEventConsumer_i::StructuredEventConsumer_i (CORBA::ORB_ptr orb)
  : orb_ (CORBA::ORB::_duplicate (orb))
{
}

void
StructuredEventConsumer_i::push_structured_event (
  CosNotification::StructuredEvent const &event)
{
  std::cout << "event received " << std::endl;

  char const *value {};

  for (CORBA::ULong i = 0; i < event.filterable_data.length (); ++i)
    {
      event.filterable_data[i].value >>= value;
      std::cout << event.filterable_data[i].name.in () << "\t" << value << std::endl;
    }
  ++count_;
  if (count_ >= 3)
    {
      consumerStop ();
    }
}

void
StructuredEventConsumer_i::disconnect_structured_push_consumer ()
{
  CORBA::Object_var const obj = orb_->resolve_initial_references ("POACurrent");
  PortableServer::Current_var const current =
    PortableServer::Current::_narrow (obj.in ());
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

int
StructuredEventConsumer_i::nummsgs () const
{
  return count_;
}
