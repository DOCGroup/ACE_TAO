// -*- C++ -*-
// $Id$
ACE_INLINE int
TAO_ORB_Parameters::sock_rcvbuf_size (void) const
{
  return sock_rcvbuf_size_;
}

ACE_INLINE void
TAO_ORB_Parameters::sock_rcvbuf_size (int s)
{
  sock_rcvbuf_size_ = s;
}

ACE_INLINE int
TAO_ORB_Parameters::sock_sndbuf_size (void) const
{
  return sock_sndbuf_size_;
}

ACE_INLINE void
TAO_ORB_Parameters::sock_sndbuf_size (int s)
{
  sock_sndbuf_size_ = s;
}

ACE_INLINE int
TAO_ORB_Parameters::cdr_memcpy_tradeoff (void) const
{
  return this->cdr_memcpy_tradeoff_;
}

ACE_INLINE void
TAO_ORB_Parameters::cdr_memcpy_tradeoff (int x)
{
  this->cdr_memcpy_tradeoff_ = x;
}

ACE_INLINE int
TAO_ORB_Parameters::use_dotted_decimal_addresses (void) const
{
  return this->use_dotted_decimal_addresses_;
}

ACE_INLINE void
TAO_ORB_Parameters::use_dotted_decimal_addresses (int x)
{
  this->use_dotted_decimal_addresses_ = x;
}


ACE_INLINE void
TAO_ORB_Parameters::service_port (TAO_MCAST_SERVICEID service_id,
                                  CORBA::UShort port)
{
  this->service_port_[service_id] = port;
}

ACE_INLINE CORBA::UShort
TAO_ORB_Parameters::service_port (TAO_MCAST_SERVICEID service_id) const
{
  return this->service_port_[service_id];
}

ACE_INLINE void
TAO_ORB_Parameters::mcast_discovery_endpoint (const char *mde)
{
  this->mcast_discovery_endpoint_ = CORBA::string_dup (mde);
}

ACE_INLINE const char *
TAO_ORB_Parameters::mcast_discovery_endpoint (void) const
{
  return this->mcast_discovery_endpoint_.in ();
}

ACE_INLINE void
TAO_ORB_Parameters::default_init_ref (const char *default_init_ref)
{
  this->default_init_ref_ = default_init_ref;
}

ACE_INLINE char *
TAO_ORB_Parameters::default_init_ref (void) const
{
  return CORBA::string_dup (this->default_init_ref_.c_str ());
}

ACE_INLINE int
TAO_ORB_Parameters::std_profile_components (void) const
{
  return this->std_profile_components_;
}

ACE_INLINE void
TAO_ORB_Parameters::std_profile_components (int x)
{
  this->std_profile_components_ = x;
}

ACE_INLINE int
TAO_ORB_Parameters::nodelay (void) const
{
  return this->nodelay_;
}

ACE_INLINE void
TAO_ORB_Parameters::nodelay (int x)
{
  this->nodelay_ = x;
}

ACE_INLINE int
TAO_ORB_Parameters::ace_sched_policy (void) const
{
  return this->ace_sched_policy_;
}

ACE_INLINE void
TAO_ORB_Parameters::ace_sched_policy (int x)
{
  this->ace_sched_policy_ = x;
}

ACE_INLINE long
TAO_ORB_Parameters::sched_policy (void) const
{
  return this->sched_policy_;
}

ACE_INLINE void
TAO_ORB_Parameters::sched_policy (long x)
{
  this->sched_policy_ = x;
}

ACE_INLINE long
TAO_ORB_Parameters::scope_policy (void) const
{
  return this->scope_policy_;
}

ACE_INLINE void
TAO_ORB_Parameters::scope_policy (long x)
{
  this->scope_policy_ = x;
}

ACE_INLINE long
TAO_ORB_Parameters::thread_creation_flags (void) const
{
  return
    this->scope_policy () |
    this->sched_policy ();
}

ACE_INLINE int
TAO_ORB_Parameters::single_read_optimization (void) const
{
  return this->single_read_optimization_;
}

ACE_INLINE void
TAO_ORB_Parameters::single_read_optimization (int x)
{
  this->single_read_optimization_ = x;
}

ACE_INLINE bool
TAO_ORB_Parameters::disable_rt_collocation_resolver (void) const
{
  return this->disable_rt_collocation_resolver_;
}

ACE_INLINE void
TAO_ORB_Parameters::disable_rt_collocation_resolver (bool x)
{
  this->disable_rt_collocation_resolver_ = x;
}
