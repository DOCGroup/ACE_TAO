// -*- C++ -*-

// ===================================================================
/**
 *  @file   DIOP_Connection_Handler.h
 *
 *  $Id$
 *
 *  @author  Michael Kircher
 */
// ===================================================================

#ifndef TAO_DIOP_CONNECTION_HANDLER_H
#define TAO_DIOP_CONNECTION_HANDLER_H

#include /**/ "ace/pre.h"

#include "tao/orbconf.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if defined (TAO_HAS_DIOP) && (TAO_HAS_DIOP != 0)

#include "tao/Strategies/strategies_export.h"
#include "tao/Wait_Strategy.h"
#include "tao/Connection_Handler.h"
#include "tao/Strategies/DIOP_Transport.h"
#include "ace/SOCK_Dgram.h"
#include "ace/Reactor.h"
#include "ace/Acceptor.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

// ****************************************************************

/**
 * @class TAO_DIOP_Connection_Handler
 *
 * @brief  Handles requests on a single connection.
 *
 * The Connection handler which is common for the Acceptor and
 * the Connector
 */
class TAO_Strategies_Export TAO_DIOP_Connection_Handler
  : public TAO_DIOP_SVC_HANDLER
  , public TAO_Connection_Handler
{

public:

  TAO_DIOP_Connection_Handler (ACE_Thread_Manager* t = 0);

  /// Constructor.
  TAO_DIOP_Connection_Handler (TAO_ORB_Core *orb_core);

  /// Destructor.
  ~TAO_DIOP_Connection_Handler (void);

  /// Called by the <Strategy_Acceptor> when the handler is completely
  /// connected.  Argument is unused.
  virtual int open (void *);

  // @@ Frank: Similar to open, but called on server
  virtual int open_server (void);

  //@{
  /**
   * Connection_Handler overloads
   */
  virtual int open_handler (void *);
  //@}

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

  // @@ Frank: Not needed
  /*
  /// Process the <listen_list>
  int process_listen_point_list (DIOP::ListenPointList &listen_list);
  */

  // DIOP Additions - Begin
  /**
   * @name The DIOP Additions
   *
   */
  //@{
  const ACE_INET_Addr &addr (void);

  void addr (const ACE_INET_Addr &addr);

  const ACE_INET_Addr &local_addr (void);

  void local_addr (const ACE_INET_Addr &addr);
  //@}
  // DIOP Additions - End

  /// Set Diff-Serv codepoint on outgoing packets.
  int set_dscp_codepoint (CORBA::Boolean set_network_priority);
  int set_dscp_codepoint (CORBA::Long dscp_codepoint);

protected:

  //@{
  /**
   * @name TAO_Connection Handler overloads
   */
  virtual int release_os_resources (void);
  //@}

  /// Helper method used by the set_dscp_codepoint () methods
  int set_tos (int tos);

  // DIOP Additions - Begin
  /**
   * @name The DIOP Additions
   *
   */
  //@{
  /// This is always the remote address
  ACE_INET_Addr addr_;

  /// This is always the local address for the connector
  ACE_INET_Addr local_addr_;
  //@}
  // DIOP Additions - End

private:
  /// Stores the type of service value.
  int dscp_codepoint_;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#endif /* TAO_HAS_DIOP && TAO_HAS_DIOP != 0 */

#include /**/ "ace/post.h"

#endif /* TAO_DIOP_CONNECTION_HANDLER_H */
