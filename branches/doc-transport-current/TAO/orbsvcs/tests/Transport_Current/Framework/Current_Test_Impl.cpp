// -*- C++ -*-

#include "orbsvcs/Transport_Current/TCC.h"

#include "Current_Test_Impl.h"

ACE_RCSID (Transport_Current,
           Current_Test_Impl,
           "$Id$")

  Current_Test_Impl::Current_Test_Impl (CORBA::ORB_ptr orb,
                                        PortableServer::POA_ptr poa,
                                        int do_collocated_calls)
    : orb_ (CORBA::ORB::_duplicate (orb))
    , poa_ (PortableServer::POA::_duplicate (poa))
    , do_collocated_calls_ (do_collocated_calls)
{
}

Current_Test_Impl::~Current_Test_Impl (void)
{
}

::CORBA::Long
Current_Test_Impl::self_test (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
        ACE_THROW_SPEC ((::CORBA::SystemException))
{
  return 0;
}

void
Current_Test_Impl::test_transport_current (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  if (TAO_debug_level > 2) {
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("(%P|%t) server - Testing access to Transport Current ...\n")));
  }

  CORBA::Object_var tcobject =
    orb_->resolve_initial_references ("TAO::Transport::Current"
                                      ACE_ENV_ARG_PARAMETER);
  ACE_TRY_CHECK;

  TAO::Transport::Current_var tc =
    TAO::Transport::Current::_narrow (tcobject.in ()
                                      ACE_ENV_SINGLE_ARG_DECL);
  ACE_TRY_CHECK;

  if (TAO_debug_level >= 1)
    {
      ::CORBA::Long id = tc->id (ACE_ENV_SINGLE_ARG_DECL);
      ACE_TRY_CHECK;

      ::TAO::CounterT bs = tc->bytes_sent (ACE_ENV_SINGLE_ARG_DECL);
      ACE_TRY_CHECK;

      ::TAO::CounterT br = tc->bytes_received (ACE_ENV_SINGLE_ARG_DECL);
      ACE_TRY_CHECK;

      ::TAO::CounterT rs = tc->messages_sent (ACE_ENV_SINGLE_ARG_DECL);
      ACE_TRY_CHECK;

      ::TAO::CounterT rr = tc->messages_received (ACE_ENV_SINGLE_ARG_DECL);
      ACE_TRY_CHECK;

      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("Current_Test_Impl (%P|%t) Transport [%q] - Sent/Received [bytes=%q/%q, messages=%q/%q]\n"),
                  (ACE_UINT64)id,
                  (ACE_UINT64)bs,
                  (ACE_UINT64)br,
                  (ACE_UINT64)rs,
                  (ACE_UINT64)rr));
    }

  if (TAO_debug_level > 2) {
    ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("(%P|%t) server - Successfully accessed Transport Current\n")));
  }
}

void
Current_Test_Impl::invoked_by_client (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_TRY
    {
      this->invoked_by_client_i (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCH (TAO::Transport::NoContext, ex)
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("(%P|%t) server - Caught a Transport::NoContext exception\n")));
    }
  ACE_CATCH (CORBA::SystemException, ex1)
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("(%P|%t) server - Caught a CORBA::SystemException exception\n")));
    }
  ACE_ENDTRY;
}

void
Current_Test_Impl::invoked_by_client_i (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException, TAO::Transport::NoContext))
{
  if (TAO_debug_level > 2) {
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("(%P|%t) server - Testing inside an upcall\n")));
  }

  this->test_transport_current (ACE_ENV_SINGLE_ARG_DECL);
  ACE_TRY_CHECK;

  if (this->do_collocated_calls_)
    {
      CORBA::Object_var selfobject =
        poa_->servant_to_reference (this
                                    ACE_ENV_SINGLE_ARG_DECL);
      ACE_TRY_CHECK;

      TAO::Transport::CurrentTest_var self =
        TAO::Transport::CurrentTest::_narrow (selfobject.in ()
                                              ACE_ENV_SINGLE_ARG_DECL);
      ACE_TRY_CHECK;

      if (TAO_debug_level >= 1)
        {
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("(%P|%t) server - Testing a collocated call\n")));
        }
      self->invoked_during_upcall (ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  else
    {
      if (TAO_debug_level >= 1)
        {
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("(%P|%t) server - Skipping the collocated call test\n")));
        }
    }
}

void
Current_Test_Impl::invoked_during_upcall (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  if (TAO_debug_level > 2) {
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("(%P|%t) server - Test method invoked ")
                ACE_TEXT ("by the server during upcall.\n")));
  }

}

void
Current_Test_Impl::shutdown (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  if (TAO_debug_level > 2) {
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("(%P|%t) server - shutting down.\n")));
  }

  this->orb_->shutdown (0
                        ACE_ENV_ARG_PARAMETER);
}
