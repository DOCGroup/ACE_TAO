
// $Id$

#include "mld.h"

ACE_Atomic_Op<ACE_Mutex,int> mld_counter::counter_;

// Passthrough to the static atomic op
int mld_counter::value(void)
{
    return counter_.value();
}

// Increment the counter when a new mld is created...
mld::mld(void)
{
    ++MLD_COUNTER->counter_;
}

// and decrement it when the object is destructed.
mld::~mld(void)
{
    --MLD_COUNTER->counter_;
}
