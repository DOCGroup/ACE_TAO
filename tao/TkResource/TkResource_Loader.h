// -*- C++ -*-

//=============================================================================
/**
 *  @file   TkResource_Loader.h
 *
 *  $Id$
 *
 *  @author Balachandran Natarajan <bala@cs.wustl.edu>
 *  @author Marek Brudka <mbrudka@aster.pl>
 */
//=============================================================================

#ifndef TAO_TKRESOURCE_LOADER_H
#define TAO_TKRESOURCE_LOADER_H

#include /**/ "ace/pre.h"

#include "tao/TkResource/TAO_TkResource_Export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/Versioned_Namespace.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  /**
   * @class TkResource_Loader
   *
   * @brief Loads TAO resources related with Tk.
   *
   * This class changes the default reactor implementation into
   * ACE_TkReactor one by calling TAO_ORB_Core::set_gui_resource_factory.
   * User should create an instance of this class before ORB_init
   * when the TAO server has has to be integrated within Tk event loop.
   *
   * Please notice, this class has to be created in the main Tk thread,
   * because set_gui_resource_factory creates a variable in TSS. This way
   * TkReactor is instantiated only in Tk event loop thread.
   */
  class TAO_TkResource_Export TkResource_Loader
  {
  public:

    TkResource_Loader (void);

    virtual ~TkResource_Loader (void);

  };
}

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#endif /* TAO_TKRESOURCE_LOADER_H */
