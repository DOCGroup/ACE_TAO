// -*- C++ -*-

//=============================================================================
/**
 *  @file    SSL_SOCK_Connector.h
 *
 *  $Id$
 *
 *  @author Ossama Othman <ossama@uci.edu>
 *  @author Carlos O'Ryan <coryan@uci.edu>
 *  @author John Heitmann
 *  @author Chris Zimman
 */
//=============================================================================


#ifndef ACE_SSL_SOCK_CONNECTOR_H
#define ACE_SSL_SOCK_CONNECTOR_H

#include "ace/pre.h"

#include "SSL_Export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "SSL_SOCK_Stream.h"

#include "ace/SOCK_Connector.h"
#include "ace/OS_QoS.h"

/**
 * @class ACE_SSL_SOCK_Connector
 *
 * @brief Defines a factory that creates new <ACE_SSL_SOCK_Stream>s
 *        actively.
 *
 * The ACE_SSL_SOCK_Connector doesn't have a socket of its own,
 * i.e., it simply "borrows" the one from the ACE_SSL_SOCK_Stream
 * that's being connected.  The reason for this is that the
 * underlying socket API doesn't use a "factory" socket to connect
 * "data-mode" sockets.  Therefore, there's no need to inherit
 * ACE_SSL_SOCK_Connector from ACE_SSL_SOCK.
 *
 * Since SSL is record-oriented, some additional work is done after
 * the plain socket is connected.
 *
 * @note The user must currently ensure that only one thread services
 *       a given SSL session at any given time since some underlying
 *       SSL implementations, such as OpenSSL, are not entirely
 *       thread-safe or reentrant.
 */
class ACE_SSL_Export ACE_SSL_SOCK_Connector
{

public:

  /// Default constructor.
  ACE_SSL_SOCK_Connector (void);

  /**
   * Actively connect and produce a <new_stream> if things go well.
   * The <remote_sap> is the address that we are trying to connect
   * with.  The <timeout> is the amount of time to wait to connect.
   * If it's 0 then we block indefinitely.  If *timeout == {0, 0} then
   * the connection is done using non-blocking mode.  In this case, if
   * the connection can't be made immediately the value of -1 is
   * returned with <errno == EWOULDBLOCK>.  If *timeout > {0, 0} then
   * this is the amount of time to wait before timing out.  If the
   * time expires before the connection is made <errno == ETIME>.  The
   * <local_sap> is the value of local address to bind to.  If it's
   * the default value of <ACE_Addr::sap_any> then the user is letting
   * the OS do the binding.  If <reuse_addr> == 1 then the
   * <local_addr> is reused, even if it hasn't been cleanedup yet.
   */
  ACE_SSL_SOCK_Connector (ACE_SSL_SOCK_Stream &new_stream,
                          const ACE_Addr &remote_sap,
                          const ACE_Time_Value *timeout = 0,
                          const ACE_Addr &local_sap = ACE_Addr::sap_any,
                          int reuse_addr = 0,
                          int flags = 0,
                          int perms = 0,
                          int protocol_family = ACE_PROTOCOL_FAMILY_INET,
                          int protocol = 0);

  /**
   * Actively connect and produce a <new_stream> if things go well.
   * The <remote_sap> is the address that we are trying to connect
   * with.  The <qos_params> contains QoS parameters that are passed
   * to RSVP.  The <timeout> is the amount of time to wait to connect.
   * If it's 0 then we block indefinitely.  If *timeout == {0, 0} then
   * the connection is done using non-blocking mode.  In this case, if
   * the connection can't be made immediately the value of -1 is
   * returned with <errno == EWOULDBLOCK>.  If *timeout > {0, 0} then
   * this is the amount of time to wait before timing out.  If the
   * time expires before the connection is made <errno == ETIME>.  The
   * <local_sap> is the value of local address to bind to.  If it's
   * the default value of <ACE_Addr::sap_any> then the user is letting
   * the OS do the binding.  If <reuse_addr> == 1 then the
   * <local_addr> is reused, even if it hasn't been cleanedup yet.
   */
  ACE_SSL_SOCK_Connector (ACE_SSL_SOCK_Stream &new_stream,
                          const ACE_Addr &remote_sap,
                          ACE_QoS_Params qos_params,
                          const ACE_Time_Value *timeout = 0,
                          const ACE_Addr &local_sap = ACE_Addr::sap_any,
                          ACE_Protocol_Info *protocolinfo = 0,
                          ACE_SOCK_GROUP g = 0,
                          u_long flags = 0,
                          int reuse_addr = 0,
                          int perms = 0,
                          int protocol_family = ACE_PROTOCOL_FAMILY_INET,
                          int protocol = 0);

