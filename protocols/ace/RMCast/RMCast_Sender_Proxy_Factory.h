/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    ace
//
// = FILENAME
//    RMCast_Sender_Proxy_Factory.h
//
// = AUTHOR
//    Carlos O'Ryan <coryan@uci.edu>
//
// ============================================================================

#ifndef ACE_RMCAST_SENDER_PROXY_FACTORY_H
#define ACE_RMCAST_SENDER_PROXY_FACTORY_H
#include "ace/pre.h"

#include "RMCast.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class ACE_RMCast_Sender_Proxy;

class ACE_RMCast_Export ACE_RMCast_Sender_Proxy_Factory
{
  // = DESCRIPTION
  //   Defines the interface to create Sender_Proxies.
  //   The application provides a Sender_Proxy_Factory, this is used
  //   by the receiver side to create a different proxy for each
  //   remote sender.  The application configures the proxy with the
  //   correct modules to process incoming events and achieve the
  //   desired level of reliability.  
  //
public:
  virtual ~ACE_RMCast_Sender_Proxy_Factory (void);
  // Destructor

  virtual ACE_RMCast_Sender_Proxy *create (void) = 0;
  // Create a new proxy

  virtual void destroy (ACE_RMCast_Sender_Proxy *) = 0;
  // Destroy a proxy
};

#if defined (__ACE_INLINE__)
#include "RMCast_Sender_Proxy_Factory.i"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* ACE_RMCAST_SENDER_PROXY_FACTORY_H */
