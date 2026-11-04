// -*- C++ -*-

//=============================================================================
/**
 *  @file    QoS_Session_Impl.h
 *
 *  @author Vishal Kachroo <vishal@cs.wustl.edu>
 */
//=============================================================================


#ifndef ACE_QOS_SESSION_IMPL_H
#define ACE_QOS_SESSION_IMPL_H
#include /**/ "ace/pre.h"

#include "ace/QoS/QoS_Session.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if defined (ACE_HAS_RAPI)
#include "rapi_lib.h"

#define DEFAULT_SOURCE_SENDER_PORT 10001

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

/**
 * @class ACE_RAPI_Session
 *
 * @brief A RAPI QoS session object.
 *
 * This class is a RAPI (RSVP API, an implementation of RSVP on UNIX)
 * implementation of the ACE_QoS_Session interface.
 */
class ACE_QoS_Export ACE_RAPI_Session : public ACE_QoS_Session
{
public:
  /// Default destructor.
  ~ACE_RAPI_Session () override;

  ACE_RAPI_Session (ACE_RAPI_Session const &) = delete;
  ACE_RAPI_Session &operator= (ACE_RAPI_Session const &) = delete;
  ACE_RAPI_Session (ACE_RAPI_Session &&) = delete;
  ACE_RAPI_Session &operator= (ACE_RAPI_Session &&) = delete;

  /// Error handling for RSVP callback
  static int rsvp_error;

  /// Open a RAPI QoS session [dest IP, dest port, Protocol ID].
  int open (ACE_INET_Addr dest_addr,
            ACE_Protocol_ID protocol_id) override;

  /// Close the RAPI QoS Session.
  int close () override;

  /// Returns the QoS for this RAPI session.
  ACE_QoS qos () const override;

  /// Set QoS for this RAPI session. The socket parameter is used to confirm if
  /// this QoS session was subscribed to by the socket.
  int qos (ACE_SOCK *socket,
           ACE_QoS_Manager *qos_manager,
           ACE_QoS const &ace_qos) override;

  /**
   * Sets the QoS for this session object to ace_qos. Does not interfere with the
   * QoS in the underlying socket. This call is useful to update the QoS object
   * when the underlying socket QoS is being set through a mechanism other than
   * the previous qos () method e.g. inside the dgram_mcast.subscribe () where the
   * QoS for the socket is set through ACE_OS::join_leaf ().
   */
  void qos (ACE_QoS const &ace_qos) override;

  /**
   * Calls rapi_dispatch () that further triggers the call back function.
   * It is a mechanism of updating the QoS for this session asynchronously, as
   * RSVP events occur.
   */
  int update_qos () override;

  /// Get methods for the flags_.
  ACE_End_Point_Type flags () const override;

  /// Set methods for the flags_.
  void flags (ACE_End_Point_Type flags) override;

  /// Get the RAPI session id.
  int session_id () const override;

  /// Set the RAPI session id.
  void session_id (int session_id) override;

  /// Get the RAPI file descriptor for RSVP events.
  ACE_HANDLE rsvp_events_handle () override;

  ///Set the RAPI event that last occurred
  void rsvp_event_type (RSVP_Event_Type event_type) override;

  ///Get the RAPI event that last occurred
  RSVP_Event_Type rsvp_event_type () override;

  /// Get the destination address for this RAPI session.
  ACE_INET_Addr dest_addr () const override;

  /// Set the destination address for this RAPI session.
  void dest_addr (ACE_INET_Addr const &dest_addr) override;

  /// Get the source port for this session.
  u_short source_port () const override;

  /// Set the source port for this session.
  void source_port (u_short const &source_port) override;

  //Set the source host
  ACE_INET_Addr *source_addr () const override;

  /// Set the source port for this session.
  void source_addr (ACE_INET_Addr *source_addr) override;

  /// RAPI version. Returned value = 100 * major-version + minor-version.
  int version () override;

  /// The factory is a friend so it can create this object through
  /// the only private constructor.
  friend class ACE_QoS_Session_Factory;

private:
  /// Default constuctor. Constructor is defined private so that only
  /// the friend factory can instantiate this class.
  ACE_RAPI_Session ();

  /// Construct a simplified RAPI Sender TSpec object
  /// from an ACE_Flow_Spec object. Used internally by this class.
  rapi_tspec_t *init_tspec_simplified (const ACE_Flow_Spec &flow_spec);

