// -*- C++ -*-

//=============================================================================
/**
 *  @file   FlResource_Loader.h
 *
 *  $Id$
 *
 *  @author Balachandran Natarajan <bala@cs.wustl.edu>
 *  @author Marek Brudka <mbrudka@aster.pl>
 */
//=============================================================================

#ifndef TAO_FLRESOURCE_LOADER_H
#define TAO_FLRESOURCE_LOADER_H
#include /**/ "ace/pre.h"

#include "tao/FlResource/TAO_FlResource_Export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/Versioned_Namespace.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{

  /**
   * @class FlResource_Loader
   *
   * @brief Loads TAO resources related with Fl.
   *
   * This class changes the default reactor implementation into
   * ACE_FlReactor one by calling TAO_ORB_Core::set_gui_resource_factory.
   * User should create an instance of this class before ORB_init
   * when the TAO server has has to be integrated within Fl event loop.
   *
   * Please notice, this class has to be created in the main Fl thread,
   * because set_gui_resource_factory creates a variable in TSS. This way
   * FlReactor is instantiated only in Qt event loop thread.
   */
  class TAO_FlResource_Export FlResource_Loader
  {

  public:
    FlResource_Loader (void);

    virtual ~FlResource_Loader (void);
  };
}

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#endif /* TAO_FLRESOURCE_LOADER_H */
