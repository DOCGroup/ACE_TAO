// -*- C++ -*-
// $Id$

ACE_INLINE int
TAO_ORB_Parameters::preconnects (ACE_CString &preconnects)
{
  return this->parse_endpoints (preconnects,
                                this->preconnect_insertion_strategy_);
}

ACE_INLINE TAO_PreconnectSet &
TAO_ORB_Parameters::preconnects (void)
{
  return this->preconnects_list_;
}

ACE_INLINE void
TAO_ORB_Parameters::add_preconnect (ACE_CString &preconnect)
{
  this->preconnects_list_.enqueue_tail (preconnect);
}

ACE_INLINE int
TAO_ORB_Parameters::endpoints (ACE_CString &endpoints)
{
  return this->parse_endpoints (endpoints,
                                this->endpoint_insertion_strategy_);
}

ACE_INLINE TAO_EndpointSet &
TAO_ORB_Parameters::endpoints (void)
{
  return this->endpoints_list_;
}

ACE_INLINE void
TAO_ORB_Parameters::add_endpoint (ACE_CString &endpoint)
{
  this->endpoints_list_.insert (endpoint);
}

ACE_INLINE int
TAO_ORB_Parameters::sock_rcvbuf_size (void) const
{
  return sock_rcvbuf_size_;
}

ACE_INLINE void
TAO_ORB_Parameters::sock_rcvbuf_size (int s)
{
  sock_rcvbuf_size_ = s <= ACE_DEFAULT_MAX_SOCKET_BUFSIZ 
    ? s 
    : ACE_DEFAULT_MAX_SOCKET_BUFSIZ;
}

ACE_INLINE int
TAO_ORB_Parameters::sock_sndbuf_size (void) const
{
  return sock_sndbuf_size_;
}

ACE_INLINE void
TAO_ORB_Parameters::sock_sndbuf_size (int s)
{
  sock_sndbuf_size_ = s <= ACE_DEFAULT_MAX_SOCKET_BUFSIZ 
    ? s 
    : ACE_DEFAULT_MAX_SOCKET_BUFSIZ;
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
TAO_ORB_Parameters::use_lite_protocol (void) const
{
  return this->use_lite_protocol_;
}

ACE_INLINE void
TAO_ORB_Parameters::use_lite_protocol (int x)
{
  this->use_lite_protocol_ = x;
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
TAO_ORB_Parameters::name_service_port (CORBA::UShort port)
{
  this->name_service_port_ = port;
}

ACE_INLINE CORBA::UShort
TAO_ORB_Parameters::name_service_port (void) const
{
  return this->name_service_port_;
}

ACE_INLINE void
TAO_ORB_Parameters::trading_service_port (CORBA::UShort port)
{
  this->trading_service_port_ = port;
}

ACE_INLINE CORBA::UShort
TAO_ORB_Parameters::trading_service_port (void) const
{
  return this->trading_service_port_;
}

ACE_INLINE void
TAO_ORB_Parameters::implrepo_service_port (CORBA::UShort port)
{
  this->implrepo_service_port_ = port;
}

ACE_INLINE CORBA::UShort
TAO_ORB_Parameters::implrepo_service_port (void) const
{
  return this->implrepo_service_port_;
}

ACE_INLINE void
TAO_ORB_Parameters::mcast_discovery_endpoint (const ACE_CString &mde)
{
  this->mcast_discovery_endpoint_ = mde;
}

ACE_INLINE const char *
TAO_ORB_Parameters::mcast_discovery_endpoint (void) const
{
  return this->mcast_discovery_endpoint_.c_str ();
}

ACE_INLINE TAO_IOR_LookupTable *
TAO_ORB_Parameters::ior_lookup_table (void)
{
  return &(this->ior_lookup_table_);
}

ACE_INLINE void
TAO_ORB_Parameters::default_init_ref (const ACE_CString &default_init_ref)
{
  this->default_init_ref_ = default_init_ref;
}

ACE_INLINE char *
TAO_ORB_Parameters::default_init_ref (void) const
{
  return this->default_init_ref_.rep ();
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

