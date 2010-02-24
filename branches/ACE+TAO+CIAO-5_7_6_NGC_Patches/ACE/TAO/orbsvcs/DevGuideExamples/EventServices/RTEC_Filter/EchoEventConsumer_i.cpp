// $Id$

// EchoEventConsumer_i.cpp
// Implements a PushConsumer.

#include "EchoEventConsumer_i.h"
#include "tao/PortableServer/PS_CurrentC.h"
#include "ace/OS_NS_stdio.h"
#include <sstream>

// Constructor duplicates the ORB reference.
EchoEventConsumer_i::EchoEventConsumer_i(
                       CORBA::ORB_ptr orb,
                       RtecEventChannelAdmin::ProxyPushSupplier_ptr supplier,
                       int event_limit)
  : orb_(CORBA::ORB::_duplicate(orb))
  , supplier_(RtecEventChannelAdmin::ProxyPushSupplier::_duplicate(supplier))
  , event_limit_(event_limit)
{
  // Nothing to do.
}

// Implement the push() operation.
void EchoEventConsumer_i::push(const RtecEventComm::EventSet& events)
{
  // Loop through the events, looking for shutdown events.
  for (u_int i = 0; i < events.length (); ++i) {
    //ACE_OS::printf(".");
    // Extract event data from the any.
    const char* eventData;
    std::ostringstream out;
    out << "Received event,"
         << "  type: "   << events[i].header.type
         << "  source: " << events[i].header.source;
    if (events[i].data.any_value >>= eventData) {
      out << "  text: "   << eventData;
    }

    ACE_OS::printf("%s\n", out.str().c_str()); // printf is synchronized
  }
  if (--event_limit_ <= 0) {
    supplier_->disconnect_push_supplier();
    orb_->shutdown(0);
  }
}

// Implement the disconnect_push_consumer() operation.
void EchoEventConsumer_i::disconnect_push_consumer()
{
  // Deactivate this object.
  CORBA::Object_var obj = orb_->resolve_initial_references("POACurrent");
  PortableServer::Current_var current = PortableServer::Current::_narrow(obj.in());
  PortableServer::POA_var poa = current->get_POA();
  PortableServer::ObjectId_var objectId = current->get_object_id();
  poa->deactivate_object(objectId.in());
}
