// This may look like C, but it's really -*- C++ -*-

//=============================================================================
/**
 *  @file   RT_Endpoint_Selector_Factory.h
 *
 *  $Id$
 *
 * Strategies for selecting profile/endpoint from an IOR for making an
 * invocation.
 *
 *  @author Marina Spivak <marina@cs.wustl.edu>
 */
//=============================================================================


#ifndef RT_ENDPOINT_SELECTOR_FACTORY_H
#define RT_ENDPOINT_SELECTOR_FACTORY_H
#include /**/ "ace/pre.h"

#include "rtcorba_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/Endpoint_Selector_Factory.h"

#if defined (TAO_HAS_CORBA_MESSAGING) && TAO_HAS_CORBA_MESSAGING != 0

#include "ace/Service_Config.h"

class TAO_RT_Invocation_Endpoint_Selector;

// ****************************************************************
/**
 * @class RT_Endpoint_Selector_Factory
 *
 * @brief Factory for obtaining appropriate <Invocation_Selector>.
 *
 * Used by Invocation classes to intialize its endpoint selection
 * strategy and state based on the effective policies.  Endpoint
 * selection strategies are stateless objects - all the state they
 * need is contained by Invocation.  Thus, rather than allocating an
 * endpoint selection strategy object for each Invocation, the factory
 * simply returns the appropriate one from the set preallocated in the
 * ORB_Core.  One endpoint selection strategy object can be used by
 * many invocations concurrently.
 */
class TAO_RTCORBA_Export RT_Endpoint_Selector_Factory
  : public TAO_Endpoint_Selector_Factory
{
public:
  /// Constructor.
  RT_Endpoint_Selector_Factory (void);

  /// Destructor.
  virtual ~RT_Endpoint_Selector_Factory (void);

  /// Get an Invocation's endpoint selection strategy and
  /// initialize the endpoint selection state instance.
  virtual TAO_Invocation_Endpoint_Selector *get_selector (TAO_GIOP_Invocation *invocation
                                                          ACE_ENV_ARG_DECL);

private:
  TAO_RT_Invocation_Endpoint_Selector *rt_invocation_endpoint_selector_;
};

ACE_STATIC_SVC_DECLARE_EXPORT (TAO_RTCORBA, RT_Endpoint_Selector_Factory)
ACE_FACTORY_DECLARE (TAO_RTCORBA, RT_Endpoint_Selector_Factory)

#endif /* TAO_HAS_CORBA_MESSAGING && TAO_HAS_CORBA_MESSAGING != 0 */

#include /**/ "ace/post.h"
#endif  /* RT_ENDPOINT_SELECTOR_FACTORY_H */
