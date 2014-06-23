// -*- C++ -*-
// $Id$

// -- PortableServer Include --
#include "tao/PortableServer/POA_Current.h"
#include "tao/PortableServer/POA_Current_Impl.h"

// -- TAO Include --
#include "tao/TSS_Resources.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  namespace Portable_Server
  {
    PortableServer::POA_ptr
    POA_Current::get_POA (void)
    {
      POA_Current_Impl *impl = this->implementation ();

      if (impl == 0)
        throw PortableServer::Current::NoContext ();
      return impl->get_POA ();
    }

    PortableServer::ObjectId *
    POA_Current::get_object_id (void)
    {
      POA_Current_Impl *impl = this->implementation ();

      if (impl == 0)
        throw PortableServer::Current::NoContext ();
      return impl->get_object_id ();
    }

    PortableServer::Servant
    POA_Current::get_servant (void)
    {
      POA_Current_Impl *impl = this->implementation ();

      if (impl == 0)
        throw PortableServer::Current::NoContext ();
      return impl->get_servant ();
    }

    CORBA::Object_ptr
    POA_Current::get_reference (void)
    {
      POA_Current_Impl *impl = this->implementation ();

      if (impl == 0)
        throw PortableServer::Current::NoContext ();
      return impl->get_reference ();
    }

    POA_Current_Impl *
    POA_Current::implementation (void)
    {
      return static_cast <POA_Current_Impl *>
                        (TAO_TSS_Resources::instance ()->poa_current_impl_);
    }

    POA_Current_Impl *
    POA_Current::implementation (POA_Current_Impl *new_current)
    {
      TAO_TSS_Resources *tss = TAO_TSS_Resources::instance ();

      POA_Current_Impl *old =
        static_cast <POA_Current_Impl *>
                     (tss->poa_current_impl_);
      tss->poa_current_impl_ = new_current;
      return old;
    }
  }
}

TAO_END_VERSIONED_NAMESPACE_DECL
