/* -*- C++ -*- */

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

#include "Reconnect_Worker_T.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

namespace TAO_Notify
{
  template<class TOPOOBJ>
  Reconnect_Worker<TOPOOBJ>::Reconnect_Worker()
  {
  }

  template<class TOPOOBJ>
  void
  Reconnect_Worker<TOPOOBJ>::work (TOPOOBJ* o ACE_ENV_ARG_DECL)
  {
    ACE_ASSERT(o != 0);
    o->reconnect (ACE_ENV_SINGLE_ARG_PARAMETER);
    ACE_CHECK;
  }
} // namespace TAO_Notify

#endif /* RECONNECT_WORKER_CPP */
