// -*- C++ -*-
//
// $Id$

// Exception macros
#include "tao/Environment.h"

ACE_INLINE CORBA::Boolean
TAO_POA::cleanup_in_progress (void)
{
  return this->cleanup_in_progress_;
}

ACE_INLINE ACE_Lock &
TAO_POA::lock (void)
{
  return this->lock_;
}

ACE_INLINE TAO_POA_Policy_Set &
TAO_POA::policies (void)
{
  return this->policies_;
}

ACE_INLINE PortableServer::POA_ptr
TAO_POA::the_parent (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  if (this->parent_ != 0)
    return PortableServer::POA::_duplicate (this->parent_);
  else
    return PortableServer::POA::_nil ();
}

ACE_INLINE PortableInterceptor::AdapterName *
TAO_POA::adapter_name (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return this->adapter_name_i (ACE_ENV_SINGLE_ARG_PARAMETER);
}

ACE_INLINE PortableInterceptor::ObjectReferenceTemplate *
TAO_POA::get_adapter_template (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  if (this->ORT_adapter (ACE_ENV_SINGLE_ARG_PARAMETER))
    {
      return this->ort_adapter_->get_adapter_template();
    }

  return 0;
}

ACE_INLINE PortableInterceptor::ObjectReferenceTemplate *
TAO_POA::get_adapter_template_i ()
{
  if (this->ORT_adapter_i ())
    {
      return this->ort_adapter_->get_adapter_template ();
    }

  return 0;
}

ACE_INLINE PortableInterceptor::ObjectReferenceFactory *
TAO_POA::get_obj_ref_factory (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  if (this->ORT_adapter (ACE_ENV_SINGLE_ARG_PARAMETER))
    {
      return this->ort_adapter_->get_obj_ref_factory();
    }

  return 0;
}

ACE_INLINE const TAO::Portable_Server::Creation_Time &
TAO_POA::creation_time (void)
{
  return this->creation_time_;
}

ACE_INLINE CORBA::Boolean
TAO_POA::system_id (void)
{
  return (this->cached_policies_.id_assignment () == PortableServer::SYSTEM_ID);
}


ACE_INLINE CORBA::Boolean
TAO_POA::root (void)
{
  return this->parent_ == 0;
}


ACE_INLINE CORBA::Boolean
TAO_POA::persistent (void)
{
  return (this->cached_policies_.lifespan () == PortableServer::PERSISTENT);
}

ACE_INLINE const ACE_CString &
TAO_POA::name (void) const
{
  return this->name_;
}

ACE_INLINE char *
TAO_POA::the_name (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return CORBA::string_dup (this->name_.c_str ());
}

ACE_INLINE const TAO_Object_Adapter::poa_name &
TAO_POA::folded_name (void) const
{
  return this->folded_name_;
}

ACE_INLINE const TAO_Object_Adapter::poa_name &
TAO_POA::system_name (void) const
{
  return this->system_name_.in ();
}

ACE_INLINE char
TAO_POA::name_separator (void)
{
  return '\0';
}

ACE_INLINE CORBA::ULong
TAO_POA::name_separator_length (void)
{
  return sizeof (char);
}

ACE_INLINE char
TAO_POA::persistent_key_char (void)
{
  return 'P';
}

ACE_INLINE char
TAO_POA::transient_key_char (void)
{
  return 'T';
}

ACE_INLINE char
TAO_POA::persistent_key_type (void)
{
  if (this->persistent ())
    return TAO_POA::persistent_key_char ();
  else
    return TAO_POA::transient_key_char ();
}

ACE_INLINE CORBA::ULong
TAO_POA::persistent_key_type_length (void)
{
  return sizeof (char);
}

ACE_INLINE char
TAO_POA::system_id_key_char (void)
{
  return 'S';
}

ACE_INLINE char
TAO_POA::user_id_key_char (void)
{
  return 'U';
}

ACE_INLINE char
TAO_POA::system_id_key_type (void)
{
  if (this->system_id ())
    return TAO_POA::system_id_key_char ();
  else
    return TAO_POA::user_id_key_char ();
}

ACE_INLINE CORBA::ULong
TAO_POA::system_id_key_type_length (void)
{
  return sizeof (char);
}

ACE_INLINE char
TAO_POA::root_key_char (void)
{
  return 'R';
}

ACE_INLINE char
TAO_POA::non_root_key_char (void)
{
  return 'N';
}

ACE_INLINE char
TAO_POA::root_key_type (void)
{
  if (this->root ())
    return TAO_POA::root_key_char ();
  else
    return TAO_POA::non_root_key_char ();
}

ACE_INLINE CORBA::ULong
TAO_POA::root_key_type_length (void)
{
  return sizeof (char);
}

ACE_INLINE CORBA::ULong
TAO_POA::outstanding_requests (void) const
{
  return this->outstanding_requests_;
}

ACE_INLINE void
TAO_POA::outstanding_requests (CORBA::ULong new_outstanding_requests)
{
  this->outstanding_requests_ = new_outstanding_requests;
}

ACE_INLINE CORBA::ULong
TAO_POA::increment_outstanding_requests (void)
{
  return ++this->outstanding_requests_;
}

ACE_INLINE CORBA::ULong
TAO_POA::decrement_outstanding_requests (void)
{
  return --this->outstanding_requests_;
}

ACE_INLINE CORBA::Boolean
TAO_POA::waiting_destruction (void) const
{
  return this->waiting_destruction_;
}

ACE_INLINE TAO_ORB_Core &
TAO_POA::orb_core (void) const
{
  return this->orb_core_;
}

#if (TAO_HAS_MINIMUM_POA == 0)

ACE_INLINE PortableServer::ThreadPolicyValue
TAO_POA::thread_policy (void) const
{
  return this->cached_policies_.thread ();
}

#endif /* TAO_HAS_MINIMUM_CORBA == 0 */

ACE_INLINE PortableInterceptor::AdapterState
TAO_POA::get_adapter_state (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
{
  return this->adapter_state_;
}

ACE_INLINE TAO::Portable_Server::Cached_Policies &
TAO_POA::cached_policies (void)
{
  return this->cached_policies_;
}

ACE_INLINE
TAO::Portable_Server::Active_Policy_Strategies &
TAO_POA::active_policy_strategies (void)
{
  return this->active_policy_strategies_;
}

ACE_INLINE void *
TAO_POA::thread_pool (void) const
{
  return 0;
}

