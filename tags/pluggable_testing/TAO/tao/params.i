/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO
//
// = FILENAME
//    params.i
//
// = AUTHOR
//    Chris Cleeland
//
// ============================================================================

ACE_INLINE int
TAO_ORB_Parameters::sock_rcvbuf_size (void) const
{
  return sock_rcvbuf_size_;
}

ACE_INLINE void
TAO_ORB_Parameters::sock_rcvbuf_size (int s)
{
  sock_rcvbuf_size_ = s <= ACE_DEFAULT_MAX_SOCKET_BUFSIZ ? s : ACE_DEFAULT_MAX_SOCKET_BUFSIZ;
}

ACE_INLINE int
TAO_ORB_Parameters::sock_sndbuf_size (void) const
{
  return sock_sndbuf_size_;
}

ACE_INLINE void
TAO_ORB_Parameters::sock_sndbuf_size (int s)
{
  sock_sndbuf_size_ = s <= ACE_DEFAULT_MAX_SOCKET_BUFSIZ ? s : ACE_DEFAULT_MAX_SOCKET_BUFSIZ;
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
TAO_ORB_Parameters::addr (const ACE_INET_Addr &addr)
{
  this->addr_ = addr;
}

ACE_INLINE const ACE_INET_Addr &
TAO_ORB_Parameters::addr (void) const
{
  return this->addr_;
}

ACE_INLINE void
TAO_ORB_Parameters::init_ref (const ACE_CString &init_ref)
{
  this->init_ref_ = init_ref;
}

ACE_INLINE const char *
TAO_ORB_Parameters::init_ref (void) const
{
  return this->init_ref_.c_str ();
}

ACE_INLINE void
TAO_ORB_Parameters::name_service_ior (const ACE_CString &ns)
{
  this->name_service_ior_ = ns;
}

ACE_INLINE const char *
TAO_ORB_Parameters::name_service_ior (void) const
{
  return this->name_service_ior_.c_str ();
}

ACE_INLINE void
TAO_ORB_Parameters::host (const ACE_CString &h)
{
  this->host_ = h;
}

ACE_INLINE const char *
TAO_ORB_Parameters::host (void) const
{
  return this->host_.c_str ();
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
TAO_ORB_Parameters::trading_service_ior (const ACE_CString &ns)
{
  this->trading_service_ior_ = ns;
}

ACE_INLINE const char *
TAO_ORB_Parameters::trading_service_ior (void) const
{
  return this->trading_service_ior_.c_str ();
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

ACE_INLINE TAO_IOR_LookupTable *
TAO_ORB_Parameters::ior_lookup_table (void)
{
  return this->ior_lookup_table_;
}

ACE_INLINE void
TAO_ORB_Parameters::ior_lookup_table (TAO_IOR_LookupTable *table)
{
  this->ior_lookup_table_ = table;
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
