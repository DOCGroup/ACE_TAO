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

#include "tao/corbafwd.h"

// Forward decls.

class TAO_Server_Connection_Handler;
class TAO_Active_Object_Map_Impl;
class TAO_Reverse_Active_Object_Map_Impl;
class TAO_IOR_LookupTable;

// This is a quick hack to avoid having to unravel the intracacies of
// the all the hairy order interdepencies that currently exist in TAO.
#if ! defined (__ACE_INLINE__)
#define TAO_LOCAL_INLINE
#else
#define TAO_LOCAL_INLINE ACE_INLINE
#endif /* ! __ACE_INLINE__ */

typedef ACE_Unbounded_Set<ACE_CString> TAO_EndpointSet;
typedef ACE_Unbounded_Set_Iterator<ACE_CString> TAO_EndpointSetIterator;

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

  int preconnects (ACE_CString &preconnects);
  TAO_EndpointSet &preconnects (void);
  void add_preconnect (ACE_CString &preconnect);
  // Specifies the endpoints to pre-establish connections on.

  int endpoints (ACE_CString &endpoints);
  TAO_EndpointSet &endpoints (void);
  void add_endpoint (ACE_CString &endpoint);
  // Specifies the endpoints on which this server is willing to
  // listen for requests.

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
  // The ORB will use a modified version of GIOP that minimizes the
  // header size. By default we use the standard GIOP protocol.

  int use_dotted_decimal_addresses (void) const;
  void use_dotted_decimal_addresses (int);
  // The ORB will use the dotted decimal notation for addresses. By
  // default we use the full ascii names.

  TAO_IOR_LookupTable * ior_lookup_table (void);
  void ior_lookup_table (TAO_IOR_LookupTable *table);
  // The table used by the ORB for looking up the ObjectID:IOR mappings
  // specified on the commandline through the -ORBInitRef and
  // -ORBDefaultInitRef parameters.

  char *default_init_ref (void) const;
  void default_init_ref (const ACE_CString &default_init_ref);
  // Set/Get the Init Reference of an arbitrary ObjectID.

private:
  // Each "endpoint" is of the form:
  //
  //   protocol:V.v//addr1,addr2,...,addrN/
  //
  // or:
  //
  //   protocol://addr1,addr2,...,addrN/
  //
  // where "V.v" is an optional version.
  //
  // Multiple sets of endpoint may be seperated by a semi-colon `;'.
  // For example:
  //
  //   iiop://space:2001,odyssey:2010/;uiop://foo,bar/
  //
  // All preconnect or endpoint strings should be of the above form(s).

  int parse_endpoints (ACE_CString &endpoints,
                       TAO_EndpointSet &endpoints_list);

  TAO_EndpointSet preconnects_list_;
  // List of endpoints used to pre-establish connections.

  TAO_EndpointSet endpoints_list_;
  // List of endpoints this server is willing to accept requests
  // on.

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

  TAO_IOR_LookupTable *ior_lookup_table_;
  // Table that has the mapping <ObjectID>:<IOR>
  // The IOR could be in any of the following formats :
  // IOR: ...  / iiop: ...  / iioploc: ... / iiopname: ...

  ACE_CString default_init_ref_;
  // List of comma separated prefixes from ORBDefaultInitRef.

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

#if defined (__ACE_INLINE__)
# include "tao/params.i"
#endif /* __ACE_INLINE__ */

#endif /* TAO_PARAMS_H */
