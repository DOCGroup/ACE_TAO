// $Id$

#include "tao/ImR_Client/ImR_Client.h"

#include "tao/debug.h"
#include "tao/ORB_Core.h"
#include "tao/Stub.h"
#include "tao/Profile.h"
#include "tao/PortableServer/Root_POA.h"
#include "tao/PortableServer/Non_Servant_Upcall.h"
#include "tao/ImR_Client/ServerObject_i.h"
#include "tao/ImR_Client/ImplRepoC.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  namespace ImR_Client
  {
    ImR_Client_Adapter_Impl::ImR_Client_Adapter_Impl (void)
     : server_object_ (0)
    {
    }

    void
    ImR_Client_Adapter_Impl::imr_notify_startup (TAO_Root_POA* poa )
    {
      CORBA::Object_var imr = poa->orb_core ().implrepo_service ();

      if (CORBA::is_nil (imr.in ()))
        {
          ACE_ERROR ((LM_ERROR,
                      ACE_TEXT ("(%P|%t) ERROR: No usable IMR initial reference ")
                      ACE_TEXT ("available but use IMR has been specified.\n")));
          throw ::CORBA::TRANSIENT (
              CORBA::SystemException::_tao_minor_code (TAO_IMPLREPO_MINOR_CODE, 0),
              CORBA::COMPLETED_NO);
        }

      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG, "Notifying ImR of startup\n"));

      ImplementationRepository::Administration_var imr_locator;

      {
        // ATTENTION: Trick locking here, see class header for details
        TAO::Portable_Server::Non_Servant_Upcall non_servant_upcall (*poa);
        ACE_UNUSED_ARG (non_servant_upcall);

        imr_locator =
          ImplementationRepository::Administration::_narrow (imr.in ());
      }

      if (CORBA::is_nil(imr_locator.in ()))
        {
          ACE_ERROR ((LM_ERROR,
                      ACE_TEXT ("(%P|%t) ERROR: Narrowed IMR initial reference ")
                      ACE_TEXT ("is nil but use IMR has been specified.\n")));

          throw ::CORBA::TRANSIENT (
              CORBA::SystemException::_tao_minor_code (TAO_IMPLREPO_MINOR_CODE, 0),
              CORBA::COMPLETED_NO);
        }

      TAO_Root_POA *root_poa = poa->object_adapter ().root_poa ();
      ACE_NEW_THROW_EX (this->server_object_,
                        ServerObject_i (poa->orb_core ().orb (),
                                        root_poa),
                        CORBA::NO_MEMORY ());

      PortableServer::ServantBase_var safe_servant (this->server_object_);
      ACE_UNUSED_ARG (safe_servant);

      // Since this method is called from the POA constructor, there
      // shouldn't be any waiting required.  Therefore,
      // <wait_occurred_restart_call_ignored> can be ignored.
      bool wait_occurred_restart_call_ignored = false;

      // Activate the servant in the root poa.
      PortableServer::ObjectId_var id =
        root_poa->activate_object_i (this->server_object_,
                                     poa->server_priority (),
                                     wait_occurred_restart_call_ignored);

      CORBA::Object_var obj = root_poa->id_to_reference_i (id.in  (), false);

      ImplementationRepository::ServerObject_var svr
        = ImplementationRepository::ServerObject::_narrow (obj.in ());

      if (!svr->_stubobj () || !svr->_stubobj ()->profile_in_use ())
        {
          ACE_ERROR ((LM_ERROR, "Invalid ImR ServerObject, bailing out.\n"));
          return;
        }

      CORBA::String_var ior =
        svr->_stubobj ()->profile_in_use ()->to_string ();

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
                    "Informing IMR that we are running at: %C\n",
                    partial_ior.c_str()));

      try
        {
          // ATTENTION: Trick locking here, see class header for details
          TAO::Portable_Server::Non_Servant_Upcall non_servant_upcall (*poa);
          ACE_UNUSED_ARG (non_servant_upcall);

          ACE_CString serverId = poa->orb_core ().server_id ();
          ACE_CString name;
          if (serverId.empty ())
          {
            name = poa->name();
          }
          else
          {
            name = serverId + ":" + poa->name();
          }

          imr_locator->server_is_running (name.c_str (),
                                          partial_ior.c_str(),
                                          svr.in());
        }
      catch (const ::CORBA::SystemException&)
        {
          throw;
        }
      catch (const ::CORBA::Exception&)
        {
          throw ::CORBA::TRANSIENT (
              CORBA::SystemException::_tao_minor_code (TAO_IMPLREPO_MINOR_CODE, 0),
              CORBA::COMPLETED_NO);
        }

      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG, "Successfully notified ImR of Startup\n"));
    }

    void
    ImR_Client_Adapter_Impl::imr_notify_shutdown (TAO_Root_POA* poa )
    {
      // Notify the Implementation Repository about shutting down.
      CORBA::Object_var imr = poa->orb_core ().implrepo_service ();

      // Check to see if there was an imr returned.
      // If none, return ourselves.
      if (CORBA::is_nil (imr.in ()))
        return;

      try
        {
          if (TAO_debug_level > 0)
            {
              CORBA::String_var poaname = poa->the_name ();
              ACE_DEBUG ((LM_DEBUG,
                          "Notifying IMR of Shutdown server:%s\n",
                          poaname.in ()));
            }

          // ATTENTION: Trick locking here, see class header for details
          TAO::Portable_Server::Non_Servant_Upcall non_servant_upcall (*poa);
          ACE_UNUSED_ARG (non_servant_upcall);

          // Get the IMR's administrative object and call shutting_down on it
          ImplementationRepository::Administration_var imr_locator =
            ImplementationRepository::Administration::_narrow (imr.in ());

          imr_locator->server_is_shutting_down (poa->name().c_str ());
        }
      catch (const ::CORBA::COMM_FAILURE&)
        {
          // At the moment we call this during ORB shutdown and the ORB is
          // configured to drop replies during shutdown (it does by default in
          // the LF model) we get a COMM_FAILURE exception which we ignore
          if (TAO_debug_level > 0)
            ACE_DEBUG((LM_DEBUG, "Ignoring COMM_FAILURE while unregistering from ImR.\n"));
        }
      catch (const ::CORBA::TRANSIENT&)
        {
          // Similarly, there are cases where we could get a TRANSIENT.
          if (TAO_debug_level > 0)
            ACE_DEBUG((LM_DEBUG, "Ignoring TRANSIENT while unregistering from ImR.\n"));
        }
      catch (const ::CORBA::Exception& ex)
        {
          ex._tao_print_exception (
            "ImR_Client_Adapter_Impl::imr_notify_shutdown()");
          // Ignore exceptions
        }

      if (this->server_object_)
        {
          PortableServer::POA_var default_poa = this->server_object_->_default_POA ();

          TAO_Root_POA *root_poa = dynamic_cast <TAO_Root_POA*> (default_poa.in ());

          if (!root_poa)
            {
              throw ::CORBA::OBJ_ADAPTER ();
            }

          PortableServer::ObjectId_var id =
            root_poa->servant_to_id_i (this->server_object_);

          root_poa->deactivate_object_i (id.in());

          server_object_ = 0;
        }
    }

    // *********************************************************************

    // Initialization and registration of dynamic service object.

    int
    ImR_Client_Adapter_Impl::Initializer (void)
    {
      TAO_Root_POA::imr_client_adapter_name ("Concrete_ImR_Client_Adapter");

      return ACE_Service_Config::process_directive (ace_svc_desc_ImR_Client_Adapter_Impl);
    }
  }
}

ACE_STATIC_SVC_DEFINE (
  ImR_Client_Adapter_Impl,
  ACE_TEXT ("Concrete_ImR_Client_Adapter"),
  ACE_SVC_OBJ_T,
  &ACE_SVC_NAME (ImR_Client_Adapter_Impl),
  ACE_Service_Type::DELETE_THIS | ACE_Service_Type::DELETE_OBJ,
  0)

ACE_FACTORY_NAMESPACE_DEFINE (
  TAO_IMR_Client,
  ImR_Client_Adapter_Impl,
  TAO::ImR_Client::ImR_Client_Adapter_Impl)

TAO_END_VERSIONED_NAMESPACE_DECL

