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
//    for ior of a bootstrappable service.
//    
// = AUTHORS
//      Sergio Flores-Gaitan
//
// ============================================================================

#if !defined (TAO_IOR_MULTICAST_H)
#define TAO_IOR_MULTICAST_H

#include "tao/corba.h"
#include "orbsvcs/orbsvcs_export.h"
#include "ace/INET_Addr.h"
#include "ace/SOCK_Dgram_Mcast.h"
#include "ace/Reactor.h"

class TAO_ORBSVCS_Export TAO_IOR_Multicast : public ACE_Event_Handler
{
  // @@ Naga, can you please add the standard comments to this class and
  // make sure that all the comments for each method go underneath the
  // method name?
public:

  TAO_IOR_Multicast (void);
  // Default constructor  

  TAO_IOR_Multicast (char * ior,
                     u_short port,
                     const char *mcast_addr,
                     TAO_Service_ID service_id);

  int init (char* ior,
            u_short port,
            const char* mcast_addr,
            TAO_Service_ID service_id);
  
  // destructor
  ~TAO_IOR_Multicast (void);

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

  TAO_Service_ID service_id_;
  // Service id that we're waiting for. 
  
  ACE_SOCK_Dgram_Mcast mcast_dgram_;
  // multicast endpoint of communication

  ACE_INET_Addr mcast_addr_;
  // multicast address

  char * ior_;
  // object reference to send in response to the multicast

  ACE_INET_Addr response_addr_;
  // address of response.

  ACE_SOCK_Dgram response_;
  // socket for response to the multicast
};

#endif /* NAMING_SERVICE_H */




