// $Id$

#include "ace/Dynamic_Service.h"

#define TAO_TRF (this->resource_factory ())
#define TAO_OC_RETRIEVE(member) \
  ((this->member##_ == 0) ? (this->member##_ = this->resource_factory ()->get_##member ()) : (this->member##_) )

ACE_INLINE ACE_Reactor *
TAO_ORB_Core::reactor (ACE_Reactor *r)
{
  ACE_Reactor *old_reactor = this->reactor_;
  this->reactor_ = r;
  return old_reactor;
}

ACE_INLINE ACE_Reactor*
TAO_ORB_Core::reactor (void)
{
  return TAO_OC_RETRIEVE (reactor);
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
  return TAO_OC_RETRIEVE (orb);
}

ACE_INLINE TAO_POA *
TAO_ORB_Core::root_poa (const char *adapter_name,
                        TAO_POA_Manager *poa_manager,
                        const TAO_POA_Policies *policies,
                        TAO_Object_Table *active_object_map)
{
  if (TAO_OC_RETRIEVE (root_poa) == 0)
    this->create_and_set_root_poa (adapter_name,
                                   poa_manager,
                                   policies,
                                   active_object_map);

  return this->root_poa_;
}

ACE_INLINE TAO_OA_Parameters *
TAO_ORB_Core::oa_params (void)
{
  return TAO_OC_RETRIEVE (oa_params);
}

ACE_INLINE
TAO_ORB_Parameters *
TAO_ORB_Core::orb_params(void)
{
  return TAO_OC_RETRIEVE (orb_params);
}

ACE_INLINE TAO_CONNECTOR *
TAO_ORB_Core::connector (TAO_CONNECTOR *c)
{
  TAO_CONNECTOR *old_connector = this->connector_;
  this->connector_ = c;
  return old_connector;
}

ACE_INLINE TAO_CONNECTOR *
TAO_ORB_Core::connector (void)
{
  return TAO_OC_RETRIEVE (connector);
}

ACE_INLINE TAO_ACCEPTOR *
TAO_ORB_Core::acceptor (TAO_ACCEPTOR *a)
{
  TAO_ACCEPTOR *old_acceptor = this->acceptor_;
  this->acceptor_ = a;
  return old_acceptor;
}

ACE_INLINE TAO_ACCEPTOR *
TAO_ORB_Core::acceptor (void)
{
  return TAO_OC_RETRIEVE (acceptor);
}

ACE_INLINE ACE_INET_Addr&
TAO_ORB_Core::addr (void)
{
  return *TAO_OC_RETRIEVE (addr);
}

#undef TAO_OC_RETRIEVE
#undef TAO_TRF

ACE_INLINE TAO_POA_Current *
TAO_ORB_Core::poa_current (void)
{
  return poa_current_;
}

ACE_INLINE TAO_POA_Current *
TAO_ORB_Core::poa_current (TAO_POA_Current *new_current)
{
  TAO_POA_Current *old = poa_current_;
  poa_current_ = new_current;
  return old;
}

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

// Template specializations that allow the cached connection manager
// to work better.

#if defined (ACE_HAS_TEMPLATE_SPECIALIZATION)
# if defined (__ACE_INLINE__) || (!defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION) && !defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA))
ACE_INLINE size_t
ACE_Hash_Addr<ACE_INET_Addr>::hash_i (const ACE_INET_Addr &addr) const
{
  return addr.get_ip_address () + addr.get_port_number ();
}
# endif /* __ACE_INLINE__ */
#endif /* ACE_HAS_TEMPLATE_SPECIALIZATION */
