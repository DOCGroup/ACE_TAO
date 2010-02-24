// -*- C++ -*-

/**
 * @file ECG_Adapters.h
 *
 * $Id$
 *
 * @author Marina Spivak <marina@atdesk.com>
 *
 * Define interfaces ECG Receivers and Mcast and UDP handlers can use
 * to communicate with each other.
 * Use of these interfaces eliminates the coupling of specific
 * Receivers on specific handlers, and vice versa.
 *
 */
#ifndef TAO_ECG_ADAPTERS_H
#define TAO_ECG_ADAPTERS_H
#include /**/ "ace/pre.h"

#include /**/ "orbsvcs/Event/event_serv_export.h"
#include "orbsvcs/RtecUDPAdminC.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Refcounted_Auto_Ptr.h"

ACE_BEGIN_VERSIONED_NAMESPACE_DECL
class ACE_SOCK_Dgram;
ACE_END_VERSIONED_NAMESPACE_DECL

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

/**
 * @class TAO_ECG_Handler_Shutdown
 *
 * @brief Interface that should be implemented
 *        by multicast and udp handlers in EC gateways, so the
 *        handlers can be notified of shutdown (usually by Event Receivers).
 */
class TAO_RTEvent_Serv_Export TAO_ECG_Handler_Shutdown
{
public:

  // Define virtual destructor to make sure we can delete derived
  // objects through a pointer to TAO_ECG_Handler_Shutdown.
  virtual ~TAO_ECG_Handler_Shutdown (void);
  virtual int shutdown (void) = 0;
};

/**
 * @class TAO_ECG_Dgram_Handler
 *
 * @brief Interface used by mcast/udp handlers to notify interested
 *        components (usually Event Receivers) that there is data
 *        available on the dgram for reading, and to query them about
 *        event type to mcast group mappings.
 */
class TAO_RTEvent_Serv_Export TAO_ECG_Dgram_Handler
{
public:

  virtual ~TAO_ECG_Dgram_Handler (void);

  virtual int handle_input (ACE_SOCK_Dgram& dgram) = 0;

  /// Call the RtecUDPAdmin::AddrServer::get_addr.  Throws exception
  /// if nil Address Server was specified in init ().
  virtual void get_addr (const RtecEventComm::EventHeader& header,
                 RtecUDPAdmin::UDP_Addr_out addr) = 0;

  /// Call the RtecUDPAdmin::AddrServer::get_address.  Throws exception
  /// if nil Address Server was specified in init ().
  virtual void get_address (const RtecEventComm::EventHeader& header,
                               RtecUDPAdmin::UDP_Address_out addr) = 0;
};

typedef ACE_Refcounted_Auto_Ptr<TAO_ECG_Handler_Shutdown,
  ACE_Null_Mutex> TAO_ECG_Refcounted_Handler;

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"
#endif /* TAO_ECG_ADAPTERS_H */
