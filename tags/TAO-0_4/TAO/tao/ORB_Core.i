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
  return this->orb_;
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

ACE_INLINE
TAO_ORB_Parameters *
TAO_ORB_Core::orb_params(void)
{
  return this->orb_params_;
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

ACE_INLINE TAO_GIOP_ServiceContextList&
TAO_ORB_Core::service_context (void)
{
  return this->service_context_;
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

ACE_INLINE TAO_Leader_Follower &
TAO_ORB_Core::leader_follower (void)
{
  return this->leader_follower_;
}

// ****************************************************************

ACE_INLINE
TAO_Leader_Follower::TAO_Leader_Follower (TAO_ORB_Core* orb_core)
  : orb_core_ (orb_core),
    reverse_lock_ (lock_),
    leaders_ (0)
{
}

ACE_INLINE TAO_ORB_Core_TSS_Resources *
TAO_Leader_Follower::get_tss_resources (void) const
{
  return this->orb_core_->get_tss_resources ();
}

ACE_INLINE void
TAO_Leader_Follower::set_server_thread (void)
{
  // Set the TSS flag to remember that we are a leader thread...
  TAO_ORB_Core_TSS_Resources *tss = this->get_tss_resources ();
  tss->is_server_thread_ = 1;

  ++this->leaders_;
}

ACE_INLINE void
TAO_Leader_Follower::reset_server_thread (void)
{
  // Set the TSS flag to remember that we are a leader thread...
  TAO_ORB_Core_TSS_Resources *tss = this->get_tss_resources ();
  tss->is_server_thread_ = 0;

  --this->leaders_;
}

ACE_INLINE int
TAO_Leader_Follower::leader_available (void) const
{
  return this->leaders_ != 0;
}

ACE_INLINE void
TAO_Leader_Follower::set_client_thread (void)
{
  // Set the TSS flag to remember that we are a leader thread...
  TAO_ORB_Core_TSS_Resources *tss = this->get_tss_resources ();
  if (tss->is_server_thread_)
    {
      --this->leaders_;
    }
}

ACE_INLINE void
TAO_Leader_Follower::reset_client_thread (void)
{
  // Set the TSS flag to remember that we are a leader thread...
  TAO_ORB_Core_TSS_Resources *tss = this->get_tss_resources ();
  if (tss->is_server_thread_)
    {
      ++this->leaders_;
    }
}

ACE_INLINE void
TAO_Leader_Follower::set_leader_thread (void)
{
  TAO_ORB_Core_TSS_Resources *tss = this->get_tss_resources ();
  if (tss->is_leader_thread_ == 0)
    {
      ++this->leaders_;
    }
  ++tss->is_leader_thread_;
}

ACE_INLINE void
TAO_Leader_Follower::reset_leader_thread (void)
{
  TAO_ORB_Core_TSS_Resources *tss = this->get_tss_resources ();
  --tss->is_leader_thread_;
  if (tss->is_leader_thread_ == 0)
    {
      --this->leaders_;
    }
}

ACE_INLINE int
TAO_Leader_Follower::is_leader_thread (void) const
{
  TAO_ORB_Core_TSS_Resources *tss = this->get_tss_resources ();
  return tss->is_leader_thread_ != 0;
}

ACE_INLINE int
TAO_Leader_Follower::follower_available (void) const
{
  return !this->follower_set_.is_empty ();
}

ACE_INLINE int
TAO_Leader_Follower::elect_new_leader (void)
{
  if (this->leaders_ == 0 && this->follower_available ())
    {
      ACE_SYNCH_CONDITION* condition_ptr = this->get_next_follower ();
      if (condition_ptr == 0 || condition_ptr->signal () == -1)
        return -1;
    }
  return 0;
}

ACE_INLINE int
TAO_Leader_Follower::add_follower (ACE_SYNCH_CONDITION *follower_ptr)
{
  if (this->follower_set_.insert (follower_ptr) != 0)
    return -1;
  return 0;
}

ACE_INLINE int
TAO_Leader_Follower::remove_follower (ACE_SYNCH_CONDITION *follower_ptr)
{
  return this->follower_set_.remove (follower_ptr);
}

ACE_INLINE ACE_SYNCH_MUTEX &
TAO_Leader_Follower::lock (void)
{
  return this->lock_;
}

ACE_INLINE ACE_Reverse_Lock<ACE_SYNCH_MUTEX> &
TAO_Leader_Follower::reverse_lock (void)
{
  return this->reverse_lock_;
}

// ****************************************************************

ACE_INLINE TAO_ORB_Table*
TAO_ORB_Table::instance (void)
{
  return ACE_Singleton<TAO_ORB_Table,ACE_SYNCH_MUTEX>::instance ();
}
