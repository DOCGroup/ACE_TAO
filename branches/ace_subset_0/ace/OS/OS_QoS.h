// -*- C++ -*-

//=============================================================================
/**
 *  @file   OS_QoS.h
 *
 *  $Id$
 *
 *  @brief  Contains OS specific data structures for QoS networking.
 *
 *  @author Craig Rodrigues  <crodrigu@bbn.com>
 */
//=============================================================================

#ifndef ACE_OS_QOS_H
#define ACE_OS_QOS_H
#include "ace/pre.h"

#include "OS.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if defined (ACE_HAS_WINSOCK2) && (ACE_HAS_WINSOCK2 != 0)
#if defined (ACE_HAS_WINSOCK2_GQOS)
typedef SERVICETYPE ACE_SERVICE_TYPE;
#else
typedef u_long ACE_SERVICE_TYPE;
#endif /* ACE_HAS_WINSOCK2_GQOS */


#define ACE_OVERLAPPED_SOCKET_FLAG WSA_FLAG_OVERLAPPED

#define ACE_XP1_QOS_SUPPORTED XP1_QOS_SUPPORTED
#define ACE_XP1_SUPPORT_MULTIPOINT XP1_SUPPORT_MULTIPOINT

#define ACE_BASEERR WSABASEERR
#define ACE_ENOBUFS WSAENOBUFS
#define ACE_FROM_PROTOCOL_INFO FROM_PROTOCOL_INFO
#define ACE_FLAG_MULTIPOINT_C_ROOT WSA_FLAG_MULTIPOINT_C_ROOT
#define ACE_FLAG_MULTIPOINT_C_LEAF WSA_FLAG_MULTIPOINT_C_LEAF
#define ACE_FLAG_MULTIPOINT_D_ROOT WSA_FLAG_MULTIPOINT_D_ROOT
#define ACE_FLAG_MULTIPOINT_D_LEAF WSA_FLAG_MULTIPOINT_D_LEAF

#define ACE_QOS_NOT_SPECIFIED QOS_NOT_SPECIFIED
#define ACE_SERVICETYPE_NOTRAFFIC SERVICETYPE_NOTRAFFIC
#define ACE_SERVICETYPE_CONTROLLEDLOAD SERVICETYPE_CONTROLLEDLOAD
#define ACE_SERVICETYPE_GUARANTEED SERVICETYPE_GUARANTEED

#define ACE_JL_SENDER_ONLY JL_SENDER_ONLY
#define ACE_JL_BOTH JL_BOTH

#define ACE_SIO_GET_QOS SIO_GET_QOS
#define ACE_SIO_MULTIPOINT_LOOPBACK SIO_MULTIPOINT_LOOPBACK
#define ACE_SIO_MULTICAST_SCOPE SIO_MULTICAST_SCOPE
#define ACE_SIO_SET_QOS SIO_SET_QOS

#else
typedef u_long ACE_SERVICE_TYPE;

#define ACE_OVERLAPPED_SOCKET_FLAG 0
#define ACE_XP1_QOS_SUPPORTED        0x00002000
#define ACE_XP1_SUPPORT_MULTIPOINT   0x00000400

#define ACE_BASEERR   10000
#define ACE_ENOBUFS   (ACE_BASEERR+55)

#define ACE_FROM_PROTOCOL_INFO (-1)

#define ACE_FLAG_MULTIPOINT_C_ROOT    0x02
#define ACE_FLAG_MULTIPOINT_C_LEAF    0x04
#define ACE_FLAG_MULTIPOINT_D_ROOT    0x08
#define ACE_FLAG_MULTIPOINT_D_LEAF    0x10

#define ACE_QOS_NOT_SPECIFIED            0xFFFFFFFF
#define ACE_SERVICETYPE_NOTRAFFIC        0x00000000  /* No data in this */
                                                     /* direction. */
#define ACE_SERVICETYPE_CONTROLLEDLOAD   0x00000001  /* Controlled Load. */
#define ACE_SERVICETYPE_GUARANTEED       0x00000003  /* Guaranteed. */

#define ACE_JL_SENDER_ONLY    0x01
#define ACE_JL_BOTH           0x04

#define ACE_SIO_GET_QOS              (0x40000000 | 0x08000000 | 7)
#define ACE_SIO_MULTIPOINT_LOOPBACK  (0x08000000 | 9)
#define ACE_SIO_MULTICAST_SCOPE      (0x08000000 | 10)
#define ACE_SIO_SET_QOS              (0x08000000 | 11)

