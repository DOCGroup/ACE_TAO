/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    SOCK_Connector.h
 *
 *  $Id$
 *
 *  @author Doug Schmidt <schmidt@cs.wustl.edu>
 */
//=============================================================================

#ifndef ACE_SOCK_CONNECTOR_H
#define ACE_SOCK_CONNECTOR_H
#include "ace/pre.h"

#include "ace/Sockets/SOCK_Stream.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Timer/Time_Value.h"

/**
 * @class ACE_SOCK_Connector
 *
 * @brief Defines a factory that creates new <ACE_Stream>s actively.
 *
 * The <ACE_SOCK_Connector> doesn't have a socket of its own,
 * i.e., it simply "borrows" the one from the ACE_SOCK_Stream
 * that's being connected.  The reason for this is that the
 * underlying socket API doesn't use a "factory" socket to connect
 * "data-mode" sockets.  Therefore, there's no need to inherit
 * <ACE_SOCK_Connector> from <ACE_SOCK>.  A nice side-effect of
 * this is that <ACE_SOCK_Connector>'s do not store state so they
 * can be used reentrantly in multi-threaded programs.
 */
class ACE_Export ACE_SOCK_Connector
{
public:
  // = Initialization and termination methods.
  /// Default constructor.
  ACE_SOCK_Connector (void);

