#ifndef RACE_RT_H
#define RACE_RT_H

#include "ace/Sched_Params.h"
#include "ace/OS_NS_errno.h"
#include "ace/Log_Msg.h"

namespace CIAO
{
  namespace RACE
  {
      int set_priority ();
  }
}

#endif /* RACE_RT_H */
