// $Id$


ACE_INLINE
TAO_POA_Current::TAO_POA_Current (void)
{
}

ACE_INLINE TAO_POA_Policy_Validator &
TAO_Object_Adapter::validator (void)
{
  return this->default_validator_;
}

ACE_INLINE TAO_POA_Policy_Set &
TAO_Object_Adapter::default_poa_policies (void)
{
  return this->default_poa_policies_;
}

ACE_INLINE ACE_Lock &
TAO_Object_Adapter::lock (void)
{
  return *this->lock_;
}

ACE_INLINE TAO_SYNCH_MUTEX &
TAO_Object_Adapter::thread_lock (void)
{
  return this->thread_lock_;
}

ACE_INLINE ACE_Reverse_Lock<ACE_Lock> &
TAO_Object_Adapter::reverse_lock (void)
{
  return this->reverse_lock_;
}

/* static */
ACE_INLINE CORBA::ULong
TAO_Object_Adapter::transient_poa_name_size ()
{
  return TAO_Object_Adapter::transient_poa_name_size_;
}

ACE_INLINE int
TAO_Object_Adapter::locate_servant (const TAO_ObjectKey &key
                                    TAO_ENV_ARG_DECL)
{
  // Lock access for the duration of this transaction.
  TAO_OBJECT_ADAPTER_GUARD_RETURN (-1);

  return this->locate_servant_i (key
                                 TAO_ENV_ARG_PARAMETER);
}

ACE_INLINE TAO_SERVANT_LOCATION
TAO_Object_Adapter::find_servant (const TAO_ObjectKey &key,
                                  PortableServer::Servant &servant
                                  TAO_ENV_ARG_DECL)
{
  // Lock access for the duration of this transaction.
  TAO_OBJECT_ADAPTER_GUARD_RETURN (TAO_SERVANT_NOT_FOUND);

  return this->find_servant_i (key,
                               servant
                               TAO_ENV_ARG_PARAMETER);
}

ACE_INLINE int
TAO_Object_Adapter::find_persistent_poa (const poa_name &system_name,
                                         TAO_POA *&poa
                                         TAO_ENV_ARG_DECL)
{
  return this->hint_strategy_->find_persistent_poa (system_name,
                                                    poa
                                                    TAO_ENV_ARG_PARAMETER);
}

ACE_INLINE int
TAO_Object_Adapter::find_poa (const poa_name &system_name,
                              CORBA::Boolean activate_it,
                              CORBA::Boolean root,
                              const TAO_Temporary_Creation_Time &poa_creation_time,
                              TAO_POA *&poa
                              TAO_ENV_ARG_DECL)
{
  if (activate_it)
    {
      return this->find_persistent_poa (system_name,
                                        poa
                                        TAO_ENV_ARG_PARAMETER);
    }
  else
    {
      return this->find_transient_poa (system_name,
                                       root,
                                       poa_creation_time,
                                       poa
                                       TAO_ENV_ARG_PARAMETER);
    }
}

ACE_INLINE int
TAO_Object_Adapter::bind_transient_poa (TAO_POA *poa,
                                        poa_name_out system_name)
{
  poa_name name;
  int result = this->transient_poa_map_->bind_create_key (poa,
                                                          name);

  if (result == 0)
    {
      ACE_NEW_RETURN (system_name,
                      poa_name (name),
                      -1);
    }

  return result;
}

ACE_INLINE int
TAO_Object_Adapter::bind_persistent_poa (const poa_name &folded_name,
                                         TAO_POA *poa,
                                         poa_name_out system_name)
{
  return this->hint_strategy_->bind_persistent_poa (folded_name,
                                                    poa,
                                                    system_name);
}

ACE_INLINE int
TAO_Object_Adapter::unbind_transient_poa (const poa_name &system_name)
{
  return this->transient_poa_map_->unbind (system_name);
}

ACE_INLINE int
TAO_Object_Adapter::unbind_persistent_poa (const poa_name &folded_name,
                                           const poa_name &system_name)
{
  return this->hint_strategy_->unbind_persistent_poa (folded_name,
                                                      system_name);
}

ACE_INLINE TAO_POA *
TAO_Object_Adapter::root_poa (void) const
{
  return this->root_;
}

ACE_INLINE TAO_ORB_Core &
TAO_Object_Adapter::orb_core (void) const
{
  return this->orb_core_;
}

