/* -*- C++ -*- */
// $Id$


// ============================================================================
//
// = LIBRARY
//    ACE
//
// = FILENAME
//    Name_Proxy.h
//
// = DESCRIPTION
//    Proxy for dealing with remote server process managing NET_LOCAL
//    Name_Bindings.
//
// = AUTHOR
//    Gerhard Lenzer, Douglas C. Schmidt, and Prashant Jain
//
// ============================================================================

#if !defined (ACE_NAME_PROXY_H)
#define ACE_NAME_PROXY_H

#include "ace/INET_Addr.h"
#include "ace/SOCK_Connector.h"
#include "ace/SOCK_Stream.h"
#include "ace/Service_Config.h"
#include "ace/Synch_Options.h"
#include "ace/Name_Request_Reply.h"

class ACE_Export ACE_Name_Proxy : public ACE_Event_Handler
  // = TITLE
  //   Proxy for dealing with remote server process managing NET_LOCAL NameBindings
  //
  // = DESCRIPTION
  //   Shields applications from details of interacting with the ACE_Name Server.
{
public:
  ACE_Name_Proxy (void);
  // Default constructor.

  // = Establish a binding with the ACE_Name Server.
  ACE_Name_Proxy (const ACE_INET_Addr &remote_addr, // Address of ACE_Name Server.
		  ACE_Synch_Options& options =
		  ACE_Synch_Options::defaults);

  int open (const ACE_INET_Addr &remote_addr,  // Address of ACE_Name Server.
	    ACE_Synch_Options& options =
	    ACE_Synch_Options::defaults);

  int request_reply (ACE_Name_Request &request);
  // Perform the request and wait for the reply.

  int send_request (ACE_Name_Request &request);
  // Perform the request.

  int recv_reply (ACE_Name_Request &reply);
  // Receive the reply.

  virtual ACE_HANDLE get_handle (void) const;
  // Obtain underlying handle.

  virtual ~ACE_Name_Proxy (void);
  // Close down the connection to the server.

  void dump (void) const;
  // Dump the state of the object;

private:

  ACE_SOCK_Connector connector_;
  // ACE_Connector factory used to establish connections actively.

  ACE_SOCK_Stream peer_;
  // Connection to ACE_Name Server peer.

  ACE_Reactor *reactor_;
  // Pointer to ACE_Reactor (used if we are run in "reactive-mode").
};

#endif /* ACE_NAME_PROXY_H */
