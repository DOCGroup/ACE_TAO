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
#ifndef TAO_FLRESOURCE_FACTORY_H
#define TAO_FLRESOURCE_FACTORY_H
#include /**/ "ace/pre.h"

#include "tao/FlResource/TAO_FlResource_Export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/FlReactor/FlReactor.h"
#include "tao/GUIResource_Factory.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  /**
   * @class FlResource_Factory
   *
   * @brief TAO_GUI_Resource_Factory for creating FlReactor.
   *
   * This factory is intended for creating FlReactor for ORB. This
   * factory can be feed into ORB using
   * TAO_ORB_Core::set_gui_resource_factory method which is usually
   * done by TAO_FlResource_Loader.
   */

  class TAO_FlResource_Export FlResource_Factory : public GUIResource_Factory
  {
  public:

    FlResource_Factory ();

  protected:

    /// Create or obtain current reactor implementation
    virtual ACE_Reactor_Impl *reactor_impl (void);

  private:

    /// Reactor created by this factory.
    ACE_FlReactor *reactor_impl_;
  };
}

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"
#endif /* TAO_FLRESOURCE_FACTORY_H */
