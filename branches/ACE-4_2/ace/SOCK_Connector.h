/* -*- C++ -*- */
// $Id$


// ============================================================================
//
// = LIBRARY
//    ace
// 
// = FILENAME
//    SOCK_Connector.h 
//
// = AUTHOR
//    Doug Schmidt 
// 
// ============================================================================

#if !defined (ACE_SOCK_CONNECTOR_H)
#define ACE_SOCK_CONNECTOR_H

#include "ace/SOCK_Stream.h"
#include "ace/Time_Value.h"

class ACE_Export ACE_SOCK_Connector : public ACE_SOCK
  // = TITLE
  //     Defines an active connection factory for the socket wrappers. 
{
public:
  // = Initialization routines.
  ACE_SOCK_Connector (void);
  // Default constructor.

  ACE_SOCK_Connector (ACE_SOCK_Stream &new_stream, 
		      const ACE_Addr &remote_sap,
		      ACE_Time_Value *timeout = 0,
		      const ACE_Addr &local_sap = ACE_Addr::sap_any,
		      int reuse_addr = 0,
		      int flags = 0,
		      int perms = 0,
		      int protocol_family = PF_INET, 
		      int protocol = 0);
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

  int connect (ACE_SOCK_Stream &new_stream, 
	       const ACE_Addr &remote_sap,
	       ACE_Time_Value *timeout = 0,
	       const ACE_Addr &local_sap = ACE_Addr::sap_any,
	       int reuse_addr = 0,
 	       int flags = 0,
	       int perms = 0,
	       int protcol_family = PF_INET, 
	       int protocol = 0); 
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

  // = Completion routine.
  int complete (ACE_SOCK_Stream &new_stream, 
		ACE_Addr *remote_sap = 0,
		ACE_Time_Value *timeout = 0);
  // Try to complete a non-blocking connection.
  // If connection completion is successful then <new_stream> contains
  // the connected ACE_SOCK_Stream.  If <remote_sap> is non-NULL then it
  // will contain the address of the connected peer.

  // = Meta-type info
  typedef ACE_INET_Addr PEER_ADDR;
  typedef ACE_SOCK_Stream PEER_STREAM;

  void dump (void) const;
  // Dump the state of an object.

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.
};

#include "ace/SOCK_Connector.i"

#endif /* ACE_SOCK_CONNECTOR_H */
