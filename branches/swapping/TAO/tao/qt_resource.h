// -*- C++ -*-

//=============================================================================
/**
 *  @file   qt_resource.h
 *
 *  $Id$
 *
 *  @author Hamish Friedlander <ullexco@wave.co.nz>
 *  @author Balachandran Natarajan <bala@cs.wustl.edu>
 */
//=============================================================================

#ifndef _TAO_QTRESOURCE_H
#define _TAO_QTRESOURCE_H
#include /**/ "ace/pre.h"

#include "tao/default_resource.h"
#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if defined (ACE_HAS_QT)
#include "ace/QtReactor.h"


/**
 * @class TAO_QtResource_Factory
 *
 * @brief TAO's default resource factory
 *
 * Using a <{resource source specifier}> as a discriminator, the
 * factory can return resource instances which are, e.g., global,
 * stored in thread-specific storage, stored in shared memory,
 * etc.
 */
class TAO_Export TAO_QtResource_Factory : public TAO_Default_Resource_Factory
{

public:
  TAO_QtResource_Factory (void);

  /// Set the context used to create the QtReactor
  static void set_context (QApplication *qapp) ;

protected:
  /// Obtain the reactor implementation
  virtual ACE_Reactor_Impl *allocate_reactor_impl (void) const;

private:
  static QApplication *qapp_ ;
};

#if defined (__ACE_INLINE__)
#include "tao/qt_resource.i"
#endif /* __ACE_INLINE__ */

ACE_STATIC_SVC_DECLARE_EXPORT (TAO, TAO_QtResource_Factory)
ACE_FACTORY_DECLARE (TAO, TAO_QtResource_Factory)

#endif /*ACE_HAS_QT */
#include /**/ "ace/post.h"
#endif /* QTRESOURCE_H */
