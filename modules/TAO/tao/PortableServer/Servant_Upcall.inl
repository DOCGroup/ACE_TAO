// -*- C++ -*-
//
// $Id$

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  namespace Portable_Server
  {
    ACE_INLINE ::TAO_Root_POA &
    Servant_Upcall::poa (void) const
    {
      return *this->poa_;
    }

    ACE_INLINE PortableServer::Servant
    Servant_Upcall::servant (void) const
    {
      return this->servant_;
    }

    ACE_INLINE TAO_Object_Adapter &
    Servant_Upcall::object_adapter (void) const
    {
      return *this->object_adapter_;
    }

    ACE_INLINE const PortableServer::ObjectId &
    Servant_Upcall::id (void) const
    {
      return this->system_id_;
    }

    ACE_INLINE void
    Servant_Upcall::user_id (
      const PortableServer::ObjectId *id)
    {
      this->user_id_ = id;
    }

    ACE_INLINE const PortableServer::ObjectId &
    Servant_Upcall::user_id (void) const
    {
      return *(this->user_id_);
    }

    #if (TAO_HAS_MINIMUM_POA == 0)

    ACE_INLINE void*
    Servant_Upcall::locator_cookie (void) const
    {
      return this->cookie_;
    }

    ACE_INLINE void
    Servant_Upcall::locator_cookie (void* cookie)
    {
      this->cookie_ = cookie;
    }

    ACE_INLINE const char *
    Servant_Upcall::operation (void) const
    {
      return this->operation_;
    }

    ACE_INLINE void
    Servant_Upcall::operation (const char *name)
    {
      this->operation_ = name;
    }

    #endif /* TAO_HAS_MINIMUM_POA == 0 */

    ACE_INLINE void
    Servant_Upcall::active_object_map_entry (TAO_Active_Object_Map_Entry *entry)
    {
      this->active_object_map_entry_ = entry;
    }

    ACE_INLINE TAO_Active_Object_Map_Entry *
    Servant_Upcall::active_object_map_entry (void) const
    {
      return this->active_object_map_entry_;
    }

    ACE_INLINE CORBA::Short
    Servant_Upcall::priority (void) const
    {
      return this->current_context_.priority ();
    }

    ACE_INLINE Servant_Upcall::State
    Servant_Upcall::state (void) const
    {
      return this->state_;
    }

    ACE_INLINE void
    Servant_Upcall::state (Servant_Upcall::State state)
    {
      this->state_ = state;
    }
  }
}

TAO_END_VERSIONED_NAMESPACE_DECL
