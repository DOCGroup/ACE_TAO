// -*- C++ -*-

// $Id$

#include "orbsvcs/Transport_Current/IIOP_Transport_Current.h"

using namespace TAO;


/// Test referencing the TC data outside of the context of an upcall,
/// or a client-side interceptor

int
test_transport_current (CORBA::ORB_ptr orb
                        ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   CORBA::UserException))
{
  // Get the Current object.
  CORBA::Object_var tcobject =
    orb->resolve_initial_references (ACE_TEXT_ALWAYS_CHAR ("TAO::Transport::IIOP::Current")
                                     ACE_ENV_ARG_PARAMETER);
  ACE_TRY_CHECK;


  if (TAO_debug_level >= 1)
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("Tester (%P|%t) Resolved initial reference for IIOP::Current\n")));

  Transport::IIOP::Current_var tc =
    Transport::IIOP::Current::_narrow (tcobject.in ()
                                       ACE_ENV_SINGLE_ARG_DECL);
  ACE_TRY_CHECK;

  if (TAO_debug_level >= 1)
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("Tester (%P|%t) Narowed the IIOP Transport Current\n")));

  if (CORBA::is_nil (tc.in ()))
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("Tester (%P|%t) ERROR: Could not resolve ")
                  ACE_TEXT ("TAO::Transport::IIOP::Current object.\n")));

      ACE_TRY_THROW (CORBA::INTERNAL ());
    }

  ::CORBA::String_var rhost (tc->remote_host (ACE_ENV_SINGLE_ARG_PARAMETER));
  ACE_TRY_CHECK;

  ::CORBA::String_var lhost (tc->local_host (ACE_ENV_SINGLE_ARG_PARAMETER));
  ACE_TRY_CHECK;

  ::CORBA::Long id = tc->id (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_TRY_CHECK;

  ::TAO::CounterT bs = tc->bytes_sent (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_TRY_CHECK;

  ::TAO::CounterT br = tc->bytes_received (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_TRY_CHECK;

  ::TAO::CounterT rs = tc->messages_sent (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_TRY_CHECK;

  ::TAO::CounterT rr = tc->messages_received (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_TRY_CHECK;

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("Tester (%P|%t) Transport [%q] [%s:%d -> %s:%d] ")
              ACE_TEXT ("Sent/Received [bytes=%q/%q, messages=%q/%q]\n"),
              (ACE_UINT64)id,
              rhost.in (), tc->remote_port (ACE_ENV_SINGLE_ARG_PARAMETER),
              lhost.in (), tc->local_port (ACE_ENV_SINGLE_ARG_PARAMETER),
              (ACE_UINT64)bs,
              (ACE_UINT64)br,
              (ACE_UINT64)rs,
              (ACE_UINT64)rr));
  return 0;
}

