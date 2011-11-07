// -*- C++ -*-
// $Id$

#include "tao/PortableServer/LifespanStrategyPersistent.h"
#include "tao/PortableServer/Root_POA.h"
#include "tao/PortableServer/POAManager.h"
#include "tao/PortableServer/ImR_Client_Adapter.h"
#include "tao/ORB_Core.h"
#include "ace/OS_NS_sys_time.h"
#include "ace/Dynamic_Service.h"
#include "ace/Service_Config.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  namespace Portable_Server
  {
    void
    LifespanStrategyPersistent::strategy_init (TAO_Root_POA *poa)
    {
      LifespanStrategy::strategy_init (poa);

      this->use_imr_ = this->poa_->orb_core ().use_implrepo ();
    }

    bool
    LifespanStrategyPersistent::validate (
      CORBA::Boolean is_persistent,
      const TAO::Portable_Server::Temporary_Creation_Time& /*creation_time*/) const
    {
      return is_persistent;
    }

    char
    LifespanStrategyPersistent::key_type (void) const
    {
      return 'P';
    }

    CORBA::Boolean
    LifespanStrategyPersistent::is_persistent (void) const
    {
      return true;
    }

    void
    LifespanStrategyPersistent::create_key (
      CORBA::Octet *buffer,
      CORBA::ULong& starting_at)
    {
      // Copy the persistence byte.
      buffer[starting_at] = static_cast<CORBA::Octet> (this->key_type ());
      starting_at += this->key_type_length ();
    }

    CORBA::ULong
    LifespanStrategyPersistent::key_length () const
    {
      return this->key_type_length ();
    }

    void
    LifespanStrategyPersistent::notify_startup (void)
    {
      if (this->use_imr_)
        {
          // The user specified that the ImR should be used.
          ImR_Client_Adapter *adapter =
            ACE_Dynamic_Service<ImR_Client_Adapter>::instance (
              TAO_Root_POA::imr_client_adapter_name ()
            );

#if !defined (TAO_AS_STATIC_LIBS)
          // In case we build shared, try to load the ImR Client library, in a
          // static build we just can't do this, so don't try it, lower layers
          // output an error then.
          if (adapter == 0)
            {
              ACE_Service_Config::process_directive (
                ACE_DYNAMIC_SERVICE_DIRECTIVE(
                  "ImR_Client_Adapter", "TAO_ImR_Client",
                  "_make_ImR_Client_Adapter_Impl", ""));

              adapter =
                ACE_Dynamic_Service<ImR_Client_Adapter>::instance (
                  TAO_Root_POA::imr_client_adapter_name ());
            }
#endif /* !TAO_AS_STATIC_LIBS */

          if (adapter != 0)
            {
              adapter->imr_notify_startup (this->poa_);
            }
          else
            {
              // When we don't have a ImR_Client adapter, but the user
              // has specified that the ImR has to be used we have an
              // error situation which has to be reported.
              ACE_ERROR ((LM_ERROR,
                          ACE_TEXT ("(%P|%t) ERROR: No ImR_Client library ")
                          ACE_TEXT ("available but use IMR has been specified.\n")));

              throw ::CORBA::INTERNAL ();
            }
        }
    }

    void
    LifespanStrategyPersistent::notify_shutdown (void)
    {
      ImR_Client_Adapter *adapter =
        ACE_Dynamic_Service<ImR_Client_Adapter>::instance (
          TAO_Root_POA::imr_client_adapter_name ()
        );

      if (adapter != 0)
        {
          adapter->imr_notify_shutdown (this->poa_);
        }
    }

    LifespanStrategyPersistent::LifespanStrategyPersistent() :
      use_imr_ (true)
    {
    }

    void
    LifespanStrategyPersistent::check_state (void)
    {
      this->poa_->tao_poa_manager().check_state ();
    }

    ::PortableServer::LifespanPolicyValue
    LifespanStrategyPersistent::type() const
    {
      return ::PortableServer::PERSISTENT;
    }

    bool
    LifespanStrategyPersistent::use_imr () const
    {
      return use_imr_;
    }
  } /* namespace Portable_Server */
} /* namespace TAO */


TAO_END_VERSIONED_NAMESPACE_DECL
