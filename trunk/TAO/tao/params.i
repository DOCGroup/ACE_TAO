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

ACE_INLINE
TAO_OA_Parameters::TAO_OA_Parameters (void)
{
}

ACE_INLINE
TAO_OA_Parameters::~TAO_OA_Parameters (void)
{
}

ACE_INLINE void
TAO_OA_Parameters::tablesize (CORBA::ULong tblsize)
{
  this->tablesize_ = tblsize;
}

ACE_INLINE CORBA::ULong
TAO_OA_Parameters::tablesize (void)
{
  return this->tablesize_;
}

ACE_INLINE TAO_Active_Object_Map_Impl *
TAO_OA_Parameters::userdef_lookup_strategy_for_user_id_policy (void)
{
  return 0;
}

ACE_INLINE TAO_Active_Object_Map_Impl *
TAO_OA_Parameters::userdef_lookup_strategy_for_system_id_policy (void)
{
  return 0;
}

ACE_INLINE TAO_Reverse_Active_Object_Map_Impl *
TAO_OA_Parameters::userdef_reverse_lookup_strategy_for_unique_id_policy (void)
{
  return 0;
}

ACE_INLINE int
TAO_ORB_Parameters::sock_rcvbuf_size (void)
{
  return sock_rcvbuf_size_;
}

ACE_INLINE void
TAO_ORB_Parameters::sock_rcvbuf_size (int s)
{
  sock_rcvbuf_size_ = s <= ACE_DEFAULT_MAX_SOCKET_BUFSIZ ? s : ACE_DEFAULT_MAX_SOCKET_BUFSIZ;
}

ACE_INLINE int
TAO_ORB_Parameters::sock_sndbuf_size (void)
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
TAO_ORB_Parameters::use_IIOP_lite_protocol (void) const
{
  return this->use_IIOP_lite_protocol_;
}

ACE_INLINE void
TAO_ORB_Parameters::use_IIOP_lite_protocol (int x)
{
  this->use_IIOP_lite_protocol_ = x;
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

