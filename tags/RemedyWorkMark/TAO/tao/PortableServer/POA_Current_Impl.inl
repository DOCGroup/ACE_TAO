// -*- C++ -*-
//
// $Id$

#include "tao/SystemException.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  namespace Portable_Server
  {
    ACE_INLINE void
    POA_Current_Impl::poa (::TAO_Root_POA *p)
    {
      this->poa_ = p;
    }

    ACE_INLINE ::TAO_Root_POA *
    POA_Current_Impl::poa (void) const
    {
      return this->poa_;
    }

    ACE_INLINE void
    POA_Current_Impl::object_id (const PortableServer::ObjectId &id)
    {
      if (this->object_id_.release () ||
          this->object_id_.get_buffer() == this->object_id_buf_)
        {
          // Resize the current object_id_.  If it is less than the
          // length of the current buffer, no allocation will take place.
          CORBA::ULong id_size = id.length ();
          this->object_id_.length (id_size);

          // Get the buffer and copy the new object id in it's place.
          ACE_OS::memcpy (this->object_id_.get_buffer (),
                          id.get_buffer (), id_size);
        }
      else
        {
          this->object_id_ = id;
        }
    }

    ACE_INLINE const PortableServer::ObjectId &
    POA_Current_Impl::object_id (void) const
    {
      return this->object_id_;
    }

    ACE_INLINE void
    POA_Current_Impl::replace_object_id (
      const PortableServer::ObjectId &system_id)
    {
      // This has the effect of replacing the underlying buffer
      // with that of another object id without copying.
      object_id_.replace (system_id.maximum (),
                          system_id.length (),
                          const_cast <CORBA::Octet *> (system_id.get_buffer ()),
                          0);
    }

    ACE_INLINE void
    POA_Current_Impl::object_key (const TAO::ObjectKey &key)
    {
      this->object_key_ = &key;
    }

    ACE_INLINE const TAO::ObjectKey &
    POA_Current_Impl::object_key (void) const
    {
      return *this->object_key_;
    }

    ACE_INLINE void
    POA_Current_Impl::servant (PortableServer::Servant servant)
    {
      this->servant_ = servant;
    }

    ACE_INLINE PortableServer::Servant
    POA_Current_Impl::servant (void) const
    {
      return this->servant_;
    }

    ACE_INLINE void
    POA_Current_Impl::priority (CORBA::Short priority)
    {
      this->priority_ = priority;
    }

    ACE_INLINE CORBA::Short
    POA_Current_Impl::priority (void) const
    {
      return this->priority_;
    }

  }
}

TAO_END_VERSIONED_NAMESPACE_DECL
