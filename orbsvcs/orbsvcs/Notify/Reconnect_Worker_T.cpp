//=============================================================================
/**
*  @file    Reconnect_Worker_T.cpp
*
*  $Id$
*
*  @author Jonathan Pollack <pollack_j@ociweb.com>
*/
//=============================================================================

#ifndef RECONNECT_WORKER_CPP
#define RECONNECT_WORKER_CPP

#include "orbsvcs/Notify/Reconnect_Worker_T.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO_Notify
{
  template<class TOPOOBJ>
  Reconnect_Worker<TOPOOBJ>::Reconnect_Worker()
  {
  }

  template<class TOPOOBJ>
  void
  Reconnect_Worker<TOPOOBJ>::work (TOPOOBJ* o)
  {
    ACE_ASSERT(o != 0);
    o->reconnect ();
  }
} // namespace TAO_Notify

TAO_END_VERSIONED_NAMESPACE_DECL

#endif /* RECONNECT_WORKER_CPP */
