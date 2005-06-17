// $Id$

// -- PortableServer Include --
#include "POA_Current.h"
#include "tao/PortableServer/POA_Current_Impl.h"

// -- TAO Include --
#include "tao/TSS_Resources.h"

#if !defined (__ACE_INLINE__)
# include "POA_Current.inl"
#endif /* __ACE_INLINE__ */

ACE_RCSID (PI_Server,
           POA_Current,
           "$Id$")

namespace TAO
{
  namespace Portable_Server
  {
    PortableServer::POA_ptr
    POA_Current::get_POA (ACE_ENV_SINGLE_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       PortableServer::Current::NoContext))
    {
      POA_Current_Impl *impl = this->implementation ();

      if (impl == 0)
        ACE_THROW_RETURN (PortableServer::Current::NoContext (),
                          0);
      return impl->get_POA ();
    }

    PortableServer::ObjectId *
    POA_Current::get_object_id (ACE_ENV_SINGLE_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       PortableServer::Current::NoContext))
    {
      POA_Current_Impl *impl = this->implementation ();

      if (impl == 0)
        ACE_THROW_RETURN (PortableServer::Current::NoContext (),
                          0);
      return impl->get_object_id ();
    }

    PortableServer::Servant
    POA_Current::get_servant (ACE_ENV_SINGLE_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       PortableServer::Current::NoContext))
    {
      POA_Current_Impl *impl = this->implementation ();

      if (impl == 0)
        ACE_THROW_RETURN (PortableServer::Current::NoContext (),
                          0);
      return impl->get_servant ();
    }


    POA_Current_Impl *
    POA_Current::implementation (void)
    {
      return static_cast <POA_Current_Impl *>
                        (TAO_TSS_RESOURCES::instance ()->poa_current_impl_);
    }

    POA_Current_Impl *
    POA_Current::implementation (POA_Current_Impl *new_current)
    {
      TAO_TSS_Resources *tss =
        TAO_TSS_RESOURCES::instance ();

      POA_Current_Impl *old =
        static_cast <POA_Current_Impl *>
                     (tss->poa_current_impl_);
      tss->poa_current_impl_ = new_current;
      return old;
    }
  }
}

