// $Id$

#include "StructuredEventConsumer_i.h"
#include "tao/PortableServer/PS_CurrentC.h"
#include <iostream>

int consumerStop();

StructuredEventConsumer_i::StructuredEventConsumer_i(CORBA::ORB_ptr orb)
  : orb_(CORBA::ORB::_duplicate(orb)), count_(0)
{
}

void
StructuredEventConsumer_i::push_structured_event(
       const CosNotification::StructuredEvent &event
      )
{

  std::cout << "event received " << std::endl;

  const char *value;

  for (unsigned int i=0; i<event.filterable_data.length(); i++) {
    event.filterable_data[i].value >>= value;
    std::cout << event.filterable_data[i].name.in() << "\t" << value << std::endl;
  }
  count_++;
  if ( count_ >= 3 )
  {
    consumerStop();
  }

}

void
StructuredEventConsumer_i::disconnect_structured_push_consumer()
{

   CORBA::Object_var obj = orb_->resolve_initial_references ("POACurrent");
   PortableServer::Current_var current =
                          PortableServer::Current::_narrow (obj.in());
   PortableServer::POA_var poa = current->get_POA ();
   PortableServer::ObjectId_var objectId = current->get_object_id ();
   poa->deactivate_object (objectId.in());

}

void
StructuredEventConsumer_i::offer_change(
        const CosNotification::EventTypeSeq &,
        const CosNotification::EventTypeSeq &
      )
{
}

int
StructuredEventConsumer_i::nummsgs()
{
    return count_;
}
