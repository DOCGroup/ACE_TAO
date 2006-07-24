// -*- C++ -*-

//=============================================================================
/**
 *  @file   Smart_Proxies.h
 *
 *  $Id$
 *
 * This file contains a base class for the generated smart proxy classes
 * It's purpose is to hold the pointer to the real proxy
 *
 *  @author Brian Wallis <brian.wallis@ot.com.au>
 */
//=============================================================================

#ifndef TAO_SMARTPROXIES_H
#define TAO_SMARTPROXIES_H
#include /**/ "ace/pre.h"

#include "tao/SmartProxies/smartproxies_export.h"
#include "tao/Object.h"
#include "tao/TAO_Singleton.h"
#include "tao/LocalObject.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

/**
 * @class TAO_Smart_Proxy_Base
 *
 * @brief A base class for all TAO_IDL generated smart proxy base classes.
 *
 * Contains the _var pointer to the real proxy.
 */
class TAO_SmartProxies_Export TAO_Smart_Proxy_Base
  : public TAO_Local_RefCounted_Object
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

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined (__ACE_INLINE__)
#include "tao/SmartProxies/Smart_Proxies.inl"
#endif /* defined INLINE */

#include /**/ "ace/post.h"
#endif /* TAO_SMARTPROXIES_H */
