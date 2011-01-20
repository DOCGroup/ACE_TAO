// -*- C++ -*-


//=============================================================================
/**
 *  @file    IOR_Multicast.h
 *
 *  $Id$
 *
 *  Defines a class that listens to a multicast address for client requests
 *  for ior of a bootstrappable service.
 *
 *
 *  @author   Sergio Flores-Gaitan
 */
//=============================================================================


#ifndef TAO_IOR_MULTICAST_H
#define TAO_IOR_MULTICAST_H
#include /**/ "ace/pre.h"

#include "orbsvcs/svc_utils_export.h"
#include "tao/ORB.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/INET_Addr.h"
#include "ace/SOCK_Dgram_Mcast.h"
#include "ace/Reactor.h"
#include "ace/SString.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

/**
 * @class TAO_IOR_Multicast
 *
 * @brief Event Handler that services multicast requests for IOR of a
 * bootstrappable service.
 *
 * This class uses the ACE_SOCK_Dgram_Mcast class and should be
 * registered with a reactor and should be initialized with the
 * ior of the  service to be multicasted.
 */
class TAO_Svc_Utils_Export TAO_IOR_Multicast : public ACE_Event_Handler
{
public:
  /// Constructor.
  TAO_IOR_Multicast (void);

  /// Constructor taking the ior of the service.
  TAO_IOR_Multicast (const char *ior,
                     u_short port,
                     const char *mcast_addr,
                     TAO_Service_ID service_id);

  /// Initialization method.
  int init (const char *ior,
            u_short port,
            const char *mcast_addr,
            TAO_Service_ID service_id);

  /// Initialization method. Takes in "address:port" string as a
  /// parameter.
  int init (const char *ior,
            const char *mcast_addr,
            TAO_Service_ID service_id);

  /// Destructor.
  virtual ~TAO_IOR_Multicast (void);

  /// Callback when input is received on the handle.
  virtual int handle_input (ACE_HANDLE n);

  /// Callback when a timeout has occurred.
  virtual int handle_timeout (const ACE_Time_Value &tv,
                              const void *arg);

  /// Returns the internal handle used to receive multicast.
  virtual ACE_HANDLE get_handle (void) const;

private:
  /// Factor common functionality from the two init functions.
  int common_init (const char *ior,
                   TAO_Service_ID service_id);

  /// Service id that we're waiting for.
  TAO_Service_ID service_id_;

  /// multicast endpoint of communication
  ACE_SOCK_Dgram_Mcast mcast_dgram_;

  /// multicast address
  ACE_INET_Addr mcast_addr_;

  /// object reference to send in response to the multicast
  ACE_CString ior_;

  /// address of response.
  ACE_INET_Addr response_addr_;

  /// socket for response to the multicast
  ACE_SOCK_Dgram response_;

  ACE_CString mcast_nic_;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"
#endif /* TAO_IOR_MULTICAST_H */
