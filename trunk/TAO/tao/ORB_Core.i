// -*- C++ -*-
// $Id$

#include "ace/Dynamic_Service.h"

ACE_INLINE ACE_Thread_Manager *
TAO_ORB_Core::thr_mgr (void)
{
  return &this->tm_;
}

ACE_INLINE CORBA::ORB_ptr
TAO_ORB_Core::orb (void)
{
  return this->orb_.in ();
}

ACE_INLINE TAO_POA *
TAO_ORB_Core::root_poa (CORBA::Environment &ACE_TRY_ENV,
                        const char *adapter_name,
                        TAO_POA_Manager *poa_manager,
                        const TAO_POA_Policies *policies)
{
  if (this->root_poa_ == 0)
    {
      this->create_and_set_root_poa (adapter_name,
                                     poa_manager,
                                     policies,
                                     ACE_TRY_ENV);
    }
  return this->root_poa_;
}

ACE_INLINE void
TAO_ORB_Core::optimize_collocation_objects (CORBA::Boolean opt)
{
  this->opt_for_collocation_ = opt;
}

ACE_INLINE CORBA::Boolean
TAO_ORB_Core::optimize_collocation_objects (void) const
{
  return this->opt_for_collocation_;
}

ACE_INLINE void
TAO_ORB_Core::use_global_collocation (CORBA::Boolean opt)
{
  this->use_global_collocation_ = opt;
}

ACE_INLINE CORBA::Boolean
TAO_ORB_Core::use_global_collocation (void) const
{
  return this->use_global_collocation_;
}

ACE_INLINE CORBA::ULong
TAO_ORB_Core::get_collocation_strategy (void) const
{
  return this->collocation_strategy_;
}

ACE_INLINE TAO_ORB_Parameters *
TAO_ORB_Core::orb_params(void)
{
  return &(this->orb_params_);
}

#define TAO_OC_RETRIEVE(member) \
  ((this->member##_ == 0) ? (this->member##_ = this->resource_factory ()->get_##member ()) : (this->member##_) )

ACE_INLINE TAO_ProtocolFactorySet *
TAO_ORB_Core::protocol_factories (void)
{
  return TAO_OC_RETRIEVE (protocol_factories);
}

ACE_INLINE TAO_Connector_Registry *
TAO_ORB_Core::connector_registry (void)
{
  return TAO_OC_RETRIEVE (connector_registry);
}

ACE_INLINE TAO_Acceptor_Registry *
TAO_ORB_Core::acceptor_registry (void)
{
  return TAO_OC_RETRIEVE (acceptor_registry);
}

#undef TAO_OC_RETRIEVE

ACE_INLINE ACE_Char_Codeset_Translator *
TAO_ORB_Core::from_iso8859 (void) const
{
  return this->from_iso8859_;
}

ACE_INLINE ACE_Char_Codeset_Translator *
TAO_ORB_Core::to_iso8859 (void) const
{
  return this->to_iso8859_;
}

ACE_INLINE ACE_WChar_Codeset_Translator *
TAO_ORB_Core::from_unicode (void) const
{
  return this->from_unicode_;
}

ACE_INLINE ACE_WChar_Codeset_Translator *
TAO_ORB_Core::to_unicode (void) const
{
  return this->to_unicode_;
}

#if defined (TAO_HAS_CORBA_MESSAGING)
ACE_INLINE TAO_Policy_Manager*
TAO_ORB_Core::policy_manager (void)
{
  return &this->policy_manager_;
}

ACE_INLINE CORBA::Policy_ptr
TAO_ORB_Core::get_default_policy (
      CORBA::PolicyType policy,
      CORBA::Environment &ACE_TRY_ENV)
{
  return this->default_policies_.get_policy (policy, ACE_TRY_ENV);
}

ACE_INLINE POA_TAO::ClientPriorityPolicy*
TAO_ORB_Core::default_client_priority (void) const
{
  return this->default_policies_.client_priority ();
}

ACE_INLINE POA_Messaging::RelativeRoundtripTimeoutPolicy*
TAO_ORB_Core::default_relative_roundtrip_timeout (void) const
{
  return this->default_policies_.relative_roundtrip_timeout ();
}

#endif /* TAO_HAS_CORBA_MESSAGING */

ACE_INLINE TAO_ORB_Core_TSS_Resources*
TAO_ORB_Core::get_tss_resources (void)
{
  return ACE_TSS_GET (&this->tss_resources_,TAO_ORB_Core_TSS_Resources);
}

ACE_INLINE int
TAO_ORB_Core::has_shutdown (void)
{
  return this->has_shutdown_;
}

ACE_INLINE void
TAO_ORB_Core::check_shutdown (CORBA_Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  if (this->has_shutdown ())
    {
      // As defined by the CORBA 2.3 specification, throw a
      // CORBA::BAD_INV_ORDER exception with minor code 4 if the ORB
      // has shutdown by the time an ORB function is called.

      ACE_THROW (CORBA::BAD_INV_ORDER (4, CORBA::COMPLETED_NO));
    }
}


ACE_INLINE int
TAO_ORB_Core::thread_per_connection_timeout (ACE_Time_Value &timeout) const
{
  timeout = this->thread_per_connection_timeout_;
  return this->thread_per_connection_use_timeout_;
}

ACE_INLINE const char *
TAO_ORB_Core::orbid (void) const
{
  return this->orbid_;
}

// ****************************************************************

ACE_INLINE TAO_ORB_Table *
TAO_ORB_Table::instance (void)
{
  return ACE_Singleton<TAO_ORB_Table, ACE_SYNCH_MUTEX>::instance ();
}

ACE_INLINE TAO_ORB_Core *
TAO_ORB_Table::first_orb (void)
{
  return this->first_orb_;
}
