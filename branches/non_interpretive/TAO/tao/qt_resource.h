// -*- C++ -*-
// $Id$

// ============================================================================
//
// = LIBRARY
//   TAO
//
// = FILENAME
//   qt_resource.h
//
// = AUTHOR
//    Hamish Friedlander <ullexco@wave.co.nz>
//    integrated in to TAO by Balachandran Natarajan <bala@cs.wustl.edu>
// ============================================================================
#ifndef _TAO_QTRESOURCE_H
#define _TAO_QTRESOURCE_H

#include "tao/default_resource.h"
#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if defined (ACE_HAS_QT)
#include "ace/QtReactor.h"


class TAO_Export TAO_QtResource_Factory : public TAO_Default_Resource_Factory
{
  // = TITLE
  //   TAO's default resource factory
  //
  // = DESCRIPTION
  //   Using a <{resource source specifier}> as a discriminator, the
  //   factory can return resource instances which are, e.g., global,
  //   stored in thread-specific storage, stored in shared memory,
  //   etc.

public:
  TAO_QtResource_Factory (void);

  static void set_context (QApplication *qapp) ;
  // Set the context used to create the QtReactor

protected:
  virtual ACE_Reactor_Impl *allocate_reactor_impl (void) const;
  // Obtain the reactor implementation

private:
  static QApplication *qapp_ ;
};

#if defined (__ACE_INLINE__)
#include "tao/qt_resource.i"
#endif /* __ACE_INLINE__ */

ACE_STATIC_SVC_DECLARE( TAO_QtResource_Factory )
ACE_FACTORY_DECLARE   ( TAO, TAO_QtResource_Factory )

#endif /*ACE_HAS_QT */
#endif /* QTRESOURCE_H */
