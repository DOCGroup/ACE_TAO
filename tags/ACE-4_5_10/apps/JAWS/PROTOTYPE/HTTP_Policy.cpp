// $Id$

#include "HTTP_Policy.h"

HTTP_Policy::HTTP_Policy (JAWS_Concurrency *concurrency)
  : concurrency_ (concurrency)
{
}

JAWS_Concurrency_Base *
HTTP_Policy update (void *)
{
  /* for now, we always return the same concurrency strategy */
  returh this->concurrency_;
}
