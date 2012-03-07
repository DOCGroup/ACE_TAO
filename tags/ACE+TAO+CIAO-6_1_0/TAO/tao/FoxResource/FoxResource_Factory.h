// -*- C++ -*-

//=============================================================================
/**
 *  @file   FoxResource_Factory.h
 *
 *  $Id$
 *
 *  @author Balachandran Natarajan <bala@cs.wustl.edu>
 *  @author Marek Brudka <mbrudka@aster.pl>
 */
//=============================================================================
#ifndef TAO_TOXRESOURCE_FACTORY_H
#define TAO_TOXRESOURCE_FACTORY_H
#include /**/ "ace/pre.h"
#include "ace/FoxReactor/FoxReactor.h"
#include "tao/FoxResource/TAO_FoxResource_Export.h"
#include "tao/GUIResource_Factory.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{

  /**
   * @class FoxResource_Factory
   *
   * @brief TAO_GUI_Resource_Factory for creating FoxReactor.
   *
   * This factory is intended for creating FoxReactor for ORB. This
   * factory can be feed into ORB using
   * TAO_ORB_Core::set_gui_resource_factory method which is usually
   * done by TAO_FoxResource_Loader.
   */

  class TAO_FoxResource_Export FoxResource_Factory : public GUIResource_Factory
  {
  public:

    FoxResource_Factory (FXApp *app);

  protected:

    /// Create or obtain current reactor implementation
    virtual ACE_Reactor_Impl *reactor_impl (void);

  private:

    /// Reactor created by this factory.
    ACE_FoxReactor *reactor_impl_;

    FXApp *fxapp_;
  };
}

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"
#endif /* TAO_TOXRESOURCE_FACTORY_H */
