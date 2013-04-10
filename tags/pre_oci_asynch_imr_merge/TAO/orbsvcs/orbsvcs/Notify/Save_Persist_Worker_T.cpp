//=============================================================================
/**
*  @file    Save_Persist_Worker_T.cpp
*
*  $Id$
*
*  @author Jonathan Pollack <pollack_j@ociweb.com>
*/
//=============================================================================

#ifndef SAVE_PERSIST_WORKER_CPP
#define SAVE_PERSIST_WORKER_CPP

#include "orbsvcs/ESF/ESF_Worker.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO_Notify
{
  template<class TOPOOBJ>
  Save_Persist_Worker<TOPOOBJ>::Save_Persist_Worker(Topology_Saver& saver,
                                                    bool want_all_children)
    : saver_ (saver)
    , want_all_children_ (want_all_children)
  {
  }

  template<class TOPOOBJ>
  void
  Save_Persist_Worker<TOPOOBJ>::work (TOPOOBJ* o)
  {
    ACE_ASSERT(o != 0);
    if (this->want_all_children_ || o->is_changed ())
    {
      o->save_persistent (saver_);
    }
  }
} // namespace TAO_Notify

TAO_END_VERSIONED_NAMESPACE_DECL

#endif /* SAVE_PERSIST_WORKER_CPP */
