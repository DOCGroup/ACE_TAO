/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/orbsvcs/orbsvcs
//
// = FILENAME
//    IOR_Multicast.h
//
// = DESCRIPTION
//    Defines a class that listens to a multicast address for client requests
//    for ior of a bootstrappable service.
//
// = AUTHORS
//      Sergio Flores-Gaitan
//
// ============================================================================

#ifndef TAO_IOR_MULTICAST_H
#define TAO_IOR_MULTICAST_H

#include "tao/corba.h"
#include "orbsvcs/orbsvcs_export.h"
#include "ace/INET_Addr.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/SOCK_Dgram_Mcast.h"
#include "ace/Reactor.h"

class TAO_ORBSVCS_Export TAO_IOR_Multicast : public ACE_Event_Handler
{
  // = TITLE
  //     Event Handler that services multicast requests for IOR of a
  //     bootstrappable service. 
  //
  // = DESCRIPTION
  //     This class uses the ACE_SOCK_Dgram_Mcast class and should be
  //     registered with a reactor and should be initialized with the
  //     ior of the  service to be multicasted.
public:
  TAO_IOR_Multicast (void);
  // Constructor.

  TAO_IOR_Multicast (const char *ior,
                     u_short port,
                     const char *mcast_addr,
                     TAO_Service_ID service_id);
  // Constructor taking the ior of the service.

  int init (const char *ior,
            u_short port,
            const char *mcast_addr,
            TAO_Service_ID service_id);
  // Initialization method.

  ~TAO_IOR_Multicast (void);
  // Destructor.

  virtual int handle_input (ACE_HANDLE n);
  // Callback when input is received on the handle.

  virtual int handle_timeout (const ACE_Time_Value &tv,
                              const void *arg);
  // Callback when a timeout has occurred.

  virtual ACE_HANDLE get_handle (void) const;
  // Returns the internal handle used to receive multicast.

private:
  char buf_[BUFSIZ];
  // temporary buffer.

  TAO_Service_ID service_id_;
  // Service id that we're waiting for.

  ACE_SOCK_Dgram_Mcast mcast_dgram_;
  // multicast endpoint of communication

  ACE_INET_Addr mcast_addr_;
  // multicast address

  const char *ior_;
  // object reference to send in response to the multicast

  ACE_INET_Addr response_addr_;
  // address of response.

  ACE_SOCK_Dgram response_;
  // socket for response to the multicast
};

#endif /* TAO_IOR_MULTICAST_H */




