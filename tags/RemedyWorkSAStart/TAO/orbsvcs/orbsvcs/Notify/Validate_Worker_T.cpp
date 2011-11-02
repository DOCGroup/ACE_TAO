/* -*- C++ -*- $Id$ */

#ifndef NOTIFY_VALIDATE_WORKER_CPP
#define NOTIFY_VALIDATE_WORKER_CPP

#include "Validate_Worker_T.h"
#include "tao/debug.h"
#include "ace/Log_Msg.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO_Notify
{
  template<class TOPOOBJ>
  Validate_Worker<TOPOOBJ>::Validate_Worker()
  {
  }

  template<class TOPOOBJ>
  void
  Validate_Worker<TOPOOBJ>::work (TOPOOBJ* o)
  {
    if (o == 0)
    {
      if (TAO_debug_level > 0)
      {
        ACE_DEBUG ((LM_DEBUG,
          ACE_TEXT("(%P|%t)Validate_Worker<TOPOOBJ>::work: obj is nil\n")));
      }
    }
    else
    {
      o->validate ();
    }
  }
} // namespace TAO_Notify

TAO_END_VERSIONED_NAMESPACE_DECL

#endif /* VALIDATE_WORKER_CPP */


