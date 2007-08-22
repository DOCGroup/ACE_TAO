// This may look like C, but it's really -*- C++ -*-

//=============================================================================
/**
 *  @file   OC_Endpoint_Selector_Factory.h
 *
 *  $Id$
 *
 * Strategies for selecting profile/endpoint from an IOR for making an
 * invocation.
 *
 *  @author Phil Mesnier <mesnier_p@ociweb.com>
 */
//=============================================================================


#ifndef TAO_OC_ENDPOINT_SELECTOR_FACTORY_H
#define TAO_OC_ENDPOINT_SELECTOR_FACTORY_H
#include /**/ "ace/pre.h"


#include "tao/Strategies/strategies_export.h"

#include "tao/Endpoint_Selector_Factory.h"
#include "ace/Service_Config.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

// Forward references
class TAO_Optimized_Connection_Endpoint_Selector;

// ****************************************************************

/**
 * @class TAO_OC_Endpoint_Selector_Factory
 *
 * @brief Factory for initializing <Endpoint_Selection_State> and
 * obtaining appropriate <Invocation_Endpoint_Selector>.
 *
 * Used by Invocation classes to initialize its endpoint selection
 * strategy and state based on the effective policies.
 * Endpoint selection strategies are stateless objects - all the
 * state they need is contained by Invocation in
 * <Endpoint_Selection_State>.  Thus, rather than allocating an
 * endpoint selection strategy object for each Invocation, the
 * factory simply returns the appropriate one from the
 * set preallocated in the ORB_Core.  One endpoint selection
 * strategy object can be used by many invocations concurrently.
 */
class TAO_Strategies_Export TAO_OC_Endpoint_Selector_Factory
  : public TAO_Endpoint_Selector_Factory
{
public:
  /// Constructor.
  TAO_OC_Endpoint_Selector_Factory (void);

  /// Destructor.
  virtual ~TAO_OC_Endpoint_Selector_Factory (void);

  virtual int init (int argc, ACE_TCHAR *argv[]);

  /// Get an Invocation's endpoint selection strategy and
  /// initialize the endpoint selection state instance.
  virtual TAO_Invocation_Endpoint_Selector *get_selector (void);

protected:
  /// The possible endpoint selector strategies that can be
  /// returned by this factory

  TAO_Optimized_Connection_Endpoint_Selector *oc_endpoint_selector_;

  int register_orb_initializer (void);
};


TAO_END_VERSIONED_NAMESPACE_DECL


ACE_STATIC_SVC_DECLARE_EXPORT (TAO_Strategies, TAO_OC_Endpoint_Selector_Factory)
ACE_FACTORY_DECLARE (TAO_Strategies, TAO_OC_Endpoint_Selector_Factory)

#include /**/ "ace/post.h"
#endif  /* TAO_OC_ENDPOINT_SELECTOR_FACTORY_H */
