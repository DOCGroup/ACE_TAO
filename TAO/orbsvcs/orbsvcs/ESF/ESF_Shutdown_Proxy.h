/* -*- C++ -*- */
/**
 *  @file   ESF_Shutdown_Proxy.h
 *
 *  $Id$
 *
 *  @author Carlos O'Ryan (coryan@cs.wustl.edu)
 *
 *  http://doc.ece.uci.edu/~coryan/EC/index.html
 */


#ifndef TAO_ESF_SHUTDOWN_PROXY_H
#define TAO_ESF_SHUTDOWN_PROXY_H

#include "ESF_Worker.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

/// A worker to invoke the shutdown method of each proxy.
template<class PROXY>
class TAO_ESF_Shutdown_Proxy : public TAO_ESF_Worker<PROXY>
{
public:
  TAO_ESF_Shutdown_Proxy (void);

  void work (PROXY *proxy
             TAO_ENV_ARG_DECL);
};

// ****************************************************************

#if defined (__ACE_INLINE__)
#include "ESF_Shutdown_Proxy.i"
#endif /* __ACE_INLINE__ */

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "ESF_Shutdown_Proxy.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("ESF_Shutdown_Proxy.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#endif /* TAO_ESF_SHUTDOWN_PROXY_H */
