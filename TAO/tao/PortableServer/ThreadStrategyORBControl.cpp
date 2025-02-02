// -*- C++ -*-
#include "tao/PortableServer/ThreadStrategyORBControl.h"
#include "ace/Log_Msg.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  namespace Portable_Server
  {
    int
    ThreadStrategyORBControl::enter ()
    {
      return 0;
    }

    int
    ThreadStrategyORBControl::exit ()
    {
      return 0;
    }
  }
}

TAO_END_VERSIONED_NAMESPACE_DECL
