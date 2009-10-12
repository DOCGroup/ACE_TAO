// -*- C++ -*-

// $Id$
// ===================================================================
/**
 *  @file   SCIOP_Connection_Handler.h
 *
 *  @author  Jason Cohen, Lockheed Martin ATL  <jcohen@atl.lmco.com>
 *  @author  Keith O'Hara, Lockheed Martin ATL
 */
// ===================================================================

#ifndef TAO_SCIOP_CONNECTION_HANDLER_H
#define TAO_SCIOP_CONNECTION_HANDLER_H

#include /**/ "ace/pre.h"

#include "tao/orbconf.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if TAO_HAS_SCIOP == 1

#include "tao/Strategies/strategies_export.h"
#include "tao/Connection_Handler.h"
#include "tao/IIOPC.h"
#include "tao/TimeBaseC.h"

#include "ace/Reactor.h"
#include "ace/Svc_Handler.h"
#include "ace/SOCK_SEQPACK_Association.h"

#if defined ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION_EXPORT
template class TAO_Strategies_Export ACE_Svc_Handler<ACE_SOCK_SEQPACK_ASSOCIATION, ACE_NULL_SYNCH>;
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION_EXPORT */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

// Service Handler for this transport
typedef ACE_Svc_Handler<ACE_SOCK_SEQPACK_ASSOCIATION, ACE_NULL_SYNCH>
        TAO_SCIOP_SVC_HANDLER;

// ****************************************************************

/**
 * @class TAO_SCIOP_Connection_Handler
 *
 * @brief  Handles requests on a single connection.
 *
 * The Connection handler which is common for the Acceptor and
 * the Connector
 */


class TAO_Strategies_Export TAO_SCIOP_Connection_Handler
  : public TAO_SCIOP_SVC_HANDLER,
    public TAO_Connection_Handler
{

public:

  TAO_SCIOP_Connection_Handler (ACE_Thread_Manager* t = 0);

  /// Constructor.
  TAO_SCIOP_Connection_Handler (TAO_ORB_Core *orb_core);

  /// Destructor.
  ~TAO_SCIOP_Connection_Handler (void);

  //@{
  /**
   * Connection_Handler overloads
   */
  virtual int open_handler (void *);
  //@}

  /// Called by the <Strategy_Acceptor> when the handler is completely
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

  /// Process the <listen_list>
  int process_listen_point_list (IIOP::ListenPointList &listen_list);

  /// Set Diff-Serv codepoint on outgoing packets.
  int set_dscp_codepoint (CORBA::Boolean set_network_priority);
  int set_dscp_codepoint (CORBA::Long dscp);

protected:

  /// Helper method needed by the set_dscp_codepoint () methods
  int set_tos (int tos);

  //@{
  /**
   * @name TAO_Connection Handler overloads
   */
  //  void handle_close_i (void);
  virtual int release_os_resources (void);
  virtual int handle_write_ready (const ACE_Time_Value *timeout);
  //@}

private:

  /// Stores the type of service value.
  int dscp_codepoint_;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#endif /* TAO_HAS_SCIOP == 1 */

#include /**/ "ace/post.h"

#endif /* TAO_SCIOP_CONNECTION_HANDLER_H */
