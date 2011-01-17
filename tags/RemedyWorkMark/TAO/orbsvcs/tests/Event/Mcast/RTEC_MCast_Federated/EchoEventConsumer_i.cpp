// $Id$

// EchoEventConsumer_i.cpp
// Implements a PushConsumer.

#include "EchoEventConsumer_i.h"
#include "tao/PortableServer/PS_CurrentC.h"
#include "ace/OS_NS_stdio.h"
#include "ace/OS_NS_unistd.h"

#include <sstream>

// Constructor duplicates the ORB reference.
EchoEventConsumer_i::EchoEventConsumer_i(CORBA::ORB_ptr orb, int event_limit)
  : orb_(CORBA::ORB::_duplicate(orb))
  , event_limit_(event_limit)
{
  // Nothing to do.
}

// Implement the push() operation.
void EchoEventConsumer_i::push(const RtecEventComm::EventSet& events)
{
  // Loop through the events, looking for shutdown events.
  for (u_int i = 0; i < events.length (); ++i)
    {
      //ACE_OS::printf(".");
      // Extract event data from the any.
      const char* eventData;
      std::ostringstream out;

#ifndef ACE_LACKS_GETPID
      out << "[" << ACE_OS::getpid();
#endif
      out << "] Received event,"
          << "  type: "   << events[i].header.type
          << "  source: " << events[i].header.source;

#if !defined (TAO_LACKS_EVENT_CHANNEL_ANY)
      if (events[i].data.any_value >>= eventData)
        {
          out << "  text: "   << eventData;
        }
#else
      if (events[i].data.payload.length() > 0)
        {
          out << "  text: " <<
            (const char *)events[i].data.payload.get_buffer();
        }
#endif  /* !TAO_LACKS_EVENT_CHANNEL_ANY */
      ACE_OS::printf("%s\n", out.str().c_str()); // printf is synchronized
    }
  if (--event_limit_ <= 0)
    {
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
