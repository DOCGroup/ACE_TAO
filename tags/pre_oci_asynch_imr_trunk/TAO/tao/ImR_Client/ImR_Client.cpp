// $Id$

#include "tao/ImR_Client/ImR_Client.h"

#include "ace/Vector_T.h"
#include "tao/debug.h"
#include "tao/ORB_Core.h"
#include "tao/Stub.h"
#include "tao/Profile.h"
#include "tao/PortableServer/Root_POA.h"
#include "tao/PortableServer/Non_Servant_Upcall.h"
#include "tao/ImR_Client/ServerObject_i.h"
#include "tao/ImR_Client/ImplRepoC.h"
#include "tao/IORManipulation/IORManip_Loader.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace
{
  char* find_delimiter (char* const ior, const char delimiter)
  {
    // Search for "corbaloc:" alone, without the protocol.  This code
    // should be protocol neutral.
    const char corbaloc[] = "corbaloc:";
    char *pos = ACE_OS::strstr (ior, corbaloc);
    pos = ACE_OS::strchr (pos + sizeof (corbaloc), ':');

    pos = ACE_OS::strchr (pos + 1, delimiter);

    return pos;
  }

  CORBA::Object_ptr combine (TAO_ORB_Core& orb_core,
                             const TAO_Profile& profile,
                             const char* const key_str,
                             const char* type_id)
  {
    CORBA::String_var profile_str = profile.to_string ();

//          if (TAO_debug_level > 0)
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("**************    IMR partial IOR =\n%C\n"),
                  profile_str.in ()));

    char* const pos = find_delimiter (profile_str.inout (),
                                      profile.object_key_delimiter ());
    if (pos)
      pos[1] = 0;  // Crop the string.
    else
      {
        if (TAO_debug_level > 0)
          ACE_ERROR ((LM_ERROR,
                      ACE_TEXT ("Could not parse ImR IOR, skipping ImRification\n")));
        return CORBA::Object::_nil();
      }

    ACE_CString ior (profile_str.in ());

    // Add the key.
    ior += key_str;

