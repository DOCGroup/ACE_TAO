// -*- C++ -*-

//=============================================================================
/**
 *  @file   QtResource_Loader.h
 *
 *  $Id$
 *
 *  @author Marek Brudka <mbrudka@aster.pl>
 *  @author Balachandran Natarajan <bala@cs.wustl.edu>
 */
//=============================================================================

#ifndef TAO_QTRESOURCE_LOADER_H
#define TAO_QTRESOURCE_LOADER_H
#include /**/ "ace/pre.h"

#include "tao/QtResource/TAO_QtResource_Export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/Versioned_Namespace.h"

#if defined (ACE_HAS_QT4)
#include <QtGui/qapplication.h>
#else
#include <qapplication.h>
#endif

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  /**
   * @class QtResource_Loader
   *
   * @brief Loads TAO resources related with Qt.
   *
   * This class changes the default reactor implementation into
   * ACE_QtReactor one by calling TAO_ORB_Core::set_gui_resource_factory.
   * User should create an instance of this class before ORB_init
   * when the TAO server has has to be integrated within Qt event loop.
   *
   * Please notice, this class has to be created in the main Qt thread,
   * because set_gui_resource_factory creates a variable in TSS. This way
   * QtReactor is instantiated only in Qt event loop thread.
   */

  class TAO_QtResource_Export QtResource_Loader
  {
  public:
    QtResource_Loader (QApplication *qapp);
    virtual ~QtResource_Loader (void);
  };
}

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"
#endif /* TAO_QTRESOURCE_LOADER_H */
