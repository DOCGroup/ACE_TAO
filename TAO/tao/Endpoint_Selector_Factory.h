// This may look like C, but it's really -*- C++ -*-

//=============================================================================
/**
 *  @file   Endpoint_Selector_Factory.h
 *
 *  $Id$
 *
 * Strategies for selecting profile/endpoint from an IOR for making an
 * invocation.
 *
 *  @author Marina Spivak <marina@cs.wustl.edu>
 *  @author Frank Hunleth <fhunleth@cs.wustl.edu>
 */
//=============================================================================

#ifndef TAO_ENDPOINT_SELECTOR_FACTORY_H
#define TAO_ENDPOINT_SELECTOR_FACTORY_H
#include /**/ "ace/pre.h"

#include "tao/corbafwd.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Service_Object.h"
#include "ace/CORBA_macros.h"

class TAO_GIOP_Invocation;
class TAO_Invocation_Endpoint_Selector;

// ****************************************************************

/**
 * @class TAO_Endpoint_Selector_Factory
 *
 * @brief Factory for initializing <Endpoint_Selection_State> and
 * obtaining appropriate <Invocation_Endpoint_Selector>.
 *
 * Used by Invocation classes to intialize its endpoint selection
 * strategy and state based on the effective policies.
 * Endpoint selection strategies are stateless objects - all the
 * state they need is contained by Invocation in
 * <Endpoint_Selection_State>.  Thus, rather than allocating an
 * endpoint selection strategy object for each Invocation, the
 * factory simply returns the appropriate one from the
 * set preallocated in the ORB_Core.  One endpoint selection
 * strategy object can be used by many invocations concurrently.
 */
class TAO_Export TAO_Endpoint_Selector_Factory
  : public ACE_Service_Object
{
public:
  /// Destructor.
  virtual ~TAO_Endpoint_Selector_Factory (void);

  /// Get an Invocation's endpoint selection strategy and
  /// initialize the endpoint selection state instance.
  virtual TAO_Invocation_Endpoint_Selector *get_selector (
                             TAO_GIOP_Invocation *invocation
                             ACE_ENV_ARG_DECL) = 0;

  virtual TAO_Invocation_Endpoint_Selector *get_selector (
              ACE_ENV_SINGLE_ARG_DECL) = 0;


};

#include /**/ "ace/post.h"
#endif  /* TAO_ENDPOINT_SELECTOR_FACTORY_H */