ACE_INLINE TAO_Object_Adapter::Non_Servant_Upcall *
TAO_Object_Adapter::non_servant_upcall_in_progress (void) const
{
  return this->non_servant_upcall_in_progress_;
}

// ****************************************************************

ACE_INLINE TAO_POA &
TAO_Object_Adapter::Non_Servant_Upcall::poa (void) const
{
  return this->poa_;
}

ACE_INLINE TAO_POA &
TAO_Object_Adapter::Servant_Upcall::poa (void) const
{
  return *this->poa_;
}

ACE_INLINE PortableServer::Servant
TAO_Object_Adapter::Servant_Upcall::servant (void) const
{
  return this->servant_;
}

ACE_INLINE TAO_Object_Adapter &
TAO_Object_Adapter::Servant_Upcall::object_adapter (void) const
{
  return *this->object_adapter_;
}

ACE_INLINE const PortableServer::ObjectId &
TAO_Object_Adapter::Servant_Upcall::id (void) const
{
  return this->system_id_;
}

ACE_INLINE void
TAO_Object_Adapter::Servant_Upcall::user_id (
  const PortableServer::ObjectId *id)
{
  this->user_id_ = id;
}

ACE_INLINE const PortableServer::ObjectId &
TAO_Object_Adapter::Servant_Upcall::user_id (void) const
{
  return *(this->user_id_);
}

#if (TAO_HAS_MINIMUM_POA == 0)

ACE_INLINE PortableServer::ServantLocator::Cookie
TAO_Object_Adapter::Servant_Upcall::locator_cookie (void) const
{
  return this->cookie_;
}

ACE_INLINE void
TAO_Object_Adapter::Servant_Upcall::locator_cookie (PortableServer::ServantLocator::Cookie cookie)
{
  this->cookie_ = cookie;
}

ACE_INLINE const char *
TAO_Object_Adapter::Servant_Upcall::operation (void) const
{
  return this->operation_;
}

ACE_INLINE void
TAO_Object_Adapter::Servant_Upcall::operation (const char *name)
{
  this->operation_ = name;
}

#endif /* TAO_HAS_MINIMUM_POA == 0 */

ACE_INLINE void
TAO_Object_Adapter::Servant_Upcall::active_object_map_entry (TAO_Active_Object_Map::Map_Entry *entry)
{
  this->active_object_map_entry_ = entry;
}

ACE_INLINE TAO_Active_Object_Map::Map_Entry *
TAO_Object_Adapter::Servant_Upcall::active_object_map_entry (void) const
{
  return this->active_object_map_entry_;
}

ACE_INLINE void
TAO_Object_Adapter::Servant_Upcall::using_servant_locator (void)
{
  this->using_servant_locator_ = 1;
}

////////////////////////////////////////////////////////////////////////////////

ACE_INLINE void
TAO_POA_Current_Impl::poa (TAO_POA *p)
{
  this->poa_ = p;
}

ACE_INLINE TAO_POA *
TAO_POA_Current_Impl::poa (void) const
{
  return this->poa_;
}

ACE_INLINE void
TAO_POA_Current_Impl::object_id (const PortableServer::ObjectId &id)
{
  this->object_id_ = id;
}

ACE_INLINE const PortableServer::ObjectId &
TAO_POA_Current_Impl::object_id (void) const
{
  return this->object_id_;
}

ACE_INLINE void
TAO_POA_Current_Impl::object_key (const TAO_ObjectKey &key)
{
  this->object_key_ = &key;
}

ACE_INLINE const TAO_ObjectKey &
TAO_POA_Current_Impl::object_key (void) const
{
  return *this->object_key_;
}

ACE_INLINE void
TAO_POA_Current_Impl::servant (PortableServer::Servant servant)
{
  this->servant_ = servant;
}

ACE_INLINE PortableServer::Servant
TAO_POA_Current_Impl::servant (void) const
{
  return this->servant_;
}

ACE_INLINE void
TAO_POA_Current_Impl::priority (CORBA::Short priority)
{
  this->priority_ = priority;
}

ACE_INLINE CORBA::Short
TAO_POA_Current_Impl::priority (void) const
{
  return this->priority_;
}

ACE_INLINE CORBA::Short
TAO_Object_Adapter::Servant_Upcall::priority (void) const
{
  return this->current_context_.priority ();
}
