// This may look like C, but it's really -*- C++ -*-

// ===================================================================
/**
 *  @file   UIOP_Connection_Handler.h
 *
 *  $Id$
 *
 *  @author Originally by Ossama Othman <ossama@ece.uci.edu> as
 *   UIOP_Connect.h
 *  @author modified by Balachandran Natarajan <bala@cs.wustl.edu>
 */
// ===================================================================
#ifndef TAO_UIOP_CONNECTION_HANDLER_H
#define TAO_UIOP_CONNECTION_HANDLER_H
#include "ace/pre.h"

#include "tao/corbafwd.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if TAO_HAS_UIOP == 1

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Acceptor.h"
#include "ace/Reactor.h"
#include "tao/Connection_Handler.h"
#include "tao/corbafwd.h"
#include "tao/Wait_Strategy.h"
#include "UIOP_Transport.h"

// Forward Decls
class TAO_Pluggable_Messaging;

/**
 * @class TAO_UIOP_Properties
 *
 * @brief Unix Domain Sockets protocol properties specification for a
 *  set of connections.
 */
class TAO_Strategies_Export TAO_UIOP_Properties
{
public:

  int send_buffer_size;
  int recv_buffer_size;
};

// ****************************************************************

/**
 * @class TAO_UIOP_Connection_Handler
 *
 * @brief  Handles requests on a single connection.
 *
 * The Connection handler which is common for the Acceptor and
 * the Connector
 */
class TAO_Strategies_Export TAO_UIOP_Connection_Handler : public TAO_UIOP_SVC_HANDLER,
                                                          public TAO_Connection_Handler
{

public:

  TAO_UIOP_Connection_Handler (ACE_Thread_Manager* t = 0);

  /// Constructor. <arg> parameter is used by the Acceptor to pass the
  /// protocol configuration properties for this connection.
  TAO_UIOP_Connection_Handler (TAO_ORB_Core *orb_core,
                               CORBA::Boolean flag,
                               void *arg);


  /// Destructor.
  ~TAO_UIOP_Connection_Handler (void);

  /// Called by the <Strategy_Acceptor> when the handler is completely
  /// connected.  Argument is unused.
  virtual int open (void *);


  /// Active object activation method.
  virtual int activate (long flags = THR_NEW_LWP,
                        int n_threads = 1,
                        int force_active = 0,
                        long priority = ACE_DEFAULT_THREAD_PRIORITY,
                        int grp_id = -1,
                        ACE_Task_Base *task = 0,
                        ACE_hthread_t thread_handles[] = 0,
                        void *stack[] = 0,
                        size_t stack_size[] = 0,
                        ACE_thread_t  thread_names[] = 0);

  /// Only used when the handler is turned into an active object by
  /// calling <activate>.  This serves as the event loop in such cases.
  virtual int svc (void);

  //@{
  /** @name Event Handler overloads
   */
  virtual int resume_handler (void);
  virtual int handle_input (ACE_HANDLE);
  virtual int handle_output (ACE_HANDLE);
  virtual int handle_close (ACE_HANDLE, ACE_Reactor_Mask);
  //@}

  /// Add ourselves to Cache.
  int add_transport_to_cache (void);

protected:

  //@{
  /**
   * @name TAO_Connection Handler overloads
   */
  void handle_close_i (void);
  virtual int release_os_resources (void);
  //@}

private:

  /// TCP configuration for this connection.
  TAO_UIOP_Properties *uiop_properties_;
};



#if defined (__ACE_INLINE__)
#include "UIOP_Connection_Handler.inl"
#endif /* __ACE_INLINE__ */

#endif /* TAO_HAS_UIOP == 1 */

#include "ace/post.h"
#endif /* TAO_UIOP_CONNECT_H */
