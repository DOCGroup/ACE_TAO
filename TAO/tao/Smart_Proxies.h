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

#include "tao/Object.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class TAO_Export TAO_Smart_Proxy_Base 
{
  // = TITLE
  //   TAO_Smart_Proxy_Base
  //
  // = DESCRIPTION
  //   A base class for all TAO_IDL generated smart proxy base classes
  //   Contains the _var pointer to the real proxy
	
public:
  virtual ~TAO_Smart_Proxy_Base(void);
  // Destructor
  
protected:
  TAO_Smart_Proxy_Base (void);
  TAO_Smart_Proxy_Base (CORBA::Object_ptr proxy);
  // Constructors
  
  CORBA::Object_var base_proxy_;
  // The proxy to which remote/collocated calls are made.
};

#if defined (__ACE_INLINE__)
#include "tao/Smart_Proxies.i"
#endif /* defined INLINE */

#endif /* TAO_SMARTPROXIES_H */
