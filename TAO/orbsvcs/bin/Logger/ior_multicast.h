/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/orbsvcs/bin/Logger
//
// = FILENAME
//    ior_multicast.h
//
// = DESCRIPTION 
//    Listens to multicast address for client requests for ior's.
//    
// = AUTHORS
//      Sergio Flores-Gaitan
//
// ============================================================================

#if !defined (IOR_MULTICAST_H)
#define IOR_MULTICAST_H

#include "ace/INET_Addr.h"
#include "ace/SOCK_Dgram_Mcast.h"
#include "ace/Reactor.h"

// defines
#define DEFAULT_LOGGER_SERVER_REQUEST_PORT 10030
#define DEFAULT_LOGGER_SERVER_REPLY_PORT 10031
#define DEFAULT_LOGGER_SERVER_MULTICAST_ADDR ACE_DEFAULT_MULTICAST_ADDR
#define DEFAULT_LOGGER_SERVER_TIMEOUT 5

class IOR_Multicast : public ACE_Event_Handler
{
public:
  IOR_Multicast (char * ior,
		u_short port,
		const char *mcast_addr,
		u_short response_port);
  ~IOR_Multicast (void);

  virtual int handle_input (ACE_HANDLE fd);
  virtual int handle_timeout (const ACE_Time_Value &tv,
			      const void *arg);

  virtual ACE_HANDLE get_handle (void) const;

private:
  char buf_[BUFSIZ];

  ACE_SOCK_Dgram_Mcast mcast_dgram_;
  ACE_INET_Addr remote_addr_;
  ACE_INET_Addr mcast_addr_;
  char * ior_;
  const u_short SERVICE_RESPONSE_UDP_PORT_;
  ACE_INET_Addr response_addr_;
  ACE_SOCK_Dgram response_;

};

#endif /* IOR_MULTICAST_H */
