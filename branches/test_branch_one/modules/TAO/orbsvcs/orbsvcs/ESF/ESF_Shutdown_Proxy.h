// -*- C++ -*-

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

#include "orbsvcs/ESF/ESF_Worker.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

/// A worker to invoke the shutdown method of each proxy.
template<class PROXY>
class TAO_ESF_Shutdown_Proxy : public TAO_ESF_Worker<PROXY>
{
public:
  TAO_ESF_Shutdown_Proxy (void);

  void work (PROXY *proxy);
};

// ****************************************************************

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined (__ACE_INLINE__)
#include "orbsvcs/ESF/ESF_Shutdown_Proxy.inl"
#endif /* __ACE_INLINE__ */

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "orbsvcs/ESF/ESF_Shutdown_Proxy.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("ESF_Shutdown_Proxy.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#endif /* TAO_ESF_SHUTDOWN_PROXY_H */
