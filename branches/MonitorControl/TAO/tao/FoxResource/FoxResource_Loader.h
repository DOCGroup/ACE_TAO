// -*- C++ -*-

//=============================================================================
/**
 *  @file   FoxResource_Loader.h
 *
 *  $Id$
 *
 *  @author Balachandran Natarajan <bala@cs.wustl.edu>
 *  @author Marek Brudka <mbrudka@aster.pl>
 */
//=============================================================================

#ifndef TAO_TOXRESOURCE_LOADER_H
#define TAO_TOXRESOURCE_LOADER_H

#include /**/ "ace/pre.h"

#include "tao/FoxResource/TAO_FoxResource_Export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/Versioned_Namespace.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  /**
   * @class FoxResource_Loader
   *
   * @brief Loads TAO resources related with Fox.
   *
   * This class changes the default reactor implementation into
   * ACE_FoxReactor one by calling TAO_ORB_Core::set_gui_resource_factory.
   * User should create an instance of this class before ORB_init
   * when the TAO server has has to be integrated within Fox event loop.
   *
   * Please notice, this class has to be created in the main Fox thread,
   * because set_gui_resource_factory creates a variable in TSS. This way
   * FoxReactor is instantiated only in Fox event loop thread.
   */
  class TAO_FoxResource_Export FoxResource_Loader
  {
  public:

    FoxResource_Loader (FXApp *app);

    virtual ~FoxResource_Loader (void);

  };
}

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#endif /* TAO_TOXRESOURCE_LOADER_H */
