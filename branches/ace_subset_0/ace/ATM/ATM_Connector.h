/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    ATM_Connector.h
 *
 *  $Id$
 *
 *  @author Joe Hoffert <joeh@cs.wustl.edu>
 */
//=============================================================================

#ifndef ACE_ATM_CONNECTOR_H
#define ACE_ATM_CONNECTOR_H
#include "ace/pre.h"

#include "ace/ATM/ATM_Stream.h"
#include "ace/ATM/ATM_Params.h"
#include "ace/ATM/ATM_QoS.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if defined (ACE_HAS_ATM)

#if defined (ACE_WIN32) || defined (ACE_HAS_LINUX_ATM)
#include "SOCK_Connector.h"
typedef ACE_SOCK_Connector ATM_Connector;
#else
#include "XTI_ATM_Mcast.h"
typedef ACE_XTI_ATM_Mcast ATM_Connector;
#endif

/**
 * @class ACE_ATM_Connector
 *
 * @brief Defines an active connection factory for the ACE_ATM C++
 * wrappers.
 */
class ACE_Export ACE_ATM_Connector
{
public:
  // = Initialization methods.
  /// Default constructor.
  ACE_ATM_Connector (void);

  /**
   * Actively connect and produce a <new_stream> if things go well.
   * The <remote_sap> is the address that we are trying to connect
   * with.  The <params> are the parameters needed for either socket
   * or XTI/ATM connections.  The <timeout> is the amount of time to
   * wait to connect. If it's 0 then we block indefinitely.  If
   * *timeout == {0, 0} then the connection is done using non-blocking
   * mode.  In this case, if the connection can't be made immediately
   * the value of -1 is returned with <errno == EWOULDBLOCK>.  If
   * *timeout > {0, 0} then this is the maximum amount of time to wait before
   * timing out.  If the time expires before the connection is made
   * <errno == ETIME>.  The <local_sap> is the value of local address
   * to bind to.  If it's the default value of <ACE_ATM_Addr::sap_any> then
   * the user is letting the OS do the binding.  If <reuse_addr> == 1
   * then the <local_addr> is reused, even if it hasn't been cleanedup yet.
   */
  ACE_ATM_Connector (ACE_ATM_Stream &new_stream,
                     const ACE_ATM_Addr &remote_sap,
                     ACE_ATM_Params params = ACE_ATM_Params(),
                     ACE_ATM_QoS options = ACE_ATM_QoS(),
                     ACE_Time_Value *timeout = 0,
                     const ACE_ATM_Addr &local_sap = ACE_ATM_Addr( "", 0 ),
                     int reuse_addr = 0,
#if defined (ACE_WIN32)
                     int flags = 0,
#else
                     int flags = O_RDWR,
#endif /* ACE_WIN32 */
                     int perms = 0);

  /**
   * Actively connect and produce a <new_stream> if things go well.
   * The <remote_sap> is the address that we are trying to connect
   * with.  The <params> are the parameters needed for either socket
   * or XTI/ATM connections.  The <timeout> is the amount of time to
   * wait to connect. If it's 0 then we block indefinitely.  If
   * *timeout == {0, 0} then the connection is done using non-blocking
   * mode.  In this case, if the connection can't be made immediately
   * the value of -1 is returned with <errno == EWOULDBLOCK>.  If
   * *timeout > {0, 0} then this is the maximum amount of time to wait before
   * timing out.  If the time expires before the connection is made
   * <errno == ETIME>.  The <local_sap> is the value of local address
   * to bind to.  If it's the default value of <ACE_ATM_Addr::sap_any> then
   * the user is letting the OS do the binding.  If <reuse_addr> == 1
   * then the <local_addr> is reused, even if it hasn't been cleanedup yet.
   */
  int connect (ACE_ATM_Stream &new_stream,
               const ACE_ATM_Addr &remote_sap,
               ACE_ATM_Params params = ACE_ATM_Params(),
               ACE_ATM_QoS options = ACE_ATM_QoS(),
               ACE_Time_Value *timeout = 0,
               const ACE_ATM_Addr &local_sap = ACE_ATM_Addr( "",
                                                             0 ),
               int reuse_addr = 0,
#if defined (ACE_WIN32)
               int flags = 0,
#else
               int flags = O_RDWR,
#endif /* ACE_WIN32 */
               int perms = 0);

  /**
   * Try to complete a non-blocking connection.
   * If connection completion is successful then <new_stream> contains
   * the connected ACE_SOCK_Stream.  If <remote_sap> is non-NULL then it
   * will contain the address of the connected peer.
   */
  int complete (ACE_ATM_Stream &new_stream,
                ACE_ATM_Addr *remote_sap,
                ACE_Time_Value *tv);

  //int add_leaf (ACE_ATM_Stream &current_stream,
  //              const ACE_Addr &remote_sap,
  //              ACE_INT32 leaf_id,
  //              ACE_Time_Value *timeout = 0);

  /**
   * Actively add a leaf to the root (i.e., point-to-multipoint). The
   * <remote_sap> is the address of the leaf that we
   * are trying to add.
   */
  int add_leaf (ACE_ATM_Stream &current_stream,
                const ACE_Addr &remote_sap,
                ACE_ATM_QoS &qos);

  /// Resets any event associations on this handle
  int reset_new_handle (ACE_HANDLE handle);

  // = Meta-type info
  typedef ACE_ATM_Addr PEER_ADDR;
  typedef ACE_ATM_Stream PEER_STREAM;

  /// Dump the state of an object.
  void dump (void) const;

  /// Declare the dynamic allocation hooks.
  ACE_ALLOC_HOOK_DECLARE;

private:
  ATM_Connector connector_;
};

#if defined (__ACE_INLINE__)
#include "ace/ATM_Connector.i"
#endif /* __ACE_INLINE__ */

#endif /* ACE_HAS_ATM */
#include "ace/post.h"
#endif /* ACE_ATM_CONNECTOR_H */
