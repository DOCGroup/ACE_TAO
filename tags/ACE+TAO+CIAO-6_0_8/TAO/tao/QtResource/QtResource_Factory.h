// -*- C++ -*-

//=============================================================================
/**
 *  @file   QtResource_Factory.h
 *
 *  $Id$
 *
 *  @author Balachandran Natarajan <bala@dre.vanderbilt.edu>
 *  @author Marek Brudka <mbrudka@aster.pl>
 */
//=============================================================================
#ifndef TAO_QTRESOURCE_FACTORY_H
#define TAO_QTRESOURCE_FACTORY_H
#include /**/ "ace/pre.h"

#include "tao/QtResource/TAO_QtResource_Export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/QtReactor/QtReactor.h"
#include "tao/GUIResource_Factory.h"

class QApplication;

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  /**
   * @class QtResource_Factory
   *
   * @brief TAO_GUI_Resource_Factory for creating QtReactor.
   *
   * This factory is intended for creating QtReactor for ORB. This
   * factory can be feed into ORB using
   * TAO_ORB_Core::set_gui_resource_factory method which is usually
   * done by TAO_QtResource_Loader.
   */
  class TAO_QtResource_Export QtResource_Factory : public GUIResource_Factory
  {
  public:

    QtResource_Factory (QApplication *qapp_);

  protected:

    /// Create or obtain current reactor implementation
    virtual ACE_Reactor_Impl *reactor_impl (void);

  private:

    /// Reactor created by this factory.
    ACE_QtReactor *reactor_impl_;

    /// QApplication running reactor
    QApplication *qapp_;
  };
}

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"
#endif /* TAO_QTRESOURCE_FACTORY_H */
