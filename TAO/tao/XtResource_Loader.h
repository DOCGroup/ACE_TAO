// -*- C++ -*-

//=============================================================================
/**
 *  @file   XtResource_Loader.h
 *
 *  $Id$
 *
 *  @author Marek Brudka <mbrudka@aster.pl>
 *  @author Balachandran Natarajan <bala@cs.wustl.edu>
 */
//=============================================================================

#ifndef _TAO_XTRESOURCE_LOADER_H
#define _TAO_XTRESOURCE_LOADER_H
#include /**/ "ace/pre.h"
#include /**/ "TAO_XtResource_Export.h"
#include /**/ <X11/Intrinsic.h>

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

namespace TAO
{
  /** Loads TAO resources related with Xt.
   *
   * This class changes the default reactor implementation into ACE_XtReactor by
   * calling TAO_ORB_Core::set_gui_resource_factory. Please notice, this class has
   * to be created in the main Xt thread, because set_gui_resource_factory creates
   * a variable in TSS. This way XtReactor is instantiated only in Xt event loop thread.
   *
   */
  class TAO_XtResource_Export XtResource_Loader
  {

  public:
    XtResource_Loader  (XtAppContext context);
    virtual ~XtResource_Loader ();

  };
}
#include /**/ "ace/post.h"
#endif /* _TAO_XTRESOURCE_LOADER_H */
