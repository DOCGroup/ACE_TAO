// -*- C++ -*-

// ===================================================================
/**
 *  @file   COIOP_Connection_Handler.h
 *
 *  @author  Johnny Willemsen <jwillemsen@remedy.nl>
 */
// ===================================================================

#ifndef TAO_COIOP_CONNECTION_HANDLER_H
#define TAO_COIOP_CONNECTION_HANDLER_H

#include /**/ "ace/pre.h"

#include "tao/orbconf.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if defined (TAO_HAS_COIOP) && (TAO_HAS_COIOP != 0)

#include "tao/Strategies/strategies_export.h"
#include "tao/Wait_Strategy.h"
#include "tao/Connection_Handler.h"
#include "tao/Strategies/COIOP_Transport.h"
#include "ace/Reactor.h"
#include "ace/Acceptor.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

// ****************************************************************

/**
 * @class TAO_COIOP_Connection_Handler
 *
 * @brief  Handles requests on a single connection.
 *
 * The Connection handler which is common for the Acceptor and
 * the Connector
 */
class TAO_Strategies_Export TAO_COIOP_Connection_Handler
  : public TAO_COIOP_SVC_HANDLER,
    public TAO_Connection_Handler
{
public:
  TAO_COIOP_Connection_Handler (ACE_Thread_Manager *t = nullptr);

  /// Constructor.
  TAO_COIOP_Connection_Handler (TAO_ORB_Core *orb_core);

  /// Destructor.
  ~TAO_COIOP_Connection_Handler ();

  /// Called by the <Strategy_Acceptor> when the handler is completely
  /// connected.  Argument is unused.
  int open (void *) override;

  //@{
  /**
   * Connection_Handler overloads
   */
  int open_handler (void *) override;
  //@}

  /// Close called by the Acceptor or Connector when connection
  /// establishment fails.
  int close (u_long = 0) override;

  //@{
  /** @name Event Handler overloads
   */
  int resume_handler () override;
  int close_connection () override;
  int handle_input (ACE_HANDLE) override;
  int handle_output (ACE_HANDLE) override;
  int handle_close (ACE_HANDLE, ACE_Reactor_Mask) override;
  int handle_timeout (ACE_Time_Value const &current_time,
                      void const *act = nullptr) override;
  //@}

  /// Add ourselves to Cache.
  int add_transport_to_cache ();
};

TAO_END_VERSIONED_NAMESPACE_DECL

#endif /* TAO_HAS_COIOP && TAO_HAS_COIOP != 0 */

#include /**/ "ace/post.h"

#endif /* TAO_COIOP_CONNECTION_HANDLER_H */
