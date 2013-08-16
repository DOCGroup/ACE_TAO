// $Id$

#include "ace/OS_NS_sys_time.h"
#include "PushConsumer.h"
#include "orbsvcs/FtRtEvent/Utils/resolve_init.h"
#include "ace/OS_NS_stdio.h"

PushConsumer_impl::PushConsumer_impl(CORBA::ORB_ptr orb)
: orb_(CORBA::ORB::_duplicate(orb))
{
}


void
PushConsumer_impl::push (const RtecEventComm::EventSet & event
             )
{
  CORBA::ULong x;
  ACE_Time_Value time_val = ACE_OS::gettimeofday ();

  if (event.length() >0) {
    TimeBase::TimeT elaps =
      time_val.sec () * 10000000 + time_val.usec ()* 10 - event[0].header.ec_send_time;
    event[0].data.any_value >>= x;
    ACE_OS::printf("Received data : %d,  single trip time = %d usec\n", x, static_cast<int> (elaps/10));
  }
}


void
PushConsumer_impl::disconnect_push_consumer (void)
{
  PortableServer::Current_var current =
    resolve_init<PortableServer::Current>(orb_.in(), "POACurrent");

  PortableServer::POA_var poa = current->get_POA();

  PortableServer::ObjectId_var oid = current->get_object_id();

  poa->deactivate_object(oid.in ());
}
