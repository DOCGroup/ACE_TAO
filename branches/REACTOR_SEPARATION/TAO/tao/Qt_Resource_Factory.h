// -*- C++ -*-

//=============================================================================
/**
 *  @file   Qt_Resource_Factory.h
 *
 *  $Id$
 *
 *  @author Hamish Friedlander <ullexco@wave.co.nz>
 *  @author Balachandran Natarajan <bala@cs.wustl.edu>
 */
//=============================================================================

#ifndef TAO_QT_RESOURCE_FACTORY_H
#define TAO_QT_RESOURCE_FACTORY_H
#include /**/ "ace/pre.h"

#include "tao/Qt_Resources_Export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/QtReactor.h"

namespace TAO
{
  /**
   * @class QtResource_Factory
   *
   * @brief TAO's default resource factory
   *
   * This factory is intended for creating QtReactor for ORB.
   */
  class TAO_QtResources_Export QtResource_Factory : public GUI_Resource_Factory
  {
  public:
    QtResource_Factory (void);

    /// Set the context used to create the QtReactor
    static void set_context (QApplication *qapp) ;

    /// Obtain the reactor implementation
    virtual ACE_Reactor_Impl *create (void) const;

  private:
    static QApplication *qapp_ ;
  };


  ACE_STATIC_SVC_DECLARE_EXPORT (TAO_QtReactor, QtResource_Factory)
    ACE_FACTORY_DECLARE (TAO_QtReactor, QtResource_Factory)
}



#include /**/ "ace/post.h"
#endif /* QTRESOURCE_H */
