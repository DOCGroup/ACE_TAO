#include "Callback_i.h"

Callback_i::Callback_i() = default;


Callback_i::~Callback_i () = default;


void
Callback_i::test_method()
{
  this->num_callbacks_ ++;
}


void
Callback_i::gather_stats(Foo_B_Statistics& stats)
{
  stats.actual_callbacks(this->num_callbacks_);
}

