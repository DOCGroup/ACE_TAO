// $Id$

#include "tao/TransportCurrent/Transport_Current.h"

#if defined (TAO_AS_STATIC_LIBS)
#  include "tao/TransportCurrent/Current_Loader.h"

// Create an object that will insert the <Current_Loader> into the
// list of statically linked services that the <ACE_Service_Config>
// will initialize at run-time.
ACE_STATIC_SVC_REQUIRE (TAO_Transport_Current_Loader);
#endif /* TAO_AS_STATIC_LIBS */


using namespace TAO;


/// Test referencing the TC data outside of the context of an upcall,
/// or a client-side interceptor

int
test_transport_current (Transport::Current_ptr tc
                        ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   CORBA::UserException))
{
  CORBA::Long id = tc->id (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_TRY_CHECK;

  TAO::CounterT bs = tc->bytes_sent (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_TRY_CHECK;

  TAO::CounterT br = tc->bytes_received (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_TRY_CHECK;

  TAO::CounterT rs = tc->messages_sent (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_TRY_CHECK;

  TAO::CounterT rr = tc->messages_received (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_TRY_CHECK;

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
test_transport_current (CORBA::ORB_ptr orb
                        ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   CORBA::UserException))
{
  // Get the Current object.
  CORBA::Object_var tcobject =
    orb->resolve_initial_references ("TAO::Transport::Current"
                                      ACE_ENV_ARG_PARAMETER);
  ACE_TRY_CHECK;

  Transport::Current_var tc =
    Transport::Current::_narrow (tcobject.in ()
                                      ACE_ENV_SINGLE_ARG_DECL);
  ACE_TRY_CHECK;

  if (CORBA::is_nil (tc.in ()))
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("(%P|%t) client - ERROR: Could not resolve ")
                  ACE_TEXT ("TAOTransportCurrent object.\n")));

      ACE_TRY_THROW (CORBA::INTERNAL ());
    }

  return test_transport_current (tc.in ());
}
