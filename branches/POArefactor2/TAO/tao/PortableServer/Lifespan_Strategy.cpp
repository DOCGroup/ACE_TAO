// -*- C++ -*-

//=============================================================================
/**
 *  @file    Lifespan_Strategy.cpp
 *
 *  $Id$
 *
 */
//=============================================================================

#include "Lifespan_Strategy.h"

ACE_RCSID (PortableServer,
           Lifespan_Strategy,
           "$Id$")

#if (TAO_HAS_MINIMUM_POA == 0)

#include "tao/debug.h"

namespace TAO
{
  namespace Portable_Server
  {
    Lifespan_Strategy::Lifespan_Strategy () :
      poa_ (0)
    {
    }

    Lifespan_Strategy::~Lifespan_Strategy ()
    {
    }

    void
    Lifespan_Strategy::strategy_init (TAO_POA *poa)
    {
      poa_ = poa;
    }

    Transient_Lifespan_Strategy::~Transient_Lifespan_Strategy ()
    {
    }

    void
    Persistent_Lifespan_Strategy::imr_notify_startup (ACE_ENV_SINGLE_ARG_DECL)
    {
/*
  if (TAO_debug_level > 0)
    ACE_DEBUG ((LM_DEBUG, "Notifying ImR of startup\n"));

  CORBA::Object_var imr = this->orb_core ().implrepo_service ();

  if (CORBA::is_nil (imr.in ()))
      return;

  ImplementationRepository::Administration_var imr_locator =
    ImplementationRepository::Administration::_narrow (imr.in () ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  if (CORBA::is_nil(imr_locator.in ()))
      return;

  TAO_POA *root_poa = this->object_adapter ().root_poa ();
  ACE_NEW_THROW_EX (this->server_object_,
                    ServerObject_i (this->orb_core_.orb (),
                                    root_poa),
                    CORBA::NO_MEMORY ());
  ACE_CHECK;

  PortableServer::ServantBase_var safe_servant (this->server_object_);
  ACE_UNUSED_ARG (safe_servant);

  // Since this method is called from the POA constructor, there
  // shouldn't be any waiting required.  Therefore,
  // <wait_occurred_restart_call_ignored> can be ignored.
  int wait_occurred_restart_call_ignored = 0;

  // Activate the servant in the root poa.
  PortableServer::ObjectId_var id =
    root_poa->activate_object_i (this->server_object_,
                                 this->cached_policies_.server_priority (),
                                 wait_occurred_restart_call_ignored
                                 ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  CORBA::Object_var obj = root_poa->id_to_reference_i (id.in  ()
                                                       ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  ImplementationRepository::ServerObject_var svr
    = ImplementationRepository::ServerObject::_narrow (obj.in ()
                                                       ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  if (!svr->_stubobj () || !svr->_stubobj ()->profile_in_use ())
    {
      ACE_ERROR ((LM_ERROR, "Invalid ImR ServerObject, bailing out.\n"));
      return;
    }

  CORBA::String_var ior =
    svr->_stubobj ()->profile_in_use ()->to_string (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  // Search for "corbaloc:" alone, without the protocol.  This code
  // should be protocol neutral.
  const char corbaloc[] = "corbaloc:";
  char *pos = ACE_OS::strstr (ior.inout (), corbaloc);
  pos = ACE_OS::strchr (pos + sizeof (corbaloc), ':');

  pos = ACE_OS::strchr (pos + 1,
                        svr->_stubobj ()->profile_in_use ()->object_key_delimiter ());

  ACE_CString partial_ior(ior.in (), (pos - ior.in()) + 1);

  if (TAO_debug_level > 0)
    ACE_DEBUG ((LM_DEBUG,
                "Informing IMR that we are running at: %s\n",
                ACE_TEXT_CHAR_TO_TCHAR (partial_ior.c_str())));

  ACE_TRY
    {
      imr_locator->server_is_running (this->name().c_str (),
                                      partial_ior.c_str(),
                                      svr.in()
                                      ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCH (CORBA::SystemException, sysex)
    {
      // Avoid warnings on platforms with native C++ exceptions
      ACE_UNUSED_ARG (sysex);
      ACE_RE_THROW;
    }
  ACE_CATCHANY
    {
      ACE_TRY_THROW (CORBA::TRANSIENT (
          CORBA::SystemException::_tao_minor_code (TAO_IMPLREPO_MINOR_CODE, 0),
          CORBA::COMPLETED_NO));
    }
  ACE_ENDTRY;
  ACE_CHECK;

  if (TAO_debug_level > 0)
    ACE_DEBUG ((LM_DEBUG, "Successfully notified ImR of Startup\n"));
 */
    }

    void
    Persistent_Lifespan_Strategy::imr_notify_shutdown (void)
    {
/*
  // Notify the Implementation Repository about shutting down.
  CORBA::Object_var imr = this->orb_core ().implrepo_service ();

  // Check to see if there was an imr returned.  If none, return ourselves.
  if (CORBA::is_nil (imr.in ()))
    return;

  ACE_TRY_NEW_ENV
    {
      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG, "Notifing IMR of Shutdown server:%s\n", this->the_name()));

      // Get the IMR's administrative object and call shutting_down on it
      ImplementationRepository::Administration_var imr_locator =
        ImplementationRepository::Administration::_narrow (imr.in () ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      imr_locator->server_is_shutting_down (this->the_name () ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "POA::imr_notify_shutdown()");
      // Ignore exceptions
    }
  ACE_ENDTRY;
 */
    }

    Persistent_Lifespan_Strategy::Persistent_Lifespan_Strategy() :
      server_object_ (0),
      use_imr_ (1)
    {
    }

    Persistent_Lifespan_Strategy::~Persistent_Lifespan_Strategy ()
    {
    }
  } /* namespace Portable_Server */
} /* namespace TAO */

#endif /* TAO_HAS_MINIMUM_POA == 0 */