#endif /* ACE_HAS_WINSOCK2 && ACE_HAS_WINSOCK2 != 0 */




/**
 * @class ACE_Flow_Spec
 *
 * @brief Wrapper class that defines the flow spec QoS information,
 *    which is used by IntServ (RSVP) and DiffServ.
 */
class ACE_OS_Export ACE_Flow_Spec
#if defined (ACE_HAS_WINSOCK2) && (ACE_HAS_WINSOCK2 != 0)
  : public FLOWSPEC
#endif /* ACE_HAS_WINSOCK2 */
{
public:
  /// Default constructor.
  ACE_Flow_Spec (void);

  /// Constructor that initializes all the fields.
  ACE_Flow_Spec (u_long token_rate,
                 u_long token_bucket_size,
                 u_long peak_bandwidth,
                 u_long latency,
                 u_long delay_variation,
                 ACE_SERVICE_TYPE service_type,
                 u_long max_sdu_size,
                 u_long minimum_policed_size,
                 int ttl,
                 int priority);

  /// Get the token rate in bytes/sec.
  u_long token_rate (void) const;

  /// Set the token rate in bytes/sec.
  void token_rate (u_long tr);

  /// Get the token bucket size in bytes.
  u_long token_bucket_size (void) const;

  /// Set the token bucket size in bytes.
  void token_bucket_size (u_long tbs);

  /// Get the PeakBandwidth in bytes/sec.
  u_long peak_bandwidth (void) const;

  /// Set the PeakBandwidth in bytes/sec.
  void peak_bandwidth (u_long pb);

  /// Get the latency in microseconds.
  u_long latency (void) const;

  /// Set the latency in microseconds.
  void latency (u_long l);

  /// Get the delay variation in microseconds.
  u_long delay_variation (void) const;

  /// Set the delay variation in microseconds.
  void delay_variation (u_long dv);

  /// Get the service type.
  ACE_SERVICE_TYPE service_type (void) const;

  /// Set the service type.
  void service_type (ACE_SERVICE_TYPE st);

  /// Get the maximum SDU size in bytes.
  u_long max_sdu_size (void) const;

  /// Set the maximum SDU size in bytes.
  void max_sdu_size (u_long mss);

  /// Get the minimum policed size in bytes.
  u_long minimum_policed_size (void) const;

  /// Set the minimum policed size in bytes.
  void minimum_policed_size (u_long mps);

  /// Get the time-to-live.
  int ttl (void) const;

  /// Set the time-to-live.
  void ttl (int t);

  /// Get the priority.
  int priority (void) const;

  /// Set the priority.
  void priority (int p);

#if defined (ACE_HAS_WINSOCK2) && (ACE_HAS_WINSOCK2 != 0) && \
    defined (ACE_HAS_WINSOCK2_GQOS)
#else
private:
  u_long token_rate_;
  u_long token_bucket_size_;
  u_long peak_bandwidth_;
  u_long latency_;
  u_long delay_variation_;
  ACE_SERVICE_TYPE service_type_;
  u_long max_sdu_size_;
  u_long minimum_policed_size_;
  int ttl_;
  int priority_;
#endif /* defined (ACE_HAS_WINSOCK2) && (ACE_HAS_WINSOCK2 != 0) && \
          defined (ACE_HAS_WINSOCK2_GQOS) */
};

/**
 * @class ACE_QoS
 *
 * @brief Wrapper class that holds the sender and receiver flow spec
 *     information, which is used by IntServ (RSVP) and DiffServ.
 */
class ACE_OS_Export ACE_QoS
#if defined (ACE_HAS_WINSOCK2) && (ACE_HAS_WINSOCK2 != 0)
  : public QOS
#endif /* ACE_HAS_WINSOCK2 */
{
public:
  /// Constructor
  ACE_QoS (void);

  /// Get the flow spec for data sending.
  ACE_Flow_Spec *sending_flowspec (void) const;

  /// Set the flow spec for data sending.
  void sending_flowspec (ACE_Flow_Spec *fs);

  /// Get the flow spec for data receiving.
  ACE_Flow_Spec *receiving_flowspec (void) const;

  /// Set the flow spec for data receiving.
  void receiving_flowspec (ACE_Flow_Spec *fs);

  /// Get the provider specific information.
  iovec provider_specific (void) const;

  /// Set the provider specific information.
  void provider_specific (const iovec &ps);

#if defined (ACE_HAS_WINSOCK2) && (ACE_HAS_WINSOCK2 != 0)
#else
private:

  ACE_Flow_Spec *sending_flowspec_;
  ACE_Flow_Spec *receiving_flowspec_;
#endif

};

