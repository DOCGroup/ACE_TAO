// -*- C++ -*-
TAO_BEGIN_VERSIONED_NAMESPACE_DECL

ACE_INLINE int
TAO_ORB_Parameters::sock_rcvbuf_size () const
{
  return sock_rcvbuf_size_;
}

ACE_INLINE void
TAO_ORB_Parameters::sock_rcvbuf_size (int s)
{
  sock_rcvbuf_size_ = s;
}

ACE_INLINE int
TAO_ORB_Parameters::sock_sndbuf_size () const
{
  return sock_sndbuf_size_;
}

ACE_INLINE void
TAO_ORB_Parameters::sock_sndbuf_size (int s)
{
  sock_sndbuf_size_ = s;
}

ACE_INLINE int
TAO_ORB_Parameters::cdr_memcpy_tradeoff () const
{
  return this->cdr_memcpy_tradeoff_;
}

ACE_INLINE void
TAO_ORB_Parameters::cdr_memcpy_tradeoff (int x)
{
  this->cdr_memcpy_tradeoff_ = x;
}

ACE_INLINE ACE_CDR::ULong
TAO_ORB_Parameters::max_message_size () const
{
  return this->max_message_size_;
}

ACE_INLINE void
TAO_ORB_Parameters::max_message_size (ACE_CDR::ULong size)
{
  this->max_message_size_ = size;
}

ACE_INLINE int
TAO_ORB_Parameters::use_dotted_decimal_addresses () const
{
  return this->use_dotted_decimal_addresses_;
}

ACE_INLINE void
TAO_ORB_Parameters::use_dotted_decimal_addresses (int x)
{
  this->use_dotted_decimal_addresses_ = x;
}

ACE_INLINE int
TAO_ORB_Parameters::cache_incoming_by_dotted_decimal_address () const
{
  return this->cache_incoming_by_dotted_decimal_address_;
}

ACE_INLINE void
TAO_ORB_Parameters::cache_incoming_by_dotted_decimal_address (int x)
{
  this->cache_incoming_by_dotted_decimal_address_ = x;
}

ACE_INLINE int
TAO_ORB_Parameters::linger () const
{
  return this->linger_;
}

ACE_INLINE void
TAO_ORB_Parameters::linger (int x)
{
  this->linger_ = x;
}


ACE_INLINE time_t
TAO_ORB_Parameters::accept_error_delay () const
{
  return this->accept_error_delay_;
}

ACE_INLINE void
TAO_ORB_Parameters::accept_error_delay (time_t x)
{
  this->accept_error_delay_ = x;
}


ACE_INLINE void
TAO_ORB_Parameters::service_port (TAO::MCAST_SERVICEID service_id,
                                  CORBA::UShort port)
{
  this->service_port_[service_id] = port;
}

ACE_INLINE CORBA::UShort
TAO_ORB_Parameters::service_port (TAO::MCAST_SERVICEID service_id) const
{
  return this->service_port_[service_id];
}

ACE_INLINE void
TAO_ORB_Parameters::mcast_discovery_endpoint (const char *mde)
{
  this->mcast_discovery_endpoint_ = CORBA::string_dup (mde);
}

ACE_INLINE const char *
TAO_ORB_Parameters::mcast_discovery_endpoint () const
{
  return this->mcast_discovery_endpoint_.in ();
}

ACE_INLINE void
TAO_ORB_Parameters::default_init_ref (const char *default_init_ref)
{
  this->default_init_ref_ = default_init_ref;
}

ACE_INLINE char *
TAO_ORB_Parameters::default_init_ref () const
{
  return CORBA::string_dup (this->default_init_ref_.c_str ());
}

ACE_INLINE bool
TAO_ORB_Parameters::std_profile_components () const
{
  return this->std_profile_components_;
}

ACE_INLINE void
TAO_ORB_Parameters::std_profile_components (bool x)
{
  this->std_profile_components_ = x;
}

ACE_INLINE int
TAO_ORB_Parameters::nodelay () const
{
  return this->nodelay_;
}

ACE_INLINE void
TAO_ORB_Parameters::nodelay (int x)
{
  this->nodelay_ = x;
}

ACE_INLINE int
TAO_ORB_Parameters::sock_keepalive ()
{
  return this->sock_keepalive_;
}

ACE_INLINE void
TAO_ORB_Parameters::sock_keepalive (int x)
{
  this->sock_keepalive_ = x;
}

ACE_INLINE int
TAO_ORB_Parameters::sock_dontroute ()
{
  return this->sock_dontroute_;
}

ACE_INLINE void
TAO_ORB_Parameters::sock_dontroute (int x)
{
  this->sock_dontroute_ = x;
}

ACE_INLINE int
TAO_ORB_Parameters::ip_hoplimit ()
{
  return this->ip_hoplimit_;
}

ACE_INLINE void
TAO_ORB_Parameters::ip_hoplimit (int x)
{
  this->ip_hoplimit_ = x;
}

ACE_INLINE bool
TAO_ORB_Parameters::ip_multicastloop ()
{
  return this->ip_multicastloop_;
}

