// -*- C++ -*-
//
// $Id$

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

ACE_INLINE CORBA::Boolean
TAO_Root_POA::cleanup_in_progress (void)
{
  return this->cleanup_in_progress_;
}

ACE_INLINE ACE_Lock &
TAO_Root_POA::lock (void)
{
  return this->lock_;
}

ACE_INLINE TAO_POA_Policy_Set &
TAO_Root_POA::policies (void)
{
  return this->policies_;
}

ACE_INLINE PortableServer::POA_ptr
TAO_Root_POA::the_parent (void)
{
  return PortableServer::POA::_nil ();
}

ACE_INLINE PortableInterceptor::AdapterName *
TAO_Root_POA::adapter_name (void)
{
  return this->adapter_name_i ();
}

ACE_INLINE PortableInterceptor::ObjectReferenceTemplate *
TAO_Root_POA::get_adapter_template (void)
{
  TAO::ORT_Adapter *adapter = this->ORT_adapter ();

  if (adapter)
    {
      return this->ort_adapter_->get_adapter_template();
    }

  return 0;
}

ACE_INLINE PortableInterceptor::ObjectReferenceTemplate *
TAO_Root_POA::get_adapter_template_i ()
{
  if (this->ORT_adapter_i ())
    {
      return this->ort_adapter_->get_adapter_template ();
    }

  return 0;
}

ACE_INLINE PortableInterceptor::ObjectReferenceFactory *
TAO_Root_POA::get_obj_ref_factory (void)
{
  TAO::ORT_Adapter *adapter = this->ORT_adapter ();

  if (adapter)
    {
      return this->ort_adapter_->get_obj_ref_factory();
    }

  return 0;
}

ACE_INLINE CORBA::Boolean
TAO_Root_POA::system_id (void)
{
  return (this->cached_policies_.id_assignment () == PortableServer::SYSTEM_ID);
}

ACE_INLINE CORBA::Boolean
TAO_Root_POA::persistent (void)
{
  return (this->cached_policies_.lifespan () == PortableServer::PERSISTENT);
}

ACE_INLINE const ACE_CString &
TAO_Root_POA::name (void) const
{
  return this->name_;
}

ACE_INLINE char *
TAO_Root_POA::the_name (void)
{
  return CORBA::string_dup (this->name_.c_str ());
}

ACE_INLINE const TAO_Object_Adapter::poa_name &
TAO_Root_POA::folded_name (void) const
{
  return this->folded_name_;
}

ACE_INLINE const TAO_Object_Adapter::poa_name &
TAO_Root_POA::system_name (void) const
{
  return this->system_name_.in ();
}

ACE_INLINE char
TAO_Root_POA::name_separator (void)
{
  return '\0';
}

ACE_INLINE CORBA::ULong
TAO_Root_POA::name_separator_length (void)
{
  return sizeof (char);
}

ACE_INLINE char
TAO_Root_POA::persistent_key_char (void)
{
  return 'P';
}

ACE_INLINE char
TAO_Root_POA::transient_key_char (void)
{
  return 'T';
}

ACE_INLINE CORBA::ULong
TAO_Root_POA::persistent_key_type_length (void)
{
  return sizeof (char);
}

ACE_INLINE char
TAO_Root_POA::system_id_key_char (void)
{
  return 'S';
}

ACE_INLINE char
TAO_Root_POA::user_id_key_char (void)
{
  return 'U';
}

ACE_INLINE CORBA::ULong
TAO_Root_POA::system_id_key_type_length (void)
{
  return sizeof (char);
}

ACE_INLINE char
TAO_Root_POA::root_key_char (void)
{
  return 'R';
}

ACE_INLINE char
TAO_Root_POA::non_root_key_char (void)
{
  return 'N';
}

ACE_INLINE char
TAO_Root_POA::root_key_type (void)
{
  return TAO_Root_POA::root_key_char ();
}

ACE_INLINE CORBA::ULong
TAO_Root_POA::root_key_type_length (void)
{
  return sizeof (char);
}

ACE_INLINE CORBA::ULong
TAO_Root_POA::outstanding_requests (void) const
{
  return this->outstanding_requests_;
}

ACE_INLINE void
TAO_Root_POA::outstanding_requests (CORBA::ULong new_outstanding_requests)
{
  this->outstanding_requests_ = new_outstanding_requests;
}

ACE_INLINE CORBA::ULong
TAO_Root_POA::increment_outstanding_requests (void)
{
  return ++this->outstanding_requests_;
}

ACE_INLINE CORBA::ULong
TAO_Root_POA::decrement_outstanding_requests (void)
{
  return --this->outstanding_requests_;
}

ACE_INLINE CORBA::Boolean
TAO_Root_POA::waiting_destruction (void) const
{
  return this->waiting_destruction_;
}

ACE_INLINE TAO_ORB_Core &
TAO_Root_POA::orb_core (void) const
{
  return this->orb_core_;
}

ACE_INLINE PortableInterceptor::AdapterState
TAO_Root_POA::get_adapter_state (void)
{
  return this->adapter_state_;
}

ACE_INLINE void *
TAO_Root_POA::thread_pool (void) const
{
  return 0;
}

ACE_INLINE TAO_SYNCH_CONDITION &
TAO_Root_POA::servant_deactivation_condition (void)
{
  return servant_deactivation_condition_;
}

TAO_END_VERSIONED_NAMESPACE_DECL