//          if (TAO_debug_level > 0)
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("**************    ImR-ified IOR =\n%C\n\n"),
                ior.c_str ()));

    CORBA::Object_ptr obj = orb_core.orb ()->string_to_object (ior.c_str ());
    obj->_stubobj()->type_id = type_id;
    return obj;
  }

  class ImRifyProfiles
  {
  public:
    ImRifyProfiles (const TAO_MProfile& base_profiles,
                    const TAO_Profile* const profile_in_use,
                    TAO_ORB_Core& orb_core,
                    const char* const key_str,
                    const char* type_id)
    : base_profiles_ (base_profiles),
      profile_in_use_ (profile_in_use),
      orb_core_ (orb_core),
      key_str_ (key_str),
      type_id_ (type_id),
      objs_ (base_profiles.profile_count()),
      list_buffer_ (new CORBA::Object_ptr[base_profiles.profile_count()]),
      ior_list_ (base_profiles.profile_count (),
                 base_profiles.profile_count (),
                 list_buffer_,
                 0)
    {
    }

    ~ImRifyProfiles () { delete [] list_buffer_; }

    CORBA::Object_ptr combined_ior ()
    {
      const CORBA::ULong pcount = base_profiles_.profile_count ();
      for (CORBA::ULong i = 0; i < pcount; ++i)
        {
          if (!combine_profile (i))
            {
              return default_obj (
                "could not resolve IORManipulation");
            }
        }

      CORBA::Object_var IORM = orb_core_.orb ()
        ->resolve_initial_references (TAO_OBJID_IORMANIPULATION, 0);

      if (CORBA::is_nil (IORM.in ()))
        {
          return default_obj (
            "could not resolve IORManipulation");
        }

      TAO_IOP::TAO_IOR_Manipulation_var iorm =
        TAO_IOP::TAO_IOR_Manipulation::_narrow (IORM.in ());

      if (CORBA::is_nil (iorm.in ()))
        {
          return default_obj (
            "could not narrow IORManipulation");
        }

      try
        {
          return iorm->merge_iors(ior_list_);
        }
      catch (const ::CORBA::Exception& )
        {
          return default_obj (
            "could not ImRify object with all profiles.");
        }
    }
  private:
    bool combine_profile(const CORBA::ULong i)
    {
      try
        {
          // store the combined profile+key
          list_buffer_[i] = combine (orb_core_,
                                     *(base_profiles_.get_profile (i)),
                                     key_str_,
                                     type_id_);
          // manage the memory
          objs_[i] = list_buffer_[i];

          return true;
        }
      catch (const ::CORBA::Exception& )
        {
          return false;
        }
    }

    CORBA::Object_ptr default_obj(const char* desc)
    {
      const CORBA::ULong pcount = base_profiles_.profile_count ();
      const char* info =
        "because couldn't find ImR profile_in_use in profiles";

      // identify the profile in use to see if we can default to
      // that profiles partial ImR-ification
      for (CORBA::ULong i = 0; i < pcount; ++i)
        {
          if (profile_in_use_ == base_profiles_.get_profile (i))
            {
              // if there is no object then try one last time to combine
              // the profile
              if (CORBA::is_nil(objs_[i].in ()) && !combine_profile (i))
                {
                  info = "because couldn't ImR-ify profile_in_use";
                  break;
                }
              ACE_ERROR((LM_ERROR,
                ACE_TEXT("ERROR: %C. ")
                ACE_TEXT("Defaulting to ImR-ifying profile_in_use\n"),
                desc));
              return objs_[i]._retn ();
            }
        }

      ACE_ERROR((LM_ERROR,
                 ACE_TEXT ("ERROR: %C, ")
                 ACE_TEXT ("but cannot default to ImR-ifying profile_in_use %C\n"),
                 desc,
                 info));
      return CORBA::Object::_nil();
    }

    const TAO_MProfile& base_profiles_;
    const TAO_Profile* const profile_in_use_;
    TAO_ORB_Core& orb_core_;
    const char* const key_str_;
    const char* const type_id_;
    ACE_Vector<CORBA::Object_var> objs_;
    CORBA::Object_ptr* const list_buffer_;
    TAO_IOP::TAO_IOR_Manipulation::IORList ior_list_;
  };
}

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
        {
          ACE_CString imr_info;
          if (TAO_debug_level > 1)
            {
              CORBA::ORB_ptr orb = poa->orb_core ().orb ();
              CORBA::String_var ior = orb->object_to_string (imr.in ());
              imr_info = ACE_CString (", IMR IOR=") + ior.in ();
            }
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("Notifying ImR of startup%s\n"),
                      imr_info.c_str ()));
        }

      ImplementationRepository::Administration_var imr_locator;

      {
        // ATTENTION: Trick locking here, see class header for details
        TAO::Portable_Server::Non_Servant_Upcall non_servant_upcall (*poa);
        ACE_UNUSED_ARG (non_servant_upcall);

        imr_locator =
          ImplementationRepository::Administration::_narrow (imr.in ());
      }

      if (CORBA::is_nil (imr_locator.in ()))
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

      CORBA::Object_var obj = root_poa->id_to_reference_i (id.in (), false);

      ImplementationRepository::ServerObject_var svr
        = ImplementationRepository::ServerObject::_narrow (obj.in ());

      if (!svr->_stubobj () || !svr->_stubobj ()->profile_in_use ())
        {
          ACE_ERROR ((LM_ERROR, "Invalid ImR ServerObject, bailing out.\n"));
          return;
        }

      CORBA::String_var full_ior = root_poa->_get_orb()->object_to_string (obj.in ());
      TAO_Profile& profile = *(svr->_stubobj ()->profile_in_use ());
      CORBA::String_var ior = profile.to_string();
      ACE_DEBUG((LM_INFO,
                 "\n\nfull_ior=<%s>\n\nior=<%s>\n\n",
                 full_ior.in(),
                 ior.in()));

      char* const pos = find_delimiter (ior.inout (),
                                        profile.object_key_delimiter ());

      const ACE_CString partial_ior (ior.in (), (pos - ior.in ()) + 1);

      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("Informing IMR that we are running at: %C\n"),
                    partial_ior.c_str ()));

      try
        {
          // ATTENTION: Trick locking here, see class header for details
          TAO::Portable_Server::Non_Servant_Upcall non_servant_upcall (*poa);
          ACE_UNUSED_ARG (non_servant_upcall);

          ACE_CString serverId = poa->orb_core ().server_id ();
          ACE_CString name;
          if (serverId.empty ())
            {
              name = poa->name ();
            }
          else
            {
              name = serverId + ":" + poa->name ();
            }

          imr_locator->server_is_running (name.c_str (),
                                          partial_ior.c_str (),
                                          svr.in ());
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
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("Successfully notified ImR of Startup\n")));
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
                          ACE_TEXT ("Notifying IMR of Shutdown server:%s\n"),
                          poaname.in ()));
            }

          // ATTENTION: Trick locking here, see class header for details
          TAO::Portable_Server::Non_Servant_Upcall non_servant_upcall (*poa);
          ACE_UNUSED_ARG (non_servant_upcall);

          // Get the IMR's administrative object and call shutting_down on it
          ImplementationRepository::Administration_var imr_locator =
            ImplementationRepository::Administration::_narrow (imr.in ());

          imr_locator->server_is_shutting_down (poa->name ().c_str ());
        }
      catch (const ::CORBA::COMM_FAILURE&)
        {
          // At the moment we call this during ORB shutdown and the ORB is
          // configured to drop replies during shutdown (it does by default in
          // the LF model) we get a COMM_FAILURE exception which we ignore
          if (TAO_debug_level > 0)
            ACE_DEBUG ((LM_DEBUG,
                        ACE_TEXT ("Ignoring COMM_FAILURE while unregistering")
                        ACE_TEXT ("from ImR.\n")));
        }
      catch (const ::CORBA::TRANSIENT&)
        {
          // Similarly, there are cases where we could get a TRANSIENT.
          if (TAO_debug_level > 0)
            ACE_DEBUG ((LM_DEBUG,
                        ACE_TEXT ("Ignoring TRANSIENT while unregistering")
                        ACE_TEXT ("from ImR.\n")));
        }
      catch (const ::CORBA::Exception& ex)
        {
          ex._tao_print_exception (
            "ImR_Client_Adapter_Impl::imr_notify_shutdown()");
          // Ignore exceptions
        }

      if (this->server_object_)
        {
          PortableServer::POA_var default_poa =
            this->server_object_->_default_POA ();

          TAO_Root_POA *root_poa =
            dynamic_cast <TAO_Root_POA*> (default_poa.in ());

          if (!root_poa)
            {
              throw ::CORBA::OBJ_ADAPTER ();
            }

          PortableServer::ObjectId_var id =
            root_poa->servant_to_id_i (this->server_object_);

          root_poa->deactivate_object_i (id.in ());

          this->server_object_ = 0;
        }
    }

    // *********************************************************************

    // Initialization and registration of dynamic service object.

    int
    ImR_Client_Adapter_Impl::Initializer (void)
    {
      TAO_Root_POA::imr_client_adapter_name (
        "Concrete_ImR_Client_Adapter");

      return ACE_Service_Config::process_directive (
        ace_svc_desc_ImR_Client_Adapter_Impl);
    }

    CORBA::Object_ptr
    ImR_Client_Adapter_Impl::imr_key_to_object(TAO_Root_POA* poa,
                                               const TAO::ObjectKey &key,
                                               const char* type_id) const
    {
      TAO_ORB_Core& orb_core = poa->orb_core ();
      // Check to see if we alter the IOR.
      CORBA::Object_var imr = orb_core.implrepo_service ();

      if (CORBA::is_nil (imr.in ())
          || !imr->_stubobj ()
          || !imr->_stubobj ()->profile_in_use ())
        {
          if (TAO_debug_level > 1)
            {
              ACE_DEBUG ((LM_DEBUG,
                          ACE_TEXT ("Missing ImR IOR, will not use the ImR\n")));
            }
          return CORBA::Object::_nil();
        }

      const TAO_MProfile& base_profiles = imr->_stubobj ()->base_profiles ();
      CORBA::String_var key_str;
      TAO::ObjectKey::encode_sequence_to_string (key_str.inout (), key);

      // if there is only one profile, no need to use IORManipulation
      if (base_profiles.profile_count() == 1)
        {
          return combine(orb_core,
                         *base_profiles.get_profile(0),
                         key_str.in(),
                         type_id);
        }

      // need to combine each profile in the ImR with the key and
      // then merge them all together into one ImR-ified ior
      ImRifyProfiles imrify (base_profiles,
                             imr->_stubobj ()->profile_in_use (),
                             orb_core,
                             key_str,
                             type_id);

      return imrify.combined_ior ();
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

