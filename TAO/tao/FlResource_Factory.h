// -*- C++ -*-

//=============================================================================
/**
 *  @file   FlResource_Factory.h
 *
 *  $Id$
 *
 *  @author Balachandran Natarajan <bala@cs.wustl.edu>
 *  @author Marek Brudka <mbrudka@aster.pl>
 */
//=============================================================================
#ifndef _TAO_FLRESOURCE_FACTORY_H
#define _TAO_FLRESOURCE_FACTORY_H
#include /**/ "ace/pre.h"
#include /**/ "ace/FlReactor.h"
#include /**/ "TAO_FlResource_Export.h"
#include /**/ "GUIResource_Factory.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

namespace TAO
{

  /** TAO_GUI_Resource_Factory for creating FlReactor.
   *
   * This factory is intended for creating FlReactor for ORB. This factory can be
   * feed into ORB using TAO_ORB_Core::set_gui_resource_factory method which is
   * usually done by TAO_FlResource_Loader.
   */
  class TAO_FlResource_Export FlResource_Factory : public GUIResource_Factory
  {
  public:
    FlResource_Factory ();

    /// Create or obtain current reactor implementation
    virtual ACE_Reactor_Impl *reactor_impl (void);

  private:

    /// Reactor created by this factory.
    ACE_FlReactor *reactor_impl_;
  };
}
#include /**/ "ace/post.h"
#endif /* _TAO_FLRESOURCE_FACTORY_H */
