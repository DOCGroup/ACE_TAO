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

#if !defined (ACE_FILE_CONNECTOR_H)
#define ACE_FILE_CONNECTOR_H

#include "ace/FILE_IO.h"

class ACE_Export ACE_FILE_Connector : public ACE_FILE
  // = TITLE
  //     Defines an active connection factory for the ACE_FILE wrappers. 
{
public:
  // = Initialization methods.
  ACE_FILE_Connector (void);
  // Default constructor.

  ACE_FILE_Connector (ACE_FILE_IO &new_io, 
		      const ACE_FILE_Addr &remote_sap, 
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

  int connect (ACE_FILE_IO &new_io, 
	       const ACE_FILE_Addr &remote_sap, 
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

  // = Meta-type info
  typedef ACE_FILE_Addr PEER_ADDR;
  typedef ACE_FILE_IO   PEER_STREAM;
};

#include "ace/FILE_Connector.i"

#endif /* ACE_FILE_CONNECTOR_H */
