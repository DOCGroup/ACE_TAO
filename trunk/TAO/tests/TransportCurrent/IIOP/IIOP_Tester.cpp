// -*- C++ -*-

// $Id$

#include "tao/TransportCurrent/IIOP_Transport_Current.h"
#include "tao/SystemException.h"

#if defined (TAO_AS_STATIC_LIBS)
#include "tao/TransportCurrent/IIOP_Current_Loader.h"

// Create an object that will insert the <Current_Loader> into the
// list of statically linked services that the <ACE_Service_Config>
// will initialize at run-time.
ACE_STATIC_SVC_REQUIRE (TAO_Transport_Current_Loader)
ACE_STATIC_SVC_REQUIRE (TAO_Transport_IIOP_Current_Loader)
#endif /* TAO_AS_STATIC_LIBS */

using namespace TAO;

/// Test referencing the TC data outside of the context of an upcall,
/// or a client-side interceptor

int
test_transport_current (CORBA::ORB_ptr orb)
{
  // Get the Current object.
  CORBA::Object_var tcobject =
    orb->resolve_initial_references ("TAO::Transport::IIOP::Current");

  if (TAO_debug_level >= 1)
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("Tester (%P|%t) Resolved initial reference for IIOP::Current\n")));

  Transport::IIOP::Current_var tc =
    Transport::IIOP::Current::_narrow (tcobject.in ());

  if (TAO_debug_level >= 1)
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("Tester (%P|%t) Narowed the IIOP Transport Current\n")));

  if (CORBA::is_nil (tc.in ()))
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("Tester (%P|%t) ERROR: Could not resolve ")
                  ACE_TEXT ("TAO::Transport::IIOP::Current object.\n")));

      throw CORBA::INTERNAL ();
    }

  ::CORBA::String_var rhost (tc->remote_host ());

  ::CORBA::String_var lhost (tc->local_host ());

  ::CORBA::Long id = tc->id ();

  ::TAO::CounterT bs = tc->bytes_sent ();

  ::TAO::CounterT br = tc->bytes_received ();

  ::TAO::CounterT rs = tc->messages_sent ();

  ::TAO::CounterT rr = tc->messages_received ();

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("Tester (%P|%t) Transport [%q] [%C:%d -> %C:%d] ")
              ACE_TEXT ("Sent/Received [bytes=%q/%q, messages=%q/%q]\n"),
              (ACE_UINT64)id,
              rhost.in (), tc->remote_port (),
              lhost.in (), tc->local_port (),
              (ACE_UINT64)bs,
              (ACE_UINT64)br,
              (ACE_UINT64)rs,
              (ACE_UINT64)rr));
  return 0;
}
