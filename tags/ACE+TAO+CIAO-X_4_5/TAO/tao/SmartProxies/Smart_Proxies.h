// $Id$
// This may look like C, but it's really -*- C++ -*-
// SmartProxies.h,v 1.0

// ============================================================================
//
// = LIBRARY
//    TAO
//
// = FILENAME
//    SmartProxies.h
//
// = DESCRIPTION
//     This file contains a base class for the generated smart proxy classes
//     It's purpose is to hold the pointer to the real proxy
//
// = AUTHOR
//     Brian Wallis <brian.wallis@ot.com.au>
//
// ============================================================================

#ifndef TAO_SMARTPROXIES_H
#define TAO_SMARTPROXIES_H
#include /**/ "ace/pre.h"

#include "smartproxies_export.h"
#include "tao/Object.h"
#include "tao/TAO_Singleton.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

/**
 * @class TAO_Smart_Proxy_Base
 *
 * @brief A base class for all TAO_IDL generated smart proxy base classes.
 *
 * Contains the _var pointer to the real proxy.
 */
class TAO_SmartProxies_Export TAO_Smart_Proxy_Base
{

public:
  /// Destructor
  virtual ~TAO_Smart_Proxy_Base (void);

protected:
  /// Constructor.
  TAO_Smart_Proxy_Base (void);

  /// Constructor.
  TAO_Smart_Proxy_Base (CORBA::Object_ptr proxy);

  /// The proxy to which remote/collocated calls are made.
  CORBA::Object_var base_proxy_;
};

#if defined (__ACE_INLINE__)
#include "Smart_Proxies.inl"
#endif /* defined INLINE */

#include /**/ "ace/post.h"
#endif /* TAO_SMARTPROXIES_H */
