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

#ifndef _TAO_FLRESOURCE_LOADER_H
#define _TAO_FLRESOURCE_LOADER_H
#include /**/ "ace/pre.h"
#include "TAO_FlResource_Export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

namespace TAO
{
  /** Loads TAO resources related with Fl.
   *
   * This class changes the default reactor implementation into ACE_FlReactor by
   * calling TAO_ORB_Core::set_gui_resource_factory. Please notice, this class has
   * to be created in the main Fl thread, because set_gui_resource_factory creates
   * a variable in TSS. This way FlReactor is instantiated only in Fl event loop thread.
   *
   */
  class TAO_FlResource_Export FlResource_Loader
  {

  public:
    FlResource_Loader ( );
    virtual ~FlResource_Loader ();

  };
}
#include /**/ "ace/post.h"
#endif /* _TAO_FLRESOURCE_LOADER_H */
