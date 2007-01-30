// -*- C++ -*-

//=============================================================================
/**
 *  @file   FT_Endpoint_Selector_Factory.h
 *
 *  $Id$
 *
 *  @author Balachandran Natarajan <bala@cs.wustl.edu>
 */
//=============================================================================
#ifndef TAO_FT_ENDPOINT_SELECTOR_FACTORY_H
#define TAO_FT_ENDPOINT_SELECTOR_FACTORY_H

#include /**/ "ace/pre.h"

#include "orbsvcs/FaultTolerance/FT_ClientORB_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/Endpoint_Selector_Factory.h"
#include "tao/Basic_Types.h"
#include "tao/orbconf.h"

#include "ace/Service_Config.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_FT_Invocation_Endpoint_Selector;
/**
 * @class TAO_FT_Endpoint_Selector_Factory
 *
 * @brief Factory for initializing <Endpoint_Selection_State> and
 * obtaining appropriate <Invocation_Endpoint_Selector>.
 *
 * This class is specifically for the FT_Service in TAO to return the
 * right factory
 */

class TAO_FT_ClientORB_Export TAO_FT_Endpoint_Selector_Factory
  : public TAO_Endpoint_Selector_Factory
{
public:

  /// Constructor.
  TAO_FT_Endpoint_Selector_Factory (void);

  /// Destructor.
  virtual ~TAO_FT_Endpoint_Selector_Factory (void);

  /// Get an Invocation's endpoint selection strategy and
  /// initialize the endpoint selection state instance.
  virtual TAO_Invocation_Endpoint_Selector *get_selector ();

private:

  /// The possible endpoint selector strategies that can be
  /// returned by this factory
  TAO_FT_Invocation_Endpoint_Selector *ft_endpoint_selector_;

  /// Mutex to prevent multiple initializations
  TAO_SYNCH_MUTEX mutex_;
};

TAO_END_VERSIONED_NAMESPACE_DECL

//extern TAO_FT_ClientORB_Export ACE_Static_Svc_Descriptor ace_svc_desc_TAO_FT_Endpoint_Selector_Factory;
ACE_STATIC_SVC_DECLARE_EXPORT (TAO_FT_ClientORB, TAO_FT_Endpoint_Selector_Factory)
ACE_FACTORY_DECLARE (TAO_FT_ClientORB, TAO_FT_Endpoint_Selector_Factory)

#include /**/ "ace/post.h"

#endif  /* TAO_DEFAULT_ENDPOINT_SELECTOR_FACTORY_H */
