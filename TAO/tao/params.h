/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO
//
// = FILENAME
//    params.h
//
// = AUTHOR
//    Chris Cleeland
//
// ============================================================================

#if !defined (TAO_PARAMS_H)
#  define TAO_PARAMS_H

// Forward decls.

class TAO_Server_Connection_Handler;

// This is a quick hack to avoid having to unravel the intracacies of
// the all the hairy order interdepencies that currently exist in TAO.
#if ! defined (__ACE_INLINE__)
#define TAO_LOCAL_INLINE
#else
#define TAO_LOCAL_INLINE ACE_INLINE
#endif /* ! __ACE_INLINE__ */

class TAO_Export TAO_ORB_Parameters
  // = TITLE
  //    Parameters that are specific to the ORB.  These parameters can
  //    be for the client, the server, or for both.
  //
  // = NOTE
  // Some of the functions have inline definitions in the class header
  // below.  Please do not move them back to the .i file.
  // cygnus-2.7.2-960126, distributed with Tornado 1.0.1, can't deal
  // with returing a const reference here.  It raises an internal
  // compiler error at ace/Svc_Handler.cpp:46, of all places.
{
public:
  TAO_ORB_Parameters (void);
  // Constructor.

  ~TAO_ORB_Parameters (void);
  // Destructor.

  void addr (const ACE_INET_Addr &addr);
  // Set the address on which we're listening.

  const ACE_INET_Addr &addr (void);
  // Get the address on which we're listening.

  void name_service_ior (CORBA::String ns);
  // Set the IOR of our name service.

  CORBA::String name_service_ior (void);
  // Get the IOR of our name service.

  void name_service_port (CORBA::UShort port);
  // Set the port of our name service.

  CORBA::UShort name_service_port (void);
  // Get the port of our name service.

  void trading_service_ior (CORBA::String ns);
  // Set the IOR of our trading service.

  CORBA::String trading_service_ior (void);
  // Get the IOR of our trading service.

  void trading_service_port (CORBA::UShort port);
  // Set the port of our trading service.

  CORBA::UShort trading_service_port (void);
  // Get the port of our trading service.

  int sock_rcvbuf_size (void);
  // Get the size to be used for a socket's receive buffer.

  void sock_rcvbuf_size (int);
  // Set the size to be used for a socket's receive buffer.

  int sock_sndbuf_size (void);
  // Get the size to be used for a socket's send buffer.

  void sock_sndbuf_size (int);
  // Set the size to be used for a socket's send buffer.

  int cdr_default_size (void) const;
  void cdr_default_size (int);
  // If the user does not specify the size of a CDR stream this is the
  // size of its internal buffer.

  int cdr_max_exp_size (void) const;
  void cdr_max_exp_size (int);
  // CDR streams internal buffers grow exponentially until they reach
  // this size.

  int cdr_linear_chunk (void) const;
  void cdr_linear_chunk (int);
  // After reaching their maximum exponential size limit CDR streams
  // grow linearly in chunks of this size.

  int cdr_memcpy_tradeoff (void) const;
  void cdr_memcpy_tradeoff (int);
  // Octet sequences are marshalled without doing any copies, we
  // simply append a block to the CDR message block chain. When the
  // octet sequence is small enough and there is room in the current
  // message block it is more efficient just to copy the buffer.

private:
  ACE_INET_Addr addr_;
  // host + port number we are listening on

  CORBA::String name_service_ior_;
  // The IOR of our configured Naming Service.

  CORBA::UShort name_service_port_;
  // The port number of our configured Naming Service.

  CORBA::String trading_service_ior_;
  // The IOR of our configured Trading Service.

  CORBA::UShort trading_service_port_;
  // The port number of our configured Trading Service.

  int sock_rcvbuf_size_;
  // Size to be used for a socket's receive buffer.

  int sock_sndbuf_size_;
  // Size to be used for a socket's send buffer.

  int cdr_default_size_;
  // Default size for CDR buffers.

  int cdr_max_exp_size_;
  // Cutoff value for exponential growth of CDR buffers.

  int cdr_linear_chunk_;
  // Control for linear growth of CDR buffers.

  int cdr_memcpy_tradeoff_;
  // Control the strategy for copying vs. appeding octet sequences in
  // CDR streams.
};

typedef enum
{
  TAO_NONE,
  TAO_LINEAR,
  TAO_DYNAMIC_HASH,
  TAO_ACTIVE_DEMUX,
  TAO_USER_DEFINED
} TAO_Demux_Strategy;

class TAO_Export TAO_OA_Parameters
  // = TITLE
  //    Parameters specific to an Object Adapter.  By definition, this
  //    is only on the server side, since a client does not have an
  //    object adapter.
  //
  // = NOTES
  //    This can be subclassed in order to have OA-specific
  //    parameters, e.g., the Realtime Object Adapter might subclass
  //    this and add its own parameters.
{
public:
  TAO_LOCAL_INLINE TAO_OA_Parameters (void);
  // Constructor

  TAO_LOCAL_INLINE ~TAO_OA_Parameters (void);
  // Destructor

  void demux_strategy (const char *strategy);
  // Specify the demultiplexing strategy to be used via <{strategy}>.
  // Valid values are one of (case matters) "linear", "dynamic_hash",
  // "user_def", or "active_demux".  If the value is not valid, then
  // <Dynamic Hash> is used as a default.

  TAO_LOCAL_INLINE void demux_strategy (TAO_Demux_Strategy s);
  // Specify the demultiplexing strategy to be used.

  TAO_LOCAL_INLINE TAO_Demux_Strategy demux_strategy (void);
  // Return the demultiplexing strategy being used.

  TAO_LOCAL_INLINE void userdef_lookup_strategy (TAO_Object_Table_Impl *&ot);
  // Provide a way for user defined object key lookup strategies to be
  // plugged in.

  TAO_LOCAL_INLINE TAO_Object_Table_Impl *userdef_lookup_strategy (void);
  // return the lookup strategy

  TAO_LOCAL_INLINE void tablesize (CORBA::ULong tablesize);
  // set the table size for lookup table

  TAO_LOCAL_INLINE CORBA::ULong tablesize (void);
  // get the table size for the lookup table

private:
  TAO_Demux_Strategy demux_;
  // demux strategy

  CORBA::ULong tablesize_;
  // size of object lookup table

  TAO_Object_Table_Impl *ot_;
  // concrete lookup table instance
};

#if defined (__ACE_INLINE__)
# include "tao/params.i"
#endif /* __ACE_INLINE__ */

#endif /* TAO_PARAMS_H */
