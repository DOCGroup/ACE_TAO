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
#include "tao/PortableServer/ImplRepo_i.h"
#include "tao/PortableServer/POA.h"
#include "tao/PortableServer/ServantRetentionStrategy.h"
#include "ace/OS_NS_sys_time.h"

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

    Transient_Lifespan_Strategy::Transient_Lifespan_Strategy () :
      creation_time_ (ACE_OS::gettimeofday ())
    {
    }

    Transient_Lifespan_Strategy::~Transient_Lifespan_Strategy ()
    {
    }

    void
    Transient_Lifespan_Strategy::notify_startup (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
    {
    }

    void
    Transient_Lifespan_Strategy::notify_shutdown ()
    {
    }

    bool
    Transient_Lifespan_Strategy::validate (
      CORBA::Boolean is_persistent,
      TAO::Portable_Server::Temporary_Creation_Time creation_time) const
    {
      return (!is_persistent && this->creation_time_ == creation_time);
    }

    bool
    Persistent_Lifespan_Strategy::validate (
      CORBA::Boolean is_persistent,
      TAO::Portable_Server::Temporary_Creation_Time creation_time) const
    {
      ACE_UNUSED_ARG (creation_time);
      return is_persistent;
    }


    CORBA::ULong
    Transient_Lifespan_Strategy::key_length (void) const
    {
      CORBA::ULong keylength = sizeof (char);

      #if (POA_NO_TIMESTAMP == 0)
        // Calculate the space required for the timestamp.
          keylength += TAO::Portable_Server::Creation_Time::creation_time_length ();
      #endif /* POA_NO_TIMESTAMP */

      return keylength;
    }

    void
    Transient_Lifespan_Strategy::create_key (
      CORBA::Octet *buffer,
      CORBA::ULong& starting_at)
    {
      // Copy the persistence byte.
      buffer[starting_at] = (CORBA::Octet) this->key_type ();
      starting_at += this->key_type_length ();

      #if (POA_NO_TIMESTAMP == 0)
        // Then copy the timestamp for transient POAs.
        ACE_OS::memcpy (&buffer[starting_at],
                        this->creation_time_.creation_time (),
                       TAO::Portable_Server::Creation_Time::creation_time_length ());
        starting_at += TAO::Portable_Server::Creation_Time::creation_time_length ();
      #endif /* POA_NO_TIMESTAMP */
    }

    char
    Transient_Lifespan_Strategy::key_type (void) const
    {
      // @@ Johnny, why can;t use the enum's defined within
      // PortableServer IDL? Wouldn't they make life much simpler?
      // @bala, this if for the IOR setup/parsing, I can't find these charachters
      // in the pidl file. In the POA it are static methods, seems that this
      // normal methods doesn't work, because in some static functions these are
      // used. This still needs some work, but I want to get those info out of
      // the POA.
      return 'T';
    }

    char
    Persistent_Lifespan_Strategy::key_type (void) const
    {
      // @@ Johnny, why can;t use the enum's defined within
      // PortableServer IDL? Wouldn't they make life much simpler?
      // @bala, this if for the IOR setup/parsing, I can't find these charachters
      // in the pidl file. In the POA it are static methods, seems that this
      // normal methods doesn't work, because in some static functions these are
      // used. This still needs some work, but I want to get those info out of
      // the POA.
      return 'P';
    }

    void
    Persistent_Lifespan_Strategy::create_key (
      CORBA::Octet *buffer,
      CORBA::ULong& starting_at)
    {
      // Copy the persistence byte.
      buffer[starting_at] = (CORBA::Octet) this->key_type ();
      starting_at += this->key_type_length ();
    }

    CORBA::ULong
    Lifespan_Strategy::key_type_length (void) const
    {
      return sizeof (char);
    }

    CORBA::ULong
    Persistent_Lifespan_Strategy::key_length () const
    {
      return this->key_type_length ();
    }

    void
    Persistent_Lifespan_Strategy::notify_startup (ACE_ENV_SINGLE_ARG_DECL)
    {
      (void) this->imr_notify_startup (ACE_ENV_SINGLE_ARG_PARAMETER);
    }

    void
    Persistent_Lifespan_Strategy::notify_shutdown ()
    {
      (void) this->imr_notify_shutdown ();

      if (this->server_object_)
        {
          PortableServer::POA_var poa =
            this->server_object_->_default_POA (ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_CHECK;

          TAO_POA *tao_poa = dynamic_cast<TAO_POA*>(poa.in());

          if (!tao_poa)
            {
              ACE_THROW (CORBA::OBJ_ADAPTER ());
            }

          PortableServer::ObjectId_var id =
            tao_poa->servant_to_id_i (this->server_object_
                                      ACE_ENV_ARG_PARAMETER);
          ACE_CHECK;

          tao_poa->active_policy_strategies().servant_retention_strategy()->deactivate_object (id.in() ACE_ENV_ARG_PARAMETER);
          ACE_CHECK;
        }
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
      use_imr_ (true)
    {
    }

    Persistent_Lifespan_Strategy::~Persistent_Lifespan_Strategy ()
    {
    }
  } /* namespace Portable_Server */
} /* namespace TAO */

#endif /* TAO_HAS_MINIMUM_POA == 0 */