  /// Default dtor.
  ~ACE_SSL_SOCK_Connector (void);

  /**
   * Actively connect and produce a <new_stream> if things go well.
   * The <remote_sap> is the address that we are trying to connect
   * with.  The <timeout> is the amount of time to wait to connect.
   * If it's 0 then we block indefinitely.  If *timeout == {0, 0} then
   * the connection is done using non-blocking mode.  In this case, if
   * the connection can't be made immediately the value of -1 is
   * returned with <errno == EWOULDBLOCK>.  If *timeout > {0, 0} then
   * this is the amount of time to wait before timing out.  If the
   * time expires before the connection is made <errno == ETIME>.  The
   * <local_sap> is the value of local address to bind to.  If it's
   * the default value of <ACE_Addr::sap_any> then the user is letting
   * the OS do the binding.  If <reuse_addr> == 1 then the
   * <local_addr> is reused, even if it hasn't been cleanedup yet.
   */
  int connect (ACE_SSL_SOCK_Stream &new_stream,
               const ACE_Addr &remote_sap,
               const ACE_Time_Value *timeout = 0,
               const ACE_Addr &local_sap = ACE_Addr::sap_any,
               int reuse_addr = 0,
               int flags = 0,
               int perms = 0,
               int protocol_family = ACE_PROTOCOL_FAMILY_INET,
               int protocol = 0);

  /**
   * Actively connect and produce a <new_stream> if things go well.
   * The <remote_sap> is the address that we are trying to connect
   * with.  The <qos_params> contains QoS parameters that are passed
   * to RSVP.  The <timeout> is the amount of time to wait to connect.
   * If it's 0 then we block indefinitely.  If *timeout == {0, 0} then
   * the connection is done using non-blocking mode.  In this case, if
   * the connection can't be made immediately the value of -1 is
   * returned with <errno == EWOULDBLOCK>.  If *timeout > {0, 0} then
   * this is the amount of time to wait before timing out.  If the
   * time expires before the connection is made <errno == ETIME>.  The
   * <local_sap> is the value of local address to bind to.  If it's
   * the default value of <ACE_Addr::sap_any> then the user is letting
   * the OS do the binding.  If <reuse_addr> == 1 then the
   * <local_addr> is reused, even if it hasn't been cleanedup yet.
   */
  int connect (ACE_SSL_SOCK_Stream &new_stream,
               const ACE_Addr &remote_sap,
               ACE_QoS_Params qos_params,
	       const ACE_Time_Value *timeout = 0,
	       const ACE_Addr &local_sap = ACE_Addr::sap_any,
               ACE_Protocol_Info *protocolinfo = 0,
               ACE_SOCK_GROUP g = 0,
               u_long flags = 0,
	       int reuse_addr = 0,
	       int perms = 0,
	       int protocol_family = ACE_PROTOCOL_FAMILY_INET,
	       int protocol = 0);

  /**
   * Try to complete a non-blocking connection.
   * If connection completion is successful then <new_stream> contains
   * the connected ACE_SSL_SOCK_Stream.  If <remote_sap> is non-NULL
   * then it will contain the address of the connected peer.
   */
  int complete (ACE_SSL_SOCK_Stream &new_stream,
                ACE_Addr *remote_sap = 0,
                const ACE_Time_Value *timeout = 0);

  /// Resets any event associations on this handle
  int reset_new_handle (ACE_HANDLE handle);

  /// Meta-type info
  //@{
  typedef ACE_INET_Addr PEER_ADDR;
  typedef ACE_SSL_SOCK_Stream PEER_STREAM;
  //@}

  /// Dump the state of an object.
  void dump (void) const;

  /// Declare the dynamic allocation hooks.
  ACE_ALLOC_HOOK_DECLARE;

protected:

  /// Complete non-blocking SSL active connection.
  int ssl_connect (ACE_SSL_SOCK_Stream &new_stream,
                   const ACE_Time_Value *timeout);

private:

  /// The class that does all of the non-secure socket connection.
  /// It is default contructed, and subsequently used by connect().
  ACE_SOCK_Connector connector_;

};

#if !defined (ACE_LACKS_INLINE_FUNCTIONS)
#include "SSL_SOCK_Connector.i"
#endif /* ACE_LACKS_INLINE_FUNCTIONS */

#include "ace/post.h"

#endif /* ACE_SSL_SOCK_CONNECTOR_H */

