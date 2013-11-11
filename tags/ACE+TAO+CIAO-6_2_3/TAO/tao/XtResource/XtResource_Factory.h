// -*- C++ -*-

//=============================================================================
/**
 *  @file   XtResource_Factory.h
 *
 *  $Id$
 *
 *  @author Balachandran Natarajan <bala@cs.wustl.edu>
 *  @author Marek Brudka <mbrudka@aster.pl>
 */
//=============================================================================
#ifndef TAO_XTRESOURCE_FACTORY_H
#define TAO_XTRESOURCE_FACTORY_H
#include /**/ "ace/pre.h"
#include /**/ <X11/Intrinsic.h>
#include "tao/XtResource/TAO_XtResource_Export.h"
#include "ace/XtReactor/XtReactor.h"
#include "tao/GUIResource_Factory.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{

  /**
   * @class XtResource_Factory
   *
   * @brief TAO_GUI_Resource_Factory for creating XtReactor.
   *
   * This factory is intended for creating XtReactor for ORB. This
   * factory can be feed into ORB using
   * TAO_ORB_Core::set_gui_resource_factory method which is usually
   * done by TAO_XtResource_Loader.
   */

  class TAO_XtResource_Export XtResource_Factory : public GUIResource_Factory
  {
  public:
    XtResource_Factory (XtAppContext context);

    /// Create or obtain current reactor implementation
    virtual ACE_Reactor_Impl *reactor_impl (void);

  private:

    /// Reactor created by this factory.
    ACE_XtReactor *reactor_impl_;

    /// Xt context for XtReactor
    XtAppContext  context_;

    /// for internal locking
    TAO_SYNCH_MUTEX lock_;
  };
}

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"
#endif /* TAO_XTRESOURCE_FACTORY_H */