/**
 * @class ACE_QoS_Params
 *
 * @brief Wrapper class that simplifies the information passed to the QoS
 * enabled <ACE_OS::connect> and <ACE_OS::join_leaf> methods.
 */
class ACE_OS_Export ACE_QoS_Params
{
public:
  /**
   * Initialize the data members.  The <caller_data> is a pointer to
   * the user data that is to be transferred to the peer during
   * connection establishment.  The <callee_data> is a pointer to the
   * user data that is to be transferred back from the peer during
   * connection establishment.  The_<socket_qos> is a pointer to the
   * flow specifications for the socket, one for each direction.  The
   * <group_socket_qos> is a pointer to the flow speicfications for
   * the socket group, if applicable.  The_<flags> indicate if we're a
   * sender, receiver, or both.
   */
  ACE_QoS_Params (iovec *caller_data = 0,
                  iovec *callee_data = 0,
                  ACE_QoS *socket_qos = 0,
                  ACE_QoS *group_socket_qos = 0,
                  u_long flags = 0);

  /// Get caller data.
  iovec *caller_data (void) const;

  /// Set caller data.
  void caller_data (iovec *);

  /// Get callee data.
  iovec *callee_data (void) const;

  /// Set callee data.
  void callee_data (iovec *);

  /// Get socket qos.
  ACE_QoS *socket_qos (void) const;

  /// Set socket qos.
  void socket_qos (ACE_QoS *);

  /// Get group socket qos.
  ACE_QoS *group_socket_qos (void) const;

  /// Set group socket qos.
  void group_socket_qos (ACE_QoS *);

  /// Get flags.
  u_long flags (void) const;

  /// Set flags.
  void flags (u_long);

private:
  /// A pointer to the user data that is to be transferred to the peer
  /// during connection establishment.
  iovec *caller_data_;

  /// A pointer to the user data that is to be transferred back from
  /// the peer during connection establishment.
  iovec *callee_data_;

  /// A pointer to the flow speicfications for the socket, one for each
  /// direction.
  ACE_QoS *socket_qos_;

  /// A pointer to the flow speicfications for the socket group, if
  /// applicable.
  ACE_QoS *group_socket_qos_;

  /// Flags that indicate if we're a sender, receiver, or both.
  u_long flags_;
};

// Callback function that's used by the QoS-enabled <ACE_OS::accept>
// method.
typedef int (*ACE_QOS_CONDITION_FUNC) (iovec *caller_id,
                                       iovec *caller_data,
                                       ACE_QoS *socket_qos,
                                       ACE_QoS *group_socket_qos,
                                       iovec *callee_id,
                                       iovec *callee_data,
                                       ACE_SOCK_GROUP *g,
                                       u_long callbackdata);


/**
 * @class ACE_Accept_QoS_Params
 *
 * @brief Wrapper class that simplifies the information passed to the QoS
 * enabled <ACE_OS::accept> method.
 */
class ACE_OS_Export ACE_Accept_QoS_Params
{
public:
  /**
   * Initialize the data members.  The <qos_condition_callback> is the
   * address of an optional, application-supplied condition function
   * that will make an accept/reject decision based on the caller
   * information pass in as parameters, and optionally create or join
   * a socket group by assinging an appropriate value to the result
   * parameter <g> of this function.  The <callback_data> data is
   * passed back to the application as a condition function parameter,
   * i.e., it is an Asynchronous Completion Token (ACT).
   */
  ACE_Accept_QoS_Params (ACE_QOS_CONDITION_FUNC qos_condition_callback = 0,
                         u_long callback_data = 0);

  /// Get QoS condition callback.
  ACE_QOS_CONDITION_FUNC qos_condition_callback (void) const;

  /// Set QoS condition callback.
  void qos_condition_callback (ACE_QOS_CONDITION_FUNC qcc);

  /// Get callback data.
  u_long callback_data (void) const;

  /// Set callback data.
  void callback_data (u_long cd);

private:
  /**
   * This is the address of an optional, application-supplied
   * condition function that will make an accept/reject decision based
   * on the caller information pass in as parameters, and optionally
   * create or join a socket group by assinging an appropriate value
   * to the result parameter <g> of this function.
   */
  ACE_QOS_CONDITION_FUNC qos_condition_callback_;

  /**
   * This data is passed back to the application as a condition
   * function parameter, i.e., it is an Asynchronous Completion Token
   * (ACT).
   */
  u_long callback_data_;
};


#include "ace/post.h"
#endif  /* ACE_OS_QOS_H */
