// -*- C++ -*-

//=============================================================================
/**
 *  @file   tk_resource.h
 *
 *  $Id$
 *
 *  @author Marek Brudka <mbrudka@aster.pl>
 */
//=============================================================================

#ifndef _TAO_TKRESOURCE_H
#define _TAO_TKRESOURCE_H
#include /**/ "ace/pre.h"

#include "tao/default_resource.h"
#include "tao/TAO_TkReactor_export.h"
#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

// forwards
class ACE_TkReactor;

/**
 * @class TAO_TkResource_Factory
 *
 * @brief TAO's default resource factory for TkReactor
 *
 * This factory is intended for creating TkReactor for ORB.
 */

class TAO_TkReactor_Export TAO_TkResource_Factory : public TAO_Default_Resource_Factory
{

public:
  TAO_TkResource_Factory (void);

protected:
  /// Obtain the reactor implementation
  virtual ACE_Reactor_Impl *allocate_reactor_impl (void) const;

private:
  static ACE_TkReactor *impl_;

};

#if defined (__ACE_INLINE__)
#include "tao/tk_resource.i"
#endif /* __ACE_INLINE__ */

ACE_STATIC_SVC_DECLARE_EXPORT (TAO_TkReactor, TAO_TkResource_Factory)
ACE_FACTORY_DECLARE (TAO_TkReactor, TAO_TkResource_Factory)

#include /**/ "ace/post.h"
#endif /* TKRESOURCE_H */
