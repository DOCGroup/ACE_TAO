/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    ace
//
// = FILENAME
//    SPIPE_Connector.h
//
// = AUTHOR
//    Doug Schmidt and Prashant Jain
//
// ============================================================================

#ifndef ACE_SPIPE_CONNECTOR_H
#define ACE_SPIPE_CONNECTOR_H

#include "ace/SPIPE_Stream.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class ACE_Export ACE_SPIPE_Connector
{
  // = TITLE
  //     Defines an active connection factory for the STREAM pipe
  //     wrappers.
public:
  // = Initialization method.
  ACE_SPIPE_Connector (void);
  // Default constructor.

  ACE_SPIPE_Connector (ACE_SPIPE_Stream &new_io,
                       const ACE_SPIPE_Addr &remote_sap,
                       ACE_Time_Value *timeout = 0,
                       const ACE_Addr &local_sap = ACE_Addr::sap_any,
                       int reuse_addr = 0,
                       int flags = O_RDWR,
                       int perms = 0);
  // Actively connect and produce a <new_stream> if things go well.
  // The <remote_sap> is the address that we are trying to connect
  // with.  The <timeout> is the amount of time to wait to connect.
  // If it's 0 then we block indefinitely.  If *timeout == {0, 0} then
  // the connection is done using non-blocking mode.  In this case, if
  // the connection can't be made immediately the value of -1 is
  // returned with <errno == EWOULDBLOCK>.  If *timeout > {0, 0} then
  // this is the amount of time to wait before timing out.  If the
  // time expires before the connection is made <errno == ETIME>.  The
  // <local_sap> is the value of local address to bind to.  If it's
  // the default value of <ACE_Addr::sap_any> then the user is letting
  // the OS do the binding.  If <reuse_addr> == 1 then the
  // <local_addr> is reused, even if it hasn't been cleanedup yet.
  // The <flags> and <perms> arguments are passed down to the open()
  // method.

  int connect (ACE_SPIPE_Stream &new_io,
               const ACE_SPIPE_Addr &remote_sap,
               ACE_Time_Value *timeout = 0,
               const ACE_Addr &local_sap = ACE_Addr::sap_any,
               int reuse_addr = 0,
               int flags = O_RDWR,
               int perms = 0);
  // Actively connect and produce a <new_stream> if things go well.
  // The <remote_sap> is the address that we are trying to connect
  // with.  The <timeout> is the amount of time to wait to connect.
  // If it's 0 then we block indefinitely.  If *timeout == {0, 0} then
  // the connection is done using non-blocking mode.  In this case, if
  // the connection can't be made immediately the value of -1 is
  // returned with <errno == EWOULDBLOCK>.  If *timeout > {0, 0} then
  // this is the amount of time to wait before timing out.  If the
  // time expires before the connection is made <errno == ETIME>.  The
  // <local_sap> is the value of local address to bind to.  If it's
  // the default value of <ACE_Addr::sap_any> then the user is letting
  // the OS do the binding.  If <reuse_addr> == 1 then the
  // <local_addr> is reused, even if it hasn't been cleanedup yet.
  // The <flags> and <perms> arguments are passed down to the open()
  // method.

  int reset_new_handle (ACE_HANDLE handle);
  // Resets any event associations on this handle

  // = Meta-type info
  typedef ACE_SPIPE_Addr PEER_ADDR;
  typedef ACE_SPIPE_Stream PEER_STREAM;

  void dump (void) const;
  // Dump the state of an object.

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.
};

#if !defined (ACE_LACKS_INLINE_FUNCTIONS)
#include "ace/SPIPE_Connector.i"
#endif

#endif /* ACE_SPIPE_CONNECTOR_H */
