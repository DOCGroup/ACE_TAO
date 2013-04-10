// -*- C++ -*-
//
// $Id$

#include "tao/SystemException.h"
#include "ace/CORBA_macros.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

ACE_INLINE TAO_Policy_Validator &
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
TAO_Object_Adapter::locate_servant (const TAO::ObjectKey &key)
{
  // Lock access for the duration of this transaction.
  TAO_OBJECT_ADAPTER_GUARD_RETURN (-1);

  return this->locate_servant_i (key);
}

ACE_INLINE TAO_SERVANT_LOCATION
TAO_Object_Adapter::find_servant (const TAO::ObjectKey &key,
                                  PortableServer::Servant &servant)
{
  // Lock access for the duration of this transaction.
  TAO_OBJECT_ADAPTER_GUARD_RETURN (TAO_SERVANT_NOT_FOUND);

  return this->find_servant_i (key, servant);
}

ACE_INLINE int
TAO_Object_Adapter::find_persistent_poa (const poa_name &system_name,
                                         TAO_Root_POA *&poa)
{
  return this->hint_strategy_->find_persistent_poa (system_name, poa);
}

ACE_INLINE int
TAO_Object_Adapter::find_poa (const poa_name &system_name,
                              CORBA::Boolean activate_it,
                              CORBA::Boolean root,
                              const TAO::Portable_Server::Temporary_Creation_Time &poa_creation_time,
                              TAO_Root_POA *&poa)
{
  if (activate_it)
    {
      return this->find_persistent_poa (system_name, poa);
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
TAO_Object_Adapter::bind_transient_poa (TAO_Root_POA *poa,
                                        poa_name_out system_name)
{
  poa_name name;
  int result = this->transient_poa_map_->bind_create_key (poa, name);

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
                                         TAO_Root_POA *poa,
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

ACE_INLINE TAO_Root_POA *
TAO_Object_Adapter::root_poa (void) const
{
  return this->root_;
}

ACE_INLINE TAO_ORB_Core &
TAO_Object_Adapter::orb_core (void) const
{
  return this->orb_core_;
}

ACE_INLINE TAO::Portable_Server::Non_Servant_Upcall *
TAO_Object_Adapter::non_servant_upcall_in_progress (void) const
{
  return this->non_servant_upcall_in_progress_;
}

TAO_END_VERSIONED_NAMESPACE_DECL
