// $Id$

#include "JAWS/Policy.h"

JAWS_Concurrency_Base *
JAWS_Synch_Dispatch_Policy::concurrency (void)
{
  return 0;
}

JAWS_IO *
JAWS_Synch_Dispatch_Policy::io (void)
{
  return &(this->io_);
}
