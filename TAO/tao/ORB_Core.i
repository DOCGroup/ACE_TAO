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

ACE_INLINE TAO_Acceptor_Registry *
TAO_ORB_Core::acceptor_registry (TAO_Acceptor_Registry *ar)
{
  TAO_Acceptor_Registry *old_ar = this->acceptor_registry_;
  this->acceptor_registry_ = ar;
  return old_ar;
}


ACE_INLINE TAO_ProtocolFactorySet *
TAO_ORB_Core::protocol_factories (TAO_ProtocolFactorySet *pf)
{
  TAO_ProtocolFactorySet *old = this->protocol_factories_;
  this->protocol_factories_ = pf;
  return old;
}

ACE_INLINE  TAO_ProtocolFactorySet *
TAO_ORB_Core::protocol_factories (void)
{
  return TAO_OC_RETRIEVE (protocol_factories);
}

ACE_INLINE TAO_Acceptor_Registry *
TAO_ORB_Core::acceptor_registry (void)
{
  return TAO_OC_RETRIEVE (acceptor_registry);
}

#undef TAO_OC_RETRIEVE
#undef TAO_TRF

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

#endif /* TAO_HAS_CORBA_MESSAGING */

// ****************************************************************

ACE_INLINE TAO_ORB_Table*
TAO_ORB_Table::instance (void)
{
  return ACE_Singleton<TAO_ORB_Table,ACE_SYNCH_MUTEX>::instance ();
}
