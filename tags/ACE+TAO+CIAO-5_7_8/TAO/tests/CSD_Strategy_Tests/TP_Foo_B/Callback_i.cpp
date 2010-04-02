// $Id$
#include "Callback_i.h"

Callback_i::Callback_i()
  : num_callbacks_(0)
{
}


Callback_i::~Callback_i ()
{
}


void
Callback_i::test_method(void)
{
  this->num_callbacks_ ++;
}


void
Callback_i::gather_stats(Foo_B_Statistics& stats)
{
  stats.actual_callbacks(this->num_callbacks_);
}

