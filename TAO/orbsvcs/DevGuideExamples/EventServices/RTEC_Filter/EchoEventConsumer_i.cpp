// EchoEventConsumer_i.cpp
// Implements a PushConsumer.

#include "EchoEventConsumer_i.h"
#include "tao/PortableServer/PS_CurrentC.h"
#include "ace/OS_NS_stdio.h"

#include <sstream>

EchoEventConsumer_i::EchoEventConsumer_i (
  CORBA::ORB_ptr orb,
  RtecEventChannelAdmin::ProxyPushSupplier_ptr supplier,
  int event_limit)
  : orb_ (CORBA::ORB::_duplicate (orb)),
    supplier_ (RtecEventChannelAdmin::ProxyPushSupplier::_duplicate (supplier)),
    event_limit_ (event_limit)
{
}

// Implement the push() operation.
void
EchoEventConsumer_i::push (RtecEventComm::EventSet const &events)
{
  for (CORBA::ULong i = 0; i < events.length (); ++i)
    {
      char const *event_data {};
      std::ostringstream out;
      out << "Received event,"
          << "  type: "   << events[i].header.type
          << "  source: " << events[i].header.source;
      if (events[i].data.any_value >>= event_data)
        {
          out << "  text: " << event_data;
        }

      ACE_OS::printf ("%s\n", out.str ().c_str ()); // ACE_OS::printf is synchronized.
    }

  if (--this->event_limit_ <= 0)
    {
      this->supplier_->disconnect_push_supplier ();
      this->orb_->shutdown (0);
    }
}

// Implement the disconnect_push_consumer() operation.
void EchoEventConsumer_i::disconnect_push_consumer ()
{
  // Deactivate this object.
  CORBA::Object_var const obj = this->orb_->resolve_initial_references ("POACurrent");
  PortableServer::Current_var const current = PortableServer::Current::_narrow (obj.in ());
  PortableServer::POA_var const poa = current->get_POA ();
  PortableServer::ObjectId_var const object_id = current->get_object_id ();
  poa->deactivate_object (object_id.in ());
}