  /**
   * Actively connect and produce a <new_stream> if things go well.
   * The <remote_sap> is the address that we are trying to connect
   * with.  The <timeout> is the amount of time to wait to connect.
   * If it's 0 then we block indefinitely.  If *timeout == {0, 0} then
   * the connection is done using non-blocking mode.  In this case, if
   * the connection can't be made immediately the value of -1 is
   * returned with <errno == EWOULDBLOCK>.  If *timeout > {0, 0} then
   * this is the maximum amount of time to wait before timing out; if the
   * time expires before the connection is made <errno == ETIME>. Note
   * the difference between this case and when a blocking connect
   * is attmpted that TCP times out - in the latter case, errno will
   * be ETIMEDOUT.
   * The <local_sap> is the value of local address to bind to.  If it's
   * the default value of <ACE_Addr::sap_any> then the user is letting
   * the OS do the binding.  If <reuse_addr> == 1 then the
   * <local_addr> is reused, even if it hasn't been cleanedup yet.
   * The <protocol_family> and <protocol> parameters are passed down
   * to the <socket> call, whereas <flags> and <perms> are ignored.
   */
  ACE_SOCK_Connector (ACE_SOCK_Stream &new_stream,
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
   * to the IntServ (RSVP) and DiffServ protocols.  The <timeout> is
   * the amount of time to wait to connect.  If it's 0 then we block
   * indefinitely.  If *timeout == {0, 0} then the connection is done
   * using non-blocking mode.  In this case, if the connection can't
   * be made immediately the value of -1 is returned with <errno ==
   * EWOULDBLOCK>.  If *timeout > {0, 0} then this is the amount of
   * time to wait before timing out; if the time expires before the
   * connection is made <errno == ETIME>.  Note the difference between
   * this case and when a blocking connect is attmpted that TCP times
   * out - in the latter case, errno will be ETIMEDOUT.
   * The <local_sap> is the value of local address to bind to.  If
   * it's the default value of <ACE_Addr::sap_any> then the user is
   * letting the OS do the binding.
   * If <reuse_addr> == 1 then the <local_addr> is reused,
   * even if it hasn't been cleaned up yet.
   * The <protocol_family> and <protocol> parameters are passed down
   * to the <socket> call, whereas <flags> and <perms> are ignored.
   */
  ACE_SOCK_Connector (ACE_SOCK_Stream &new_stream,
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
   * Actively connect and produce a <new_stream> if things go well.
   * The <remote_sap> is the address that we are trying to connect
   * with.  The <timeout> is the amount of time to wait to connect.
   * If it's 0 then we block indefinitely.  If *timeout == {0, 0} then
   * the connection is done using non-blocking mode.  In this case, if
   * the connection can't be made immediately the value of -1 is
   * returned with <errno == EWOULDBLOCK>.  If *timeout > {0, 0} then
   * this is the maximum amount of time to wait before timing out; if the
   * time expires before the connection is made <errno == ETIME>.   Note
   * the difference between this case and when a blocking connect
   * is attmpted that TCP times out - in the latter case, errno will
   * be ETIMEDOUT.
   * The <local_sap> is the value of local address to bind to.  If it's
   * the default value of <ACE_Addr::sap_any> then the user is letting
   * the OS do the binding.  If <reuse_addr> == 1 then the
   * <local_addr> is reused, even if it hasn't been cleanedup yet.
   * Note that the <new_stream> always starts out in blocking mode.
   * The <protocol_family> and <protocol> parameters are passed down
   * to the <socket> call, whereas <flags> and <perms> are ignored.
   */
  int connect (ACE_SOCK_Stream &new_stream,
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
   * to the IntServ (RSVP) and DiffServ protocols.  The <timeout> is
   * the amount of time to wait to connect.  If it's 0 then we block
   * indefinitely.  If *timeout == {0, 0} then the connection is done
   * using non-blocking mode.  In this case, if the connection can't
   * be made immediately the value of -1 is returned with <errno ==
   * EWOULDBLOCK>.  If *timeout > {0, 0} then this is the amount of
   * time to wait before timing out; if the time expires before the
   * connection is made <errno == ETIME>.  Note the difference between
   * this case and when a blocking connect is attmpted that TCP times
   * out - in the latter case, errno will be ETIMEDOUT.
   * The <local_sap> is the value of local address to bind to.  If
   * it's the default value of <ACE_Addr::sap_any> then the user is
   * letting the OS do the binding.
   * If <reuse_addr> == 1 then the <local_addr> is reused,
   * even if it hasn't been cleanedup yet.
   */
  int connect (ACE_SOCK_Stream &new_stream,
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
  ~ACE_SOCK_Connector (void);

  // = Completion routine.
  /**
   * Try to complete a non-blocking connection.
   * If connection completion is successful then <new_stream> contains
   * the connected ACE_SOCK_Stream.  If <remote_sap> is non-NULL then it
   * will contain the address of the connected peer.
   */
  int complete (ACE_SOCK_Stream &new_stream,
                ACE_Addr *remote_sap = 0,
                const ACE_Time_Value *timeout = 0);

  /// Resets any event associations on this handle
  int reset_new_handle (ACE_HANDLE handle);

  // = Meta-type info
  typedef ACE_INET_Addr PEER_ADDR;
  typedef ACE_SOCK_Stream PEER_STREAM;

  /// Dump the state of an object.
  void dump (void) const;

  /// Declare the dynamic allocation hooks.
  ACE_ALLOC_HOOK_DECLARE;

protected:
  /// Perform operations that ensure the socket is opened using
  /// BSD-style semantics (no QoS).
  int shared_open (ACE_SOCK_Stream &new_stream,
                   int protocol_family,
                   int protocol,
                   int reuse_addr);

  /// Perform operations that ensure the socket is opened using
  /// QoS-enabled semantics.
  int shared_open (ACE_SOCK_Stream &new_stream,
                   int protocol_family,
                   int protocol,
                   ACE_Protocol_Info *protocolinfo,
                   ACE_SOCK_GROUP g,
                   u_long flags,
                   int reuse_addr);

  /// Perform operations that must be called before <ACE_OS::connect>.
  int shared_connect_start (ACE_SOCK_Stream &new_stream,
                            const ACE_Time_Value *timeout,
                            const ACE_Addr &local_sap);

  /// Perform operations that must be called after <ACE_OS::connect>.
  int shared_connect_finish (ACE_SOCK_Stream &new_stream,
                             const ACE_Time_Value *timeout,
                             int result);
};

#if !defined (ACE_LACKS_INLINE_FUNCTIONS)
#include "ace/Sockets/SOCK_Connector.i"
#endif

#include "ace/post.h"
#endif /* ACE_SOCK_CONNECTOR_H */
