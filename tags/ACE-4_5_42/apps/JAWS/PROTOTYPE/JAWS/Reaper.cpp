// $Id$

#include "JAWS/Reaper.h"
#include "JAWS/Concurrency.h"

ACE_RCSID(JAWS, Reaper, "$Id$")

JAWS_Reaper::JAWS_Reaper (JAWS_Concurrency_Base *concurrency)
  : concurrency_ (concurrency),
    waiting_ (0)
{
}

JAWS_Reaper::~JAWS_Reaper (void)
{
}

int
JAWS_Reaper::open (void *)
{
  if (this->waiting_ == 0)
    {
      ACE_Guard<ACE_SYNCH_MUTEX> g (this->lock_);
      if (this->waiting_ == 0)
        {
          if (this->activate () == -1)
            ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "JAWS_Reaper::activate"),
                              -1);
          this->waiting_ = 1;
        }
    }
  return 0;
}

int
JAWS_Reaper::svc (void)
{
  return this->concurrency_->wait ();
}
