// This may look like C, but it's really -*- C++ -*-

//=============================================================================
/**
 *  @file   OC_Endpoint_Selector_Loader.h
 *
 *  $Id$
 *
 * Strategies for selecting profile/endpoint from an IOR for making an
 * invocation.
 *
 *  @author Phil Mesnier <mesnier_p@ociweb.com>
 */
//=============================================================================


#ifndef TAO_OC_ENDPOINT_LOADER_H
#define TAO_OC_ENDPOINT_LOADER_H
#include /**/ "ace/pre.h"

#include "tao/Strategies/strategies_export.h"
#include "tao/Strategies/OC_Endpoint_Selector_Factory.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */


TAO_BEGIN_VERSIONED_NAMESPACE_DECL

// Forward references
class TAO_Optimized_Connection_Endpoint_Selector;

// ****************************************************************

/**
 * @class TAO_OC_Endpoint_Selector_Loader
 *
 * @brief Static initializer used to ensure the Optimized Connector
 * Endpoint Selector is available for use in static applications
 *
 * This class is not needed when using dynamic linking and the service
 * configuratior. Statically linked applications need to include this
 * header to ensure that all the necessary dependencies are met. In
 * addition, the svc config directive is required for setting any
 * initialization parameters for the endpoint selector.
 *
 */
class TAO_Strategies_Export TAO_OC_Endpoint_Selector_Loader
{
public:
  static int init(void);
};

static int
TAO_Requires_OC_Endpoint_Selector_Loader =
  TAO_OC_Endpoint_Selector_Loader::init ();


TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"
#endif  /* TAO_OC_ENDPOINT_LOADER_H */
