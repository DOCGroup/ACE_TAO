// $Id$

ACE_INLINE ACE_Lock &
TAO_Object_Adapter::lock (void)
{
  return *this->lock_;
}

ACE_INLINE ACE_SYNCH_MUTEX &
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

ACE_INLINE void
TAO_Object_Adapter::deactivate (CORBA::Boolean wait_for_completion,
                                CORBA::Environment &ACE_TRY_ENV)
{
  // Lock access for the duration of this transaction.
  TAO_POA_GUARD (ACE_Lock, monitor, this->lock (), ACE_TRY_ENV);

  this->deactivate_i (wait_for_completion,
                      ACE_TRY_ENV);
}

ACE_INLINE int
TAO_Object_Adapter::locate_servant (const TAO_ObjectKey &key,
                                    CORBA::Environment &ACE_TRY_ENV)
{
  // Lock access for the duration of this transaction.
  TAO_POA_GUARD_RETURN (ACE_Lock, monitor, this->lock (), -1, ACE_TRY_ENV);

  return this->locate_servant_i (key,
                                 ACE_TRY_ENV);
}

ACE_INLINE PortableServer::Servant
TAO_Object_Adapter::find_servant (const TAO_ObjectKey &key,
                                  CORBA::Environment &ACE_TRY_ENV)
{
  // Lock access for the duration of this transaction.
  TAO_POA_GUARD_RETURN (ACE_Lock, monitor, this->lock (), 0, ACE_TRY_ENV);

  return this->find_servant_i (key,
                               ACE_TRY_ENV);
}

ACE_INLINE int
TAO_Object_Adapter::find_persistent_poa (const poa_name &system_name,
                                         TAO_POA *&poa,
                                         CORBA_Environment &ACE_TRY_ENV)
{
  return this->hint_strategy_->find_persistent_poa (system_name,
                                                    poa,
                                                    ACE_TRY_ENV);
}

ACE_INLINE int
TAO_Object_Adapter::find_poa (const poa_name &system_name,
                              CORBA::Boolean activate_it,
                              CORBA::Boolean root,
                              const TAO_Temporary_Creation_Time &poa_creation_time,
                              TAO_POA *&poa,
                              CORBA_Environment &ACE_TRY_ENV)
{
  if (activate_it)
    {
      return this->find_persistent_poa (system_name,
                                        poa,
                                        ACE_TRY_ENV);
    }
  else
    {
      return this->find_transient_poa (system_name,
                                       root,
                                       poa_creation_time,
                                       poa);
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
