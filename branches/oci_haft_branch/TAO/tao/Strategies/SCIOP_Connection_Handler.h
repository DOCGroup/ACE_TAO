// $Id$
// ===================================================================
/**
 *  @file   SCIOP_Connection_Handler.h
 *
 *  @author  Jason Cohen, Lockheed Martin ATL  <jcohen@atl.lmco.com>
 *  @author  Keith O'Hara, Lockheed Martin ATL
 *  @author  based on IIOP_Connection_Handler.h
 *  @author  Originally by Chris Cleeland
 *  @author  modified by Balachandran Natarajan <bala@cs.wustl.edu>
 */
// ===================================================================

#ifndef TAO_SCIOP_CONNECTION_HANDLER_H
#define TAO_SCIOP_CONNECTION_HANDLER_H
#include "ace/pre.h"

#include "tao/corbafwd.h"
#include "tao/TimeBaseC.h"

#include "ace/Reactor.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if TAO_HAS_SCIOP == 1

#include "strategies_export.h"
#include "tao/Connection_Handler.h"
#include "tao/IIOPC.h"

#include "ace/Svc_Handler.h"
#include "ace/SOCK_SEQPACK_Association.h"


// Service Handler for this transport
typedef ACE_Svc_Handler<ACE_SOCK_SEQPACK_ASSOCIATION, ACE_NULL_SYNCH>
        TAO_SCIOP_SVC_HANDLER;

#if defined ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION_EXPORT
template class TAO_Strategies_Export ACE_Svc_Handler<ACE_SOCK_SEQPACK_ASSOCIATION, ACE_NULL_SYNCH>;
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION_EXPORT */


// Forward Decls
class TAO_Pluggable_Messaging;

// ****************************************************************

/**
 * @class TAO_SCIOP_Properties
 *
 * @brief TCP protocol properties specification for a set of
 *  connections.
 *
 */

#define IPDSFIELD_DSCP_DEFAULT  0x00

class TAO_Strategies_Export TAO_SCIOP_Properties
{

public:
  int send_buffer_size;
  int recv_buffer_size;
  int no_delay;
  int enable_network_priority;

  /*
  TimeBase::TimeT rto_initial;
  TimeBase::TimeT rto_min;
  TimeBase::TimeT rto_max;
  float rto_alpha;
  float rto_beta;
  TimeBase::TimeT valid_cookie_life;
  short associaion_max_retrans;
  short path_max_retrans;
  short max_init_retransmits;
  TimeBase::TimeT hb_interval;
  short desired_streams;
  */
};



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

  /// Constructor. <arg> parameter is used by the Acceptor to pass the
  /// protocol configuration properties for this connection.
  TAO_SCIOP_Connection_Handler (TAO_ORB_Core *orb_core,
                               CORBA::Boolean flag,
                               void *arg);


  /// Destructor.
  ~TAO_SCIOP_Connection_Handler (void);

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

  /// Process the <listen_list>
  int process_listen_point_list (IIOP::ListenPointList &listen_list);

    ///Check if network priority needs to be enabled
  int enable_network_priority (void);

  ///Set the Diff-Serv codepoint if the Policy dicates the setting of Network Priority
  int set_dscp_codepoint (void);

  int set_dscp_codepoint (int tos);

  /// Update the tcp properties of the hanlder to the most recent
  /// properties set after the last invocation
  virtual void update_protocol_properties (int send_buffer_size,
                                           int recv_buffer_size,
                                           int no_delay,
                                           int enable_network_priority);

protected:

  /// Constructor that could be used by the derived classes.
  /**
   * Sometimes new pluggbale protocols which have similarties with
   * IIOP may be tempted to this class for their use. Classical
   * example being that of IIOP_SSL_Connection_Handler. This
   * constructor just initializes its base class and sets all of its
   * contents to the default value, if any
   */
  TAO_SCIOP_Connection_Handler (TAO_ORB_Core *orb_core,
                               void *arg);

  //@{
  /**
   * @name TAO_Connection Handler overloads
   */
  //  void handle_close_i (void);
  virtual int release_os_resources (void);
  //@}

private:
  /// TCP configuration for this connection.
  TAO_SCIOP_Properties tcp_properties_;

  /// Stores the type of service value.
  int dscp_codepoint_;
};

#if defined (__ACE_INLINE__)
#include "SCIOP_Connection_Handler.i"
#endif /* __ACE_INLINE__ */

#endif /* TAO_HAS_SCIOP == 1 */

#include "ace/post.h"
#endif /* TAO_SCIOP_CONNECTION_HANDLER_H */
