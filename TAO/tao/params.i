/* -*- C++ -*- */

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
TAO_OA_Parameters::demux_strategy (TAO_Demux_Strategy strategy)
{
  this->demux_ = strategy;      // Trust that the value is valid!
}

//ACE_INLINE TAO_OA_Parameters::TAO_Demux_Strategy
ACE_INLINE TAO_Demux_Strategy
TAO_OA_Parameters::demux_strategy (void)
{
  return this->demux_;
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

ACE_INLINE void 
TAO_OA_Parameters::userdef_lookup_strategy (TAO_Object_Table_Impl *&ot)
{
  // provide a way for user defined object key lookup strategies to be plugged
  // in
  ot_ = ot;
  ot = 0;  // we own it now
}

ACE_INLINE TAO_Object_Table_Impl *
TAO_OA_Parameters::userdef_lookup_strategy (void)
{
  // return the lookup strategy
  return ot_;
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