  /// Construct a simplified RAPI Receiver FlowSpec object
  /// from an ACE_Flow_Spec object. Used internally by the class.
  rapi_flowspec_t *init_flowspec_simplified(const ACE_Flow_Spec &flow_spec);

  /// Set sending QoS for this RAPI session.
  int sending_qos (const ACE_QoS &ace_qos);

  /// Set receiving  QoS for this RAPI session.
  int receiving_qos (const ACE_QoS &ace_qos);
};

ACE_END_VERSIONED_NAMESPACE_DECL

#endif /* ACE_HAS_RAPI */

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

/**
 * @class ACE_GQoS_Session
 *
 * @brief A GQoS session object.
 *
 * This class is a GQoS (Generic QoS, an implementation of RSVP on
 * Win2K) implementation of the ACE_QoS_Session interface.
 */
class ACE_QoS_Export ACE_GQoS_Session : public ACE_QoS_Session
{
public:
  /// Default destructor.
  ~ACE_GQoS_Session () override;

  ACE_GQoS_Session (ACE_GQoS_Session const &) = delete;
  ACE_GQoS_Session &operator= (ACE_GQoS_Session const &) = delete;
  ACE_GQoS_Session (ACE_GQoS_Session &&) = delete;
  ACE_GQoS_Session &operator= (ACE_GQoS_Session &&) = delete;

  /// This is a session ID generator. It does a lot more than expected
  /// from an int!.
  static int GQoS_session_id;

  /// Open a GQoS session [dest IP, dest port, Protocol ID].
  int open (ACE_INET_Addr dest_addr,
            ACE_Protocol_ID protocol_id) override;

  /// Close the GQoS Session.
  int close () override;

  /// Returns the QoS for this GQoS session.
  ACE_QoS qos () const override;

  /// Set QoS for this GQoS session. The socket parameter is used to confirm if
  /// this QoS session was subscribed to by the socket.
  int qos (ACE_SOCK *socket,
           ACE_QoS_Manager *qos_manager,
           ACE_QoS const &ace_qos) override;

  /**
   * Sets the QoS for this session object to ace_qos. Does not interfere with the
   * QoS in the underlying socket. This call is useful to update the QoS object
   * when the underlying socket QoS is being set through a mechanism other than
   * the previous qos () method e.g. inside the dgram_mcast.subscribe () where the
   * QoS for the socket is set through ACE_OS::join_leaf ().
   */
  void qos (ACE_QoS const &ace_qos) override;

  /// Calls the ioctl (ACE_SIO_GET_QOS). It is a mechanism of updating the
  /// QoS for this session asynchronously, as RSVP events occur.
  int update_qos () override;

  /// Get/Set methods for the flags_.
  ACE_End_Point_Type flags () const override;
  void flags (ACE_End_Point_Type flags) override;

  /// Get the destination address for this GQoS session.
  ACE_INET_Addr dest_addr () const override;

  /// Set the destination address for this GQoS session.
  void dest_addr (ACE_INET_Addr const &dest_addr) override;

  /// Get the source port for this session.
  u_short source_port () const override;

  /// Set the source port for this session.
  void source_port (u_short const &source_port) override;

  //Set the source host
  ACE_INET_Addr *source_addr () const override;

  /// Set the source port for this session.
  void source_addr (ACE_INET_Addr *source_addr) override;

  /// Get the GQoS session id.
  int session_id () const override;

  /// Set the GQoS session id.
  void session_id (int session_id) override;

  /// Get the file descriptor of the underlying socket.
  ACE_HANDLE rsvp_events_handle () override;

  void rsvp_event_type (RSVP_Event_Type event_type) override;
  ///Set the RAPI event that last occurred

  RSVP_Event_Type rsvp_event_type () override;
  ///Get the RAPI event that last occurred

  /// GQoS version.
  int version () override;

  /// The factory is a friend so it can create this object through
  /// the only private constructor.
  friend class ACE_QoS_Session_Factory;

private:
  /// Default constructor. Constructor is defined private so that only
  /// the friend factory can instantiate this class.
  ACE_GQoS_Session ();
};

ACE_END_VERSIONED_NAMESPACE_DECL

#if defined (__ACE_INLINE__)
#include "QoS_Session_Impl.inl"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"
#endif /* ACE_QOS_SESSION_IMPL_H */
