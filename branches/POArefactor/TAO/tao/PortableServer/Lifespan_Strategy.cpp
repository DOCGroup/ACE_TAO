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
    void
    Persistent_Lifespan_Strategy::imr_notify_startup (ACE_ENV_SINGLE_ARG_DECL)
    {
/*      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG, "Notifying ImR of startup\n"));

      CORBA::Object_var imr = this->orb_core ().implrepo_service ();

      if (CORBA::is_nil (imr.in ()))
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
          ACE_ERROR ((LM_ERROR, "Invalid ServerObject, bailing out.\n"));
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

      ImplementationRepository::Locator_var imr_locator =
        ImplementationRepository::Locator::_narrow (imr.in ()
                                                    ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;

      if (CORBA::is_nil(imr_locator.in ()))
        {
          ACE_DEBUG ((LM_DEBUG, "Couldnt narrow down the ImR interface\n"));
          return;
        }

      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG,
                    "Informing IMR that we are running at: %s\n",
                    ACE_TEXT_CHAR_TO_TCHAR (partial_ior.c_str())));

      char host_name[MAXHOSTNAMELEN + 1];
      ACE_OS::hostname (host_name, MAXHOSTNAMELEN);

      ACE_TRY
        {
          // Relies on the fact that host_name will be same for the activator
          // We must pass this separately, because it is NOT possible to parse
          // the hostname from the ior portably. On some platforms the hostname
          // will be like 'foo.bar.com' and on others it will just be 'foo'
          imr_locator->server_is_running_in_activator (this->name().c_str (),
                                          host_name,
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
        ACE_DEBUG ((LM_DEBUG, "Successfully notified ImR of Startup\n"));*/
    }

    void
    Persistent_Lifespan_Strategy::imr_notify_shutdown (void)
    {
/*      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG, "Notifing IMR of Shutdown\n"));

      char host_name[MAXHOSTNAMELEN + 1];
      ACE_OS::hostname (host_name, MAXHOSTNAMELEN);

      // Notify the Implementation Repository about shutting down.
      CORBA::Object_var imr = this->orb_core ().implrepo_service ();

      // Check to see if there was an imr returned.  If none, return ourselves.
      if (CORBA::is_nil (imr.in ()))
        return;

      ACE_TRY_NEW_ENV
        {
          // Get the IMR's administrative object and call shutting_down on it
          ImplementationRepository::Locator_var imr_locator =
            ImplementationRepository::Locator::_narrow (imr.in ()
                                                        ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;

          imr_locator->server_is_shutting_down_in_activator (this->the_name (),
                                                             host_name
                                                ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }
      ACE_CATCHANY
        {
          ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "Server_i::init");
          // Ignore exceptions
        }
      ACE_ENDTRY;*/
    }
  } /* namespace Portable_Server */
} /* namespace TAO */

#endif /* TAO_HAS_MINIMUM_POA == 0 */
