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

#ifndef _TAO_QTRESOURCE_LOADER_H
#define _TAO_QTRESOURCE_LOADER_H
#include /**/ "ace/pre.h"
#include /**/ <qapplication.h>
#include /**/ "TAO_QtResource_Export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

namespace TAO
{
  /** Loads TAO resources related with Qt.
   *
   * This class changes the default reactor implementation into ACE_QtReactor by
   * calling TAO_ORB_Core::set_gui_resource_factory. Please notice, this class has
   * to be created in the main Qt thread, because set_gui_resource_factory creates
   * a variable in TSS. This way QtReactor is instantiated only in Qt event loop thread.
   *
   */
  class TAO_QtResource_Export QtResource_Loader
  {

  public:
    QtResource_Loader  (QApplication *qapp);
    virtual ~QtResource_Loader ();
  };
}
#include /**/ "ace/post.h"
#endif /* _TAO_QTRESOURCE_LOADER_H */