ACE_INLINE void
TAO_ORB_Parameters::ip_multicastloop (bool x)
{
  this->ip_multicastloop_ = x;
}

ACE_INLINE u_short
TAO_ORB_Parameters::iiop_client_port_base ()
{
  return this->iiop_client_port_base_;
}

ACE_INLINE void
TAO_ORB_Parameters::iiop_client_port_base (u_short b)
{
  this->iiop_client_port_base_ = b;
}

ACE_INLINE u_short
TAO_ORB_Parameters::iiop_client_port_span ()
{
  return this->iiop_client_port_base_ ?
    this->iiop_client_port_span_ : 0;
}

ACE_INLINE void
TAO_ORB_Parameters::iiop_client_port_span (u_short s)
{
  this->iiop_client_port_span_ = s;
}

ACE_INLINE int
TAO_ORB_Parameters::ace_sched_policy () const
{
  return this->ace_sched_policy_;
}

ACE_INLINE void
TAO_ORB_Parameters::ace_sched_policy (int x)
{
  this->ace_sched_policy_ = x;
}

ACE_INLINE long
TAO_ORB_Parameters::sched_policy () const
{
  return this->sched_policy_;
}

ACE_INLINE void
TAO_ORB_Parameters::sched_policy (long x)
{
  this->sched_policy_ = x;
}

ACE_INLINE long
TAO_ORB_Parameters::scope_policy () const
{
  return this->scope_policy_;
}

ACE_INLINE void
TAO_ORB_Parameters::scope_policy (long x)
{
  this->scope_policy_ = x;
}

ACE_INLINE long
TAO_ORB_Parameters::thread_creation_flags () const
{
  return
    this->scope_policy () |
    this->sched_policy ();
}

ACE_INLINE int
TAO_ORB_Parameters::single_read_optimization () const
{
  return this->single_read_optimization_;
}

ACE_INLINE void
TAO_ORB_Parameters::single_read_optimization (int x)
{
  this->single_read_optimization_ = x;
}

ACE_INLINE bool
TAO_ORB_Parameters::use_parallel_connects () const
{
  return this->use_parallel_connects_;
}

ACE_INLINE void
TAO_ORB_Parameters::use_parallel_connects (bool x)
{
  this->use_parallel_connects_ = x;
}

ACE_INLINE unsigned long
TAO_ORB_Parameters::parallel_connect_delay () const
{
  return this->parallel_connect_delay_;
}

ACE_INLINE void
TAO_ORB_Parameters::parallel_connect_delay (unsigned long x)
{
  this->parallel_connect_delay_ = x;
}

ACE_INLINE int
TAO_ORB_Parameters::shared_profile () const
{
  return this->shared_profile_;
}

ACE_INLINE void
TAO_ORB_Parameters::shared_profile (int x)
{
  this->shared_profile_ = x;
}

ACE_INLINE bool
TAO_ORB_Parameters::disable_rt_collocation_resolver () const
{
  return this->disable_rt_collocation_resolver_;
}

ACE_INLINE void
TAO_ORB_Parameters::disable_rt_collocation_resolver (bool x)
{
  this->disable_rt_collocation_resolver_ = x;
}


ACE_INLINE bool
TAO_ORB_Parameters::negotiate_codesets () const
{
  return this->negotiate_codesets_;
}

ACE_INLINE void
TAO_ORB_Parameters::negotiate_codesets (bool x)
{
  this->negotiate_codesets_ = x;
}

ACE_INLINE bool
TAO_ORB_Parameters::ami_collication () const
{
  return this->ami_collication_;
}

ACE_INLINE void
TAO_ORB_Parameters::ami_collication (bool x)
{
  this->ami_collication_ = x;
}

ACE_INLINE void
TAO_ORB_Parameters::collocation_resolver_name (const char *s)
{
  this->collocation_resolver_name_ = s;
}

ACE_INLINE const char *
TAO_ORB_Parameters::collocation_resolver_name () const
{
  return this->collocation_resolver_name_.c_str ();
}

ACE_INLINE bool
TAO_ORB_Parameters::forward_invocation_on_object_not_exist () const
{
  return this->forward_invocation_on_object_not_exist_;
}

ACE_INLINE void
TAO_ORB_Parameters::forward_invocation_on_object_not_exist (bool x)
{
  this->forward_invocation_on_object_not_exist_ = x;
}

ACE_INLINE int
TAO_ORB_Parameters::forward_once_exception () const
{
  return this->forward_once_exception_;
}

ACE_INLINE void
TAO_ORB_Parameters::forward_once_exception (const int ef)
{
  this->forward_once_exception_ |= ef;
}

ACE_INLINE bool
TAO_ORB_Parameters::allow_ziop_no_server_policies () const
{
  return this->allow_ziop_no_server_policies_;
}

ACE_INLINE void
TAO_ORB_Parameters::allow_ziop_no_server_policies (bool x)
{
  this->allow_ziop_no_server_policies_ = x;
}

TAO_END_VERSIONED_NAMESPACE_DECL
