// $Id$

#include "ace/Dynamic_Service.h"

#define TAO_TRF (this->resource_factory ())
#define TAO_OC_RETRIEVE(member) \
  ((this->member##_ == 0) ? (this->member##_ = this->resource_factory ()->get_##member ()) : (this->member##_) )

ACE_INLINE TAO_Object_Adapter *
TAO_ORB_Core::object_adapter (void)
{
  return this->resource_factory ()->object_adapter ();
}

ACE_INLINE ACE_Thread_Manager *
TAO_ORB_Core::thr_mgr (ACE_Thread_Manager *tm)
{
  ACE_Thread_Manager *old_thr_mgr = this->thr_mgr_;
  this->thr_mgr_ = tm;
  return old_thr_mgr;
}

ACE_INLINE ACE_Thread_Manager*
TAO_ORB_Core::thr_mgr (void)
{
  return TAO_OC_RETRIEVE (thr_mgr);
}

ACE_INLINE CORBA::ORB_ptr
TAO_ORB_Core::orb (void)
{
  return this->orb_;
}

ACE_INLINE TAO_POA *
TAO_ORB_Core::root_poa (const char *adapter_name,
                        TAO_POA_Manager *poa_manager,
                        const TAO_POA_Policies *policies)
{
  if (TAO_OC_RETRIEVE (root_poa) == 0)
    this->create_and_set_root_poa (adapter_name,
                                   poa_manager,
                                   policies);

  return this->root_poa_;
}

ACE_INLINE
TAO_ORB_Parameters *
TAO_ORB_Core::orb_params(void)
{
  return this->orb_params_;
}

ACE_INLINE TAO_Connector_Registry *
TAO_ORB_Core::connector_registry (TAO_Connector_Registry *cr)
{
  TAO_Connector_Registry *old_cr = this->connector_registry_;
  this->connector_registry_ = cr;
  return old_cr;
}

ACE_INLINE TAO_Connector_Registry *
TAO_ORB_Core::connector_registry (void)
{
  return TAO_OC_RETRIEVE (connector_registry);
}

ACE_INLINE TAO_Acceptor *
TAO_ORB_Core::acceptor (TAO_Acceptor *a)
{
  TAO_Acceptor *old_acceptor = this->acceptor_;
  this->acceptor_ = a;
  return old_acceptor;
}

ACE_INLINE TAO_Acceptor *
TAO_ORB_Core::acceptor (void)
{
  return TAO_OC_RETRIEVE (acceptor);
}

#undef TAO_OC_RETRIEVE
#undef TAO_TRF

ACE_INLINE CORBA::Boolean
TAO_ORB_Core::using_collocation (void)
{
  return this->opt_for_collocation_;
}

ACE_INLINE CORBA::Boolean
TAO_ORB_Core::using_collocation (CORBA::Boolean use_col)
{
  CORBA::Boolean retv = this->opt_for_collocation_;
  this->opt_for_collocation_ = use_col;
  return retv;
}

ACE_INLINE ACE_Data_Block*
TAO_ORB_Core::create_input_cdr_data_block (size_t size)
{
  return this->resource_factory ()->create_input_cdr_data_block (size);
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

// ****************************************************************
