// $Id$

// -- PortableServer Include --
#include "Object_Adapter.h"
#include "POA_Current_Impl.h"
#include "Root_POA.h"
#include "POA_Guard.h"
#include "ServerRequestInfo.h"
#include "Default_Servant_Dispatcher.h"
#include "ServerInterceptorAdapter.h"
#include "Collocated_Object_Proxy_Broker.h"

// -- ACE Include --
#include "ace/Auto_Ptr.h"

// -- TAO Include --
#include "tao/ORB.h"
#include "tao/ORB_Core.h"
#include "tao/TSS_Resources.h"
#include "tao/TAO_Server_Request.h"
#include "tao/Stub.h"
#include "tao/Profile.h"
#include "tao/MProfile.h"
#include "tao/debug.h"
#include "tao/PortableInterceptor.h"
#include "tao/Thread_Lane_Resources_Manager.h"
#include "tao/Thread_Lane_Resources.h"
#include "tao/Protocols_Hooks.h"

#include "Policy_Creator_T.h"

#if !defined (__ACE_INLINE__)
# include "POA_Current_Impl.inl"
#endif /* __ACE_INLINE__ */

#include "ace/OS_NS_string.h"

#include "ace/Dynamic_Service.h"

ACE_RCSID (PortableServer,
           POA_Current_IMpl,
           "$Id$")

namespace TAO
{
  namespace Portable_Server
  {
    POA_Current_Impl::POA_Current_Impl (void)
      : poa_ (0),
        object_id_ (),
        object_key_ (0),
        servant_ (0),
        priority_ (TAO_INVALID_PRIORITY),
        previous_current_impl_ (0),
        setup_done_ (0)
    {
    }

    void
    POA_Current_Impl::setup (::TAO_Root_POA *p,
                             const TAO::ObjectKey &key)
    {
      // Remember information about this upcall.
      this->poa_ = p;
      this->object_key_ = &key;

      // Set the current context and remember the old one.
      this->tss_resources_ = TAO_TSS_RESOURCES::instance ();

      this->previous_current_impl_ =
        static_cast <POA_Current_Impl *>
                         (this->tss_resources_->poa_current_impl_);
      this->tss_resources_->poa_current_impl_ = this;

      // Setup is complete.
      this->setup_done_ = 1;
    }

    POA_Current_Impl *
    POA_Current_Impl::previous (void) const
    {
      return this->previous_current_impl_;
    }

    void
    POA_Current_Impl::teardown (void)
    {
      if (this->setup_done_)
        {
          // Reset the old context.
          this->tss_resources_->poa_current_impl_ = this->previous_current_impl_;
        }
    }

    PortableServer::POA_ptr
    POA_Current_Impl::get_POA (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       PortableServer::Current::NoContext))
    {
      return PortableServer::POA::_duplicate (this->poa_);
    }

    PortableServer::ObjectId *
    POA_Current_Impl::get_object_id (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       PortableServer::Current::NoContext))
    {
      PortableServer::ObjectId *objid = 0;

      // Create a new one and pass it back
      ACE_NEW_RETURN (objid,
                      PortableServer::ObjectId (this->object_id_),
                      0);
      return objid;
    }

    PortableServer::Servant
    POA_Current_Impl::get_servant (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       PortableServer::Current::NoContext))
    {
      return this->servant_;
    }

    TAO_ORB_Core &
    POA_Current_Impl::orb_core (void) const

    {
      return this->poa_->orb_core ();
    }
  }
}

