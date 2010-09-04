// $Id$

#include "tao/TransportCurrent/Transport_Current.h"
#include "tao/SystemException.h"

using namespace TAO;

/// Test referencing the TC data outside of the context of an upcall,
/// or a client-side interceptor

int
test_transport_current (Transport::Current_ptr tc)
{
  CORBA::Long id = tc->id ();

  TAO::CounterT bs = tc->bytes_sent ();

  TAO::CounterT br = tc->bytes_received ();

  TAO::CounterT rs = tc->messages_sent ();

  TAO::CounterT rr = tc->messages_received ();

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("Tester (%P|%t) Transport [%q] - Sent/Received [bytes=%q/%q, messages=%q/%q]\n"),
              (ACE_UINT64)id,
              (ACE_UINT64)bs,
              (ACE_UINT64)br,
              (ACE_UINT64)rs,
              (ACE_UINT64)rr));

  return 0;
}

int
test_transport_current (CORBA::ORB_ptr orb)
{
  // Get the Current object.
  CORBA::Object_var tcobject =
    orb->resolve_initial_references ("TAO::Transport::Current");

  Transport::Current_var tc =
    Transport::Current::_narrow (tcobject.in ());

  if (CORBA::is_nil (tc.in ()))
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("(%P|%t) client - ERROR: Could not resolve ")
                  ACE_TEXT ("TAOTransportCurrent object.\n")));

      throw CORBA::INTERNAL ();
    }

  return test_transport_current (tc.in ());
}
