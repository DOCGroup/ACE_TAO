// -*- C++ -*-
// $Id$

// ============================================================================
//
// = LIBRARY
//   TAO
//
// = FILENAME
//   xt_resource.h
//
// = AUTHOR
//   Carlos O'Ryan
//
// ============================================================================

#ifndef TAO_XT_RESOURCE_H
#include "ace/pre.h"
#define TAO_XT_RESOURCE_H

#include "tao/default_resource.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if defined (ACE_HAS_XT)

#include "ace/XtReactor.h"

// ****************************************************************

class TAO_XT_Resource_Factory : public TAO_Default_Resource_Factory
{
  // = TITLE
  //   TAO's default resource factory
  //
  // = DESCRIPTION
  //   Using a <{resource source specifier}> as a discriminator, the
  //   factory can return resource instances which are, e.g., global,
  //   stored in thread-specific storage, stored in shared memory,
  //   etc.
  //
public:
  // = Initialization and termination methods.
  TAO_XT_Resource_Factory (void);
  // Constructor.

  static void set_context (XtAppContext context);
  // Set the context used to create the XtReactor

protected:
  virtual ACE_Reactor_Impl *allocate_reactor_impl (void) const;
  // Obtain the reactor implementation

private:
  static XtAppContext context_;
};

#if defined (__ACE_INLINE__)
#include "tao/xt_resource.i"
#endif /* __ACE_INLINE__ */

ACE_STATIC_SVC_DECLARE (TAO_XT_Resource_Factory)
ACE_FACTORY_DECLARE (TAO, TAO_XT_Resource_Factory)

#endif /* ACE_HAS_XT */

#include "ace/post.h"
#endif /* TAO_DEFAULT_CLIENT_H */
