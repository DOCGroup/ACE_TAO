// $Id$

// -- PortableServer Include --
#include "tao/PortableServer/Object_Adapter.h"
#include "tao/PortableServer/POA_Current_Impl.h"
#include "tao/PortableServer/Root_POA.h"

#include "tao/TSS_Resources.h"

#if !defined (__ACE_INLINE__)
# include "tao/PortableServer/POA_Current_Impl.inl"
#endif /* __ACE_INLINE__ */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  namespace Portable_Server
  {
    POA_Current_Impl::POA_Current_Impl (void)
      : poa_ (0),
        object_id_ (TAO_POA_OBJECT_ID_BUF_SIZE, 0, object_id_buf_),
        object_key_ (0),
        servant_ (0),
        priority_ (TAO_INVALID_PRIORITY),
        previous_current_impl_ (0),
        setup_done_ (false)
    {
    }

    void
    POA_Current_Impl::setup (::TAO_Root_POA *p, const TAO::ObjectKey &key)
    {
      // Remember information about this upcall.
      this->poa_ = p;
      this->object_key_ = &key;

      // Set the current context and remember the old one.
      this->tss_resources_ = TAO_TSS_Resources::instance ();

      this->previous_current_impl_ =
        static_cast <POA_Current_Impl *>
                         (this->tss_resources_->poa_current_impl_);
      this->tss_resources_->poa_current_impl_ = this;

      // Setup is complete.
      this->setup_done_ = true;
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
    POA_Current_Impl::get_POA (void)
    {
      return PortableServer::POA::_duplicate (this->poa_);
    }

    PortableServer::ObjectId *
    POA_Current_Impl::get_object_id (void)
    {
      PortableServer::ObjectId *objid = 0;

      // Create a new one and pass it back
      ACE_NEW_RETURN (objid,
                      PortableServer::ObjectId (this->object_id_),
                      0);
      return objid;
    }

    CORBA::Object_ptr
    POA_Current_Impl::get_reference (void)
    {
      return this->poa_->id_to_reference (this->object_id_);
    }

    PortableServer::Servant
    POA_Current_Impl::get_servant (void)
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

TAO_END_VERSIONED_NAMESPACE_DECL
