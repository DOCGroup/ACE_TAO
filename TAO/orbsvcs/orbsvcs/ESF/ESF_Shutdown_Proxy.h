/* -*- C++ -*- */
// $Id$
//
// ============================================================================
//
// = LIBRARY
//   ORBSVCS Event Service Framework
//
// = FILENAME
//   ESF_Shutdown_Proxy
//
// = AUTHOR
//   Carlos O'Ryan (coryan@cs.wustl.edu)
//
// = CREDITS
//   http://www.cs.wustl.edu/~coryan/EC/index.html
//
// ============================================================================

#ifndef TAO_ESF_SHUTDOWN_PROXY_H
#define TAO_ESF_SHUTDOWN_PROXY_H

#include "ESF_Worker.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

template<class PROXY>
class TAO_ESF_Shutdown_Proxy : public TAO_ESF_Worker<PROXY>
{
public:
  TAO_ESF_Shutdown_Proxy (void);

  void work (PROXY *proxy,
             CORBA::Environment &ACE_TRY_ENV);
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
