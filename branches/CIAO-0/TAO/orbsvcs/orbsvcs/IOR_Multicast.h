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
#include "ace/pre.h"

#include "orbsvcs/svc_utils_export.h"
#include "tao/corba.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/INET_Addr.h"
#include "ace/SOCK_Dgram_Mcast.h"
#include "ace/Reactor.h"

class TAO_Svc_Utils_Export TAO_IOR_Multicast : public ACE_Event_Handler
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

  int init (const char *ior,
            const char *mcast_addr,
            TAO_Service_ID service_id);
  // Initialization method. Takes in "address:port" string as a
  // parameter.

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
  int common_init (const char *ior,
                   TAO_Service_ID service_id);
  // Factor common functionality from the two init functions.

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

  const char *mcast_nic_;
};

#include "ace/post.h"
#endif /* TAO_IOR_MULTICAST_H */
