// -*- C++ -*-

//=============================================================================
/**
 *  @file   fl_resource.h
 *
 *  $Id$
 *
 *  @author Marek Brudka <mbrudka@aster.pl>
 */
//=============================================================================

#ifndef _TAO_FLRESOURCE_H
#define _TAO_FLRESOURCE_H
#include /**/ "ace/pre.h"

#include "tao/default_resource.h"
#include "tao/TAO_FlReactor_export.h"
#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/FlReactor.h"


/**
 * @class TAO_FlResource_Factory
 *
 * @brief TAO's default resource factory for FlReactor
 *
 * Using a <{resource source specifier}> as a discriminator, the
 * factory can return resource instances which are, e.g., global,
 * stored in thread-specific storage, stored in shared memory,
 * etc.
 */
class TAO_FlReactor_Export TAO_FlResource_Factory : public TAO_Default_Resource_Factory
{

public:
  TAO_FlResource_Factory (void);

protected:
  /// Obtain the reactor implementation
  virtual ACE_Reactor_Impl *allocate_reactor_impl (void) const;
};

#if defined (__ACE_INLINE__)
#include "tao/fl_resource.i"
#endif /* __ACE_INLINE__ */

ACE_STATIC_SVC_DECLARE_EXPORT (TAO_FlReactor, TAO_FlResource_Factory)
ACE_FACTORY_DECLARE (TAO_FlReactor, TAO_FlResource_Factory)

#include /**/ "ace/post.h"
#endif /* FLRESOURCE_H */
