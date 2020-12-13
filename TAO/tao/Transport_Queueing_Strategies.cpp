// -*- C++ -*-
#include "Transport_Queueing_Strategies.h"
#include "Stub.h"
#include "debug.h"

#include "ace/Log_Msg.h"
#include "ace/OS_NS_sys_time.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  Transport_Queueing_Strategy::~Transport_Queueing_Strategy ()
  {
  }

// ****************************************************************

  bool
  Flush_Transport_Queueing_Strategy::must_queue (bool) const
  {
    return false;
  }

  bool
  Flush_Transport_Queueing_Strategy::buffering_constraints_reached (
    TAO_Stub *,
    size_t ,
    size_t ,
    bool &must_flush,
    const ACE_Time_Value &,
    bool &set_timer,
    ACE_Time_Value &) const
  {
    set_timer = false;
    must_flush = true;
    return true;
  }
}

TAO_END_VERSIONED_NAMESPACE_DECL
