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

#ifndef TAO_XTRESOURCE_LOADER_H
#define TAO_XTRESOURCE_LOADER_H
#include /**/ "ace/pre.h"
#include /**/ <X11/Intrinsic.h>
#include "tao/XtResource/TAO_XtResource_Export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/Versioned_Namespace.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  /**
   * @class XtResource_Loader
   *
   * @brief Loads TAO resources related with Xt.
   *
   * This class changes the default reactor implementation into
   * ACE_XtReactor one by calling TAO_ORB_Core::set_gui_resource_factory.
   * User should create an instance of this class before ORB_init
   * when the TAO server has has to be integrated within Xt event loop.
   *
   * Please notice, this class has to be created in the main Xt thread,
   * because set_gui_resource_factory creates a variable in TSS. This way
   * XtReactor is instantiated only in Xt event loop thread.
   */
  class TAO_XtResource_Export XtResource_Loader
  {

  public:
    XtResource_Loader  (XtAppContext context);

    virtual ~XtResource_Loader ();
  };
}

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"
#endif /* TAO_XTRESOURCE_LOADER_H */
