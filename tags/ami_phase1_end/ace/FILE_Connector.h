/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    ace
//
// = FILENAME
//    FILE_Connector.h
//
// = AUTHOR
//    Doug Schmidt
//
// ============================================================================

#ifndef ACE_FILE_CONNECTOR_H
#define ACE_FILE_CONNECTOR_H

#include "ace/FILE_IO.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class ACE_Export ACE_FILE_Connector
{
  // = TITLE
  //     Defines an active connection factory for the ACE_FILE wrappers.
public:
  // = Initialization methods.
  ACE_FILE_Connector (void);
  // Default constructor.

  ACE_FILE_Connector (ACE_FILE_IO &new_io,
                      const ACE_FILE_Addr &remote_sap,
                      ACE_Time_Value *timeout = 0,
                      const ACE_Addr &local_sap = ACE_Addr::sap_any,
                      int reuse_addr = 0,
                      int flags = O_RDWR | O_CREAT,
                      int perms = 0);
  // Actively ``connect'' and produce a <new_io> <ACE_FILE_IO> object
  // if things go well.  The <remote_sap> is the file that we are
  // trying to create/open.  If it's the default value of
  // <ACE_Addr::sap_any> then the user is letting the OS create the
  // filename (via <ACE_OS::mktemp>).  The <timeout> is the amount of
  // time to wait to create/open the file.  If it's 0 then we block
  // indefinitely.  If *timeout == {0, 0} then the file is created
  // using non-blocking mode.  In this case, if the create/open can't
  // be done immediately the value of -1 is returned with <errno ==
  // EWOULDBLOCK>.  If *timeout > {0, 0} then this is the amount of
  // time to wait before timing out.  If the time expires before the
  // connection is made <errno == ETIME>.  The <local_sap> and
  // <reuse_addr> parameters are ignored.  The <flags> and <perms>
  // arguments are passed down to the <ACE_OS::open> method.

  int connect (ACE_FILE_IO &new_io,
               const ACE_FILE_Addr &remote_sap,
               ACE_Time_Value *timeout = 0,
               const ACE_Addr &local_sap = ACE_Addr::sap_any,
               int reuse_addr = 0,
               int flags = O_RDWR | O_CREAT,
               int perms = 0);
  // Actively ``connect'' and produce a <new_io> <ACE_FILE_IO> object
  // if things go well.  The <remote_sap> is the file that we are
  // trying to create/open.  If it's the default value of
  // <ACE_Addr::sap_any> then the user is letting the OS create the
  // filename (via <ACE_OS::mktemp>).  The <timeout> is the amount of
  // time to wait to create/open the file.  If it's 0 then we block
  // indefinitely.  If *timeout == {0, 0} then the file is created
  // using non-blocking mode.  In this case, if the create/open can't
  // be done immediately the value of -1 is returned with <errno ==
  // EWOULDBLOCK>.  If *timeout > {0, 0} then this is the amount of
  // time to wait before timing out.  If the time expires before the
  // connection is made <errno == ETIME>.  The <local_sap> and
  // <reuse_addr> parameters are ignored.  The <flags> and <perms>
  // arguments are passed down to the <ACE_OS::open> method.

  int reset_new_handle (ACE_HANDLE handle);
  // Resets any event associations on this handle

  void dump (void) const;
  // Dump the state of an object.

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.

  // = Meta-type "trait" information.
  typedef ACE_FILE_Addr PEER_ADDR;
  typedef ACE_FILE_IO PEER_STREAM;
};

#if !defined (ACE_LACKS_INLINE_FUNCTIONS)
#include "ace/FILE_Connector.i"
#endif

#endif /* ACE_FILE_CONNECTOR_H */
