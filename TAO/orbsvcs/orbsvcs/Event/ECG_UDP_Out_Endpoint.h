/* -*- C++ -*- */
/**
 *  @file ECG_UDP_Out_Endpoint.h
 *
 *  $Id$
 *
 *  @author Carlos O'Ryan (coryan@cs.wustl.edu)
 *  @author Marina Spivak (marina@atdesk.com)
 */

#ifndef TAO_ECG_UDP_OUT_ENDPOINT_H
#define TAO_ECG_UDP_OUT_ENDPOINT_H
#include "ace/pre.h"

#include "event_export.h"
#include "tao/corbafwd.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Refcounted_Auto_Ptr.h"
#include "ace/SOCK_Dgram.h"
#include "ace/Atomic_Op.h"

class ACE_INET_Addr;

/**
 * @class TAO_ECG_UDP_Out_Endpoint
 *
 * @brief Maintains information about an outgoing endpoint.
 *
 * UDP senders can share a single endpoint to send UDP packets,
 * but there is more state associated with this endpoint than its
 * datagram SAP; for instance we need to keep track of the request
 * id.
 */
class TAO_RTEvent_Export TAO_ECG_UDP_Out_Endpoint
{
public:
  /// Constructor
  TAO_ECG_UDP_Out_Endpoint (void);

  /// Constructor
  ~TAO_ECG_UDP_Out_Endpoint (void);

  /// Obtain the datagram associated with this endpoint. Clients of
  /// this class must open, and register (if necessary) this datagram.
  ACE_SOCK_Dgram& dgram (void);

  /// Obtain the next request id.
  CORBA::ULong next_request_id (void);

  /// The endpoint can detect if a data-gram was sent by itself, this
  /// is useful to ignore or remove messages sent by the same process.
  CORBA::Boolean is_loopback (const ACE_INET_Addr& from);

private:
  /// The request id....
  ACE_Atomic_Op<TAO_SYNCH_MUTEX,CORBA::ULong> request_id_generator_;

  /// The datagram....
  ACE_SOCK_Dgram dgram_;

  /// Cache the port-number so we can quickly determine if an event is
  /// coming from another endpoint.
  u_short port_number_;

  /// Keep the list of local interfaces, needed for the is_loopback
  /// method.
  size_t if_count_;
  ACE_INET_Addr* ifs_;
};

/**
 * @typedef TAO_ECG_Refcounted_Endpoint
 *
 * @brief Reference counted pointer to TAO_ECG_UDP_Out_Endpoint
 *
 * Instances of TAO_ECG_UDP_Out_Endpoint are used by multiple objects
 * in the Event Channel gateways, the endpoints are reference counted
 * to manage their lifetime automatically.
 */
typedef ACE_Refcounted_Auto_Ptr<TAO_ECG_UDP_Out_Endpoint,ACE_Null_Mutex> TAO_ECG_Refcounted_Endpoint;

#if defined(__ACE_INLINE__)
#include "ECG_UDP_Out_Endpoint.i"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* TAO_ECG_UDP_OUT_ENDPOINT_H */
