// -*- C++ -*-

//=============================================================================
/**
 *  @file   xt_resource.h
 *
 *  $Id$
 *
 *  @author Carlos O'Ryan
 */
//=============================================================================


#ifndef TAO_XT_RESOURCE_H
#define TAO_XT_RESOURCE_H
#include /**/ "ace/pre.h"

#include "tao/default_resource.h"
#include "tao/TAO_XtReactor_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/XtReactor.h"
//forwards
class ACE_XtReactor;

// ****************************************************************

/**
 * @class TAO_XT_Resource_Factory
 *
 * @brief TAO's default resource factory
 *
 * This factory is intended for creating XtReactor for ORB.
 */
class TAO_XtReactor_Export TAO_XtResource_Factory : public TAO_Default_Resource_Factory
{
public:
  // = Initialization and termination methods.
  /// Constructor.
  TAO_XtResource_Factory (void);

  /// Set the context used to create the XtReactor
  static void set_context (XtAppContext context);

protected:
  /// Obtain the reactor implementation
  virtual ACE_Reactor_Impl *allocate_reactor_impl (void) const;

private:
  static XtAppContext context_;
  static ACE_XtReactor *impl_;
};

#if defined (__ACE_INLINE__)
#include "tao/xt_resource.i"
#endif /* __ACE_INLINE__ */

ACE_STATIC_SVC_DECLARE_EXPORT (TAO_XtReactor, TAO_XtResource_Factory)
ACE_FACTORY_DECLARE (TAO_XtReactor, TAO_XtResource_Factory)


#include /**/ "ace/post.h"
#endif /* TAO_DEFAULT_CLIENT_H */
