/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/orbsvcs/bin/Naming_Service
//
// = FILENAME
//    svr.h
//
// = DESCRIPTION 
//    Defines a class that listens to a multicast address for client requests 
//    for ior of the naming service.
//    
// = AUTHORS
//      Sergio Flores-Gaitan
//
// ============================================================================

#if !defined (SVR_H)
#define SVR_H

#include "ace/INET_Addr.h"
#include "ace/SOCK_Dgram_Mcast.h"
#include "ace/Reactor.h"

class IOR_Multicast : public ACE_Event_Handler
{
public:
  // constructor
  IOR_Multicast (char * ior,
		u_short port,
		const char *mcast_addr);

  // destructor
  ~IOR_Multicast (void);

  // call back when input is received on the handle.
  virtual int handle_input (ACE_HANDLE fd);
  
  // callback when a timeout has occurred.
  virtual int handle_timeout (const ACE_Time_Value &tv,
			      const void *arg);

  // returns the internal handle used to receive multicast
  virtual ACE_HANDLE get_handle (void) const;

private:
  char buf_[BUFSIZ];
  // temporary buffer

  ACE_SOCK_Dgram_Mcast mcast_dgram_;
  // multicast endpoint of communication

  ACE_INET_Addr remote_addr_;
  // remote address that sent multicast

  ACE_INET_Addr mcast_addr_;
  // multicast address

  char * ior_;
  // object reference to send in response to the multicast

  ACE_INET_Addr response_addr_;
  // address of response.

  ACE_SOCK_Dgram response_;
  // socket for response to the multicast
};

#endif /* SVR_H */
