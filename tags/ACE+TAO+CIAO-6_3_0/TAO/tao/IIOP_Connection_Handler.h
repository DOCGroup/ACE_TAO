// -*- C++ -*-

// ===================================================================
/**
 *  @file   IIOP_Connection_Handler.h
 *
 *  $Id$
 *
 *  @author Originally by Chris Cleeland as IIOP_Connect.h
 *  @author Balachandran Natarajan <bala@cs.wustl.edu>
 */
// ===================================================================

#ifndef TAO_IIOP_CONNECTION_HANDLER_H
#define TAO_IIOP_CONNECTION_HANDLER_H

#include /**/ "ace/pre.h"

#include "tao/orbconf.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if defined (TAO_HAS_IIOP) && (TAO_HAS_IIOP != 0)

#include "tao/Connection_Handler.h"
#include "tao/Basic_Types.h"

#include "ace/SOCK_Stream.h"
#include "ace/Svc_Handler.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

/// Service Handler for this transport
typedef ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_NULL_SYNCH>
        TAO_IIOP_SVC_HANDLER;

#if defined ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION_EXPORT
template class TAO_Export ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_NULL_SYNCH>;
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION_EXPORT */

namespace IIOP
{
  class ListenPointList;
}

// ****************************************************************

/**
 * @class TAO_IIOP_Connection_Handler
 *
 * @brief  Handles requests on a single connection.
 *
 * The Connection handler which is common for the Acceptor and
 * the Connector
 */
class TAO_Export TAO_IIOP_Connection_Handler : public TAO_IIOP_SVC_HANDLER,
                                               public TAO_Connection_Handler
{

public:

#ifdef TAO_LOG_CH_REF_COUNTS
  Reference_Count add_reference (void);
  Reference_Count remove_reference (void);
#endif

  TAO_IIOP_Connection_Handler (ACE_Thread_Manager * = 0);

  /// Constructor.
  TAO_IIOP_Connection_Handler (TAO_ORB_Core *orb_core);

  /// Destructor.
  ~TAO_IIOP_Connection_Handler (void);

  /// Called by the @c Strategy_Acceptor when the handler is completely
  /// connected.  Argument is unused.
  virtual int open (void *);

  /// Close called by the Acceptor or Connector when connection
  /// establishment fails.
  int close (u_long = 0);

  //@{
  /** @name Event Handler overloads
   */
  virtual int resume_handler (void);
  virtual int close_connection (void);
  virtual int handle_input (ACE_HANDLE);
  virtual int handle_output (ACE_HANDLE);
  virtual int handle_close (ACE_HANDLE, ACE_Reactor_Mask);
  virtual int handle_timeout (const ACE_Time_Value &current_time,
                              const void *act = 0);
  //@}

  /// Add ourselves to Cache.
  int add_transport_to_cache (void);

  /// Process the @a listen_list
  int process_listen_point_list (IIOP::ListenPointList &listen_list);

  /// Check if network priority needs to be enabled
  int enable_network_priority (void);

  /// Set Diff-Serv codepoint on outgoing packets.
  int set_dscp_codepoint (CORBA::Boolean set_network_priority);
  int set_dscp_codepoint (CORBA::Long dscp_codepoint);

  virtual int open_handler (void *);

  /// This is used during a canceled connection attempt. Force the
  /// SO_LINGER timeout to 0 so that when the peer is closed, it won't
  /// hang around.
  void abort (void);

protected:

  //@{
  /**
   * @name TAO_Connection Handler overloads
   */
  virtual int release_os_resources (void);
  //@}

  virtual int handle_write_ready (const ACE_Time_Value *timeout);

  /// helper function used by the set_dscp_codepoint () methods to
  /// set the TOS field in the IP packets.
  int set_tos (int tos);

private:
  /// Stores the type of service value.
  int dscp_codepoint_;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#endif /* TAO_HAS_IIOP && TAO_HAS_IIOP != 0 */

#include /**/ "ace/post.h"
#endif /* TAO_IIOP_CONNECTION_HANDLER_H */
