// EchoEventConsumer_i.cpp
// Implements a PushConsumer.

#include "EchoEventConsumer_i.h"
#include "tao/PortableServer/PS_CurrentC.h"
#include "ace/OS_NS_unistd.h"

#include <cstdio>
#include <sstream>

EchoEventConsumer_i::EchoEventConsumer_i (CORBA::ORB_ptr orb, int event_limit)
  : orb_ (CORBA::ORB::_duplicate (orb)),
    event_limit_ (event_limit)
{
}

// Implement the push() operation.
void EchoEventConsumer_i::push (RtecEventComm::EventSet const &events)
{
  // Loop through the events, looking for shutdown events.
  for (CORBA::ULong i {}; i < events.length (); ++i)
    {
      // Extract event data from the any.
      char const *event_data {};
      std::ostringstream out;

#ifndef ACE_LACKS_GETPID
      out << "[" << ACE_OS::getpid();
#endif
      out << "] Received event,"
          << "  type: "   << events[i].header.type
          << "  source: " << events[i].header.source;

#if !defined (TAO_LACKS_EVENT_CHANNEL_ANY)
      if (events[i].data.any_value >>= event_data)
        {
          out << "  text: "   << event_data;
        }
#else
      if (events[i].data.payload.length() > 0)
        {
          out << "  text: " <<
            reinterpret_cast<char const *> (events[i].data.payload.get_buffer ());
        }
#endif  /* !TAO_LACKS_EVENT_CHANNEL_ANY */
      std::printf ("%s\n", out.str ().c_str ()); // printf is synchronized
    }
  if (--event_limit_ <= 0)
    {
      this->orb_->shutdown (false);
    }
}

// Implement the disconnect_push_consumer() operation.
void EchoEventConsumer_i::disconnect_push_consumer ()
{
  // Deactivate this object.
  CORBA::Object_var obj = this->orb_->resolve_initial_references ("POACurrent");
  PortableServer::Current_var current = PortableServer::Current::_narrow (obj.in ());
  PortableServer::POA_var poa = current->get_POA ();
  PortableServer::ObjectId_var object_id = current->get_object_id ();
  poa->deactivate_object (object_id.in ());
}
