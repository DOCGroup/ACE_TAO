// $Id$

#include "ace/Dynamic_Service.h"

#define TAO_TRF (this->resource_factory ())
#define TAO_OC_RETRIEVE(member) \
  ( (this->##member##_ == 0) ? (this->##member##_ = this->resource_factory ()->get_##member ()) : (this->##member##_) )

ACE_INLINE void
TAO_ORB_Core::reactor (ACE_Reactor *r)
{
  this->reactor_ = r;
}

ACE_INLINE ACE_Reactor*
TAO_ORB_Core::reactor (void)
{
  return TAO_OC_RETRIEVE (reactor);
}

ACE_INLINE void
TAO_ORB_Core::thr_mgr (ACE_Thread_Manager *tm)
{
  this->thr_mgr_ = tm;
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
TAO_ORB_Core::root_poa (void)
{
  if (TAO_OC_RETRIEVE (root_poa) == 0)
    this->create_and_set_root_poa ();

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

ACE_INLINE void
TAO_ORB_Core::connector (TAO_CONNECTOR *c)
{
  this->connector_ = c;
}

ACE_INLINE TAO_CONNECTOR *
TAO_ORB_Core::connector (void)
{
  return TAO_OC_RETRIEVE (connector);
}

ACE_INLINE void
TAO_ORB_Core::acceptor (TAO_ACCEPTOR *a)
{
  this->acceptor_ = a;
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
