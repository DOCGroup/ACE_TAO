/* -*- C++ -*- */
// $Id$


// ============================================================================
//
// = LIBRARY
//    ace
// 
// = FILENAME
//    UPIPE_Connector.h
//
// = AUTHOR
//    Gerhard Lenzer and Douglas C. Schmidt
// 
// ============================================================================

#if !defined (ACE_UPIPE_CONNECTOR_H)
#define ACE_UPIPE_CONNECTOR_H

#include "ace/UPIPE_Stream.h"
#include "ace/Synch.h"
#include "ace/SPIPE_Stream.h"

#if defined (ACE_HAS_THREADS)

class ACE_Export ACE_UPIPE_Connector : public ACE_SPIPE
  // = TITLE
  //     Defines an active connection factory for the
  //     <ACE_UPIPE_STREAM> wrappers.
{
public:
  // = Initialization methods.
  ACE_UPIPE_Connector (void);
  // Default constructor.

  ACE_UPIPE_Connector (ACE_UPIPE_Stream &new_stream, 
		       const ACE_UPIPE_Addr &addr, 
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
  // time expires before the connection is made <errno == ETIMEDOUT>.
  // The <local_sap> is the value of local address to bind to.  If
  // it's the default value of <ACE_Addr::sap_any> then the user is
  // letting the OS do the binding.  If <reuse_addr> == 1 then the
  // <local_addr> is reused, even if it hasn't been cleanedup yet.
  // The <flags> and <perms> arguments are passed down to the open()
  // method.

  int connect (ACE_UPIPE_Stream &new_stream, 
	       const ACE_UPIPE_Addr &addr, 
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
  // time expires before the connection is made <errno == ETIMEDOUT>.
  // The <local_sap> is the value of local address to bind to.  If
  // it's the default value of <ACE_Addr::sap_any> then the user is
  // letting the OS do the binding.  If <reuse_addr> == 1 then the
  // <local_addr> is reused, even if it hasn't been cleanedup yet.
  // The <flags> and <perms> arguments are passed down to the open()
  // method.

  void dump (void) const;
  // Dump the state of an object.

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.
};

#include "ace/UPIPE_Connector.i"
#endif /* ACE_HAS_THREADS */
#endif /* ACE_UPIPE_CONNECTOR_H */
