// -*- C++ -*-

//=============================================================================
/**
 *  @file   TkResource_Factory.h
 *
 *  $Id$
 *
 *  @author Balachandran Natarajan <bala@cs.wustl.edu>
 *  @author Marek Brudka <mbrudka@aster.pl>
 */
//=============================================================================
#ifndef _TAO_TKRESOURCE_FACTORY_H
#define _TAO_TKRESOURCE_FACTORY_H
#include /**/ "ace/pre.h"
#include /**/ "ace/TkReactor.h"
#include /**/ "TAO_TkResource_Export.h"
#include /**/ "GUIResource_Factory.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

namespace TAO
{

  /** TAO_GUI_Resource_Factory for creating TkReactor.
   *
   * This factory is intended for creating TkReactor for ORB. This factory can be
   * feed into ORB using TAO_ORB_Core::set_gui_resource_factory method which is
   * usually done by TAO_TkResource_Loader.
   */

  class TAO_TkResource_Export TkResource_Factory : public GUIResource_Factory
  {
  public:
    TkResource_Factory ();

    /// Create or obtain current reactor implementation
    virtual ACE_Reactor_Impl *reactor_impl (void);

  private:

    /// Reactor created by this factory.
    ACE_TkReactor *reactor_impl_;
  };
}
#include /**/ "ace/post.h"
#endif /* _TAO_TKRESOURCE_FACTORY_H */
