/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    MEM_Connector.h
 *
 *  $Id$
 *
 *  @author Nanbor Wang <nanbor@cs.wustl.edu>
 */
//=============================================================================

#ifndef ACE_MEM_CONNECTOR_H
#define ACE_MEM_CONNECTOR_H
#include "ace/pre.h"

#include "ace/SOCK_Connector.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if (ACE_HAS_POSITION_INDEPENDENT_POINTERS == 1)

#include "ace/MEM_Stream.h"
#include "ace/MEM_Addr.h"

/**
 * @class ACE_MEM_Connector
 *
 * @brief Defines the format and interface for the connector side of
 * the <ACE_MEM_Stream>.
 */
class ACE_Export ACE_MEM_Connector : public ACE_SOCK_Connector
{
public:
  // = Initialization methods.
  /// Default constructor.
  ACE_MEM_Connector (void);

  /**
   * Actively connect and produce a <new_stream> if things go well.
   * The <remote_sap> is the address that we are trying to connect
   * with.  The <timeout> is the amount of time to wait to connect.
   * If it's 0 then we block indefinitely.  If *timeout == {0, 0} then
   * the connection is done using non-blocking mode.  In this case, if
   * the connection can't be made immediately the value of -1 is
   * returned with <errno == EWOULDBLOCK>.  If *timeout > {0, 0} then
   * this is the maximum amount of time to wait before timing out.  If the
   * time expires before the connection is made <errno == ETIME>.  The
   * <local_sap> is the value of local address to bind to.  If it's
   * the default value of <0> then the user is letting the OS do the
   * binding.  If <reuse_addr> == 1 then the <local_addr> is reused,
   * even if it hasn't been cleanedup yet.
   */
  ACE_MEM_Connector (ACE_MEM_Stream &new_stream,
                     const ACE_INET_Addr &remote_sap,
                     ACE_Time_Value *timeout = 0,
                     const ACE_Addr &local_sap = ACE_Addr::sap_any,
                     int reuse_addr = 0,
                     int flags = 0,
                     int perms = 0,
                     int protocol = 0);

  /**
   * Actively connect and produce a <new_stream> if things go well.
   * The <remote_sap> is the address that we are trying to connect
   * with.  The <timeout> is the amount of time to wait to connect.
   * If it's 0 then we block indefinitely.  If *timeout == {0, 0} then
   * the connection is done using non-blocking mode.  In this case, if
   * the connection can't be made immediately the value of -1 is
   * returned with <errno == EWOULDBLOCK>.  If *timeout > {0, 0} then
   * this is the maximum amount of time to wait before timing out.  If the
   * time expires before the connection is made <errno == ETIME>.  The
   * <local_sap> is the value of local address to bind to.  If it's
   * the default value of <0> then the user is letting the OS do the
   * binding.  If <reuse_addr> == 1 then the <local_addr> is reused,
   * even if it hasn't been cleanedup yet.
   */
  int connect (ACE_MEM_Stream &new_stream,
               const ACE_INET_Addr &remote_sap,
               ACE_Time_Value *timeout = 0,
               const ACE_Addr &local_sap = ACE_Addr::sap_any,
               int reuse_addr = 0,
               int flags = 0,
               int perms = 0,
               int protocol = 0);

  // Set/get the preferred signaling strategy.
  ACE_MEM_IO::Signal_Strategy preferred_strategy (void) const;
  void preferred_strategy (ACE_MEM_IO::Signal_Strategy strategy);

  /// Accessor to underlying malloc options.
  ACE_MEM_SAP::MALLOC_OPTIONS &malloc_options (void);

  // = Meta-type info
  typedef ACE_INET_Addr PEER_ADDR;
  typedef ACE_MEM_Stream PEER_STREAM;

  /// Dump the state of an object.
  void dump (void) const;

  /// Declare the dynamic allocation hooks.
  ACE_ALLOC_HOOK_DECLARE;

private:
  /// The acceptor address this connector is connecting to.
  ACE_MEM_Addr address_;

  /// A cached MALLOC_OPTIONS that the MEM_Connector used to initialize
  /// the shared memory malloc update connection establishment.
  ACE_MEM_SAP::MALLOC_OPTIONS malloc_options_;

  // Preferred signaling strategy.
  ACE_MEM_IO::Signal_Strategy preferred_strategy_;
};

#if !defined (ACE_LACKS_INLINE_FUNCTIONS)
#include "ace/MEM_Connector.i"
#endif

#endif /* ACE_HAS_POSITION_INDEPENDENT_POINTERS == 1 */

#include "ace/post.h"
#endif /* ACE_MEM_CONNECTOR_H */
