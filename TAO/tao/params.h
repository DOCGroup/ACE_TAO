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

#ifndef TAO_PARAMS_H
#define TAO_PARAMS_H

#include "ace/INET_Addr.h"
#include "tao/corbafwd.h"

// Forward decls.

class TAO_Server_Connection_Handler;
class TAO_Active_Object_Map_Impl;
class TAO_Reverse_Active_Object_Map_Impl;

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

  const ACE_INET_Addr &addr (void) const;
  void addr (const ACE_INET_Addr &addr);
  // Set/Get the address on which we're listening.

  const char *host (void) const;
  void host (const ACE_CString &host);
  // Set/Get the hostname.

  const char *init_ref (void) const;
  void init_ref (const ACE_CString &init_ref);
  // Set/Get the Init Reference of an arbitrary ObjectID.

  const char *name_service_ior (void) const;
  void name_service_ior (const ACE_CString &ns);
  // Set/Get the IOR of our name service.

  CORBA::UShort name_service_port (void) const;
  void name_service_port (CORBA::UShort port);
  // Set/Get the port of our name service.

  const char *trading_service_ior (void) const;
  void trading_service_ior (const ACE_CString &ns);
  // Set/Get the IOR of our trading service.

  CORBA::UShort trading_service_port (void) const;
  void trading_service_port (CORBA::UShort port);
  // Set/Get the port of our trading service.

  int sock_rcvbuf_size (void) const;
  void sock_rcvbuf_size (int);
  // Set/Get the size to be used for a socket's receive buffer.

  int sock_sndbuf_size (void) const;
  void sock_sndbuf_size (int);
  // Set/Get the size to be used for a socket's send buffer.

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

  int use_lite_protocol (void) const;
  void use_lite_protocol (int);
  // The ORB will use a modified version of IIOP that minimizes the
  // header size. By default we use the standard IIOP protocol.

  int use_dotted_decimal_addresses (void) const;
  void use_dotted_decimal_addresses (int);
  // The ORB will use the dotted decimal notation for addresses. By
  // default we use the full ascii names.

private:
  ACE_INET_Addr addr_;
  // host + port number we are listening on

  ACE_CString host_;
  // host name

  ACE_CString name_service_ior_;
  // The IOR of our configured Naming Service.

  CORBA::UShort name_service_port_;
  // The port number of our configured Naming Service.

  ACE_CString trading_service_ior_;
  // The IOR of our configured Trading Service.

  CORBA::UShort trading_service_port_;
  // The port number of our configured Trading Service.

  ACE_CString init_ref_;
  // Initial Reference supplied as <ObjectID>:<IOR>

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

  int use_lite_protocol_;
  // For selecting a liteweight version of the GIOP protocol.

  int use_dotted_decimal_addresses_;
  // For selecting a address notation
};

enum TAO_Demux_Strategy
{
  TAO_LINEAR,
  TAO_DYNAMIC_HASH,
  TAO_ACTIVE_DEMUX,
  TAO_USER_DEFINED
};

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

  TAO_LOCAL_INLINE TAO_Active_Object_Map_Impl *userdef_lookup_strategy_for_user_id_policy (void);
  // return the lookup strategy for the user id policy

  TAO_LOCAL_INLINE TAO_Active_Object_Map_Impl *userdef_lookup_strategy_for_system_id_policy (void);
  // return the lookup strategy for the system id policy

  TAO_LOCAL_INLINE TAO_Reverse_Active_Object_Map_Impl *userdef_reverse_lookup_strategy_for_unique_id_policy (void);
  // return the reverse lookup strategy for unique id policy

  TAO_LOCAL_INLINE void tablesize (CORBA::ULong tablesize);
  // set the table size for lookup table

  TAO_LOCAL_INLINE CORBA::ULong tablesize (void);
  // get the table size for the lookup table

private:
  CORBA::ULong tablesize_;
  // size of object lookup table
};

#if defined (__ACE_INLINE__)
# include "tao/params.i"
#endif /* __ACE_INLINE__ */

#endif /* TAO_PARAMS_H */
