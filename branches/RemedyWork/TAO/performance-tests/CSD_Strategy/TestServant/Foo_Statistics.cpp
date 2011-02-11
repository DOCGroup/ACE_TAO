// $Id$
#include "Foo_Statistics.h"
#include "Foo_ClientEngine.h"


Foo_Statistics::Foo_Statistics()
  : num_clients_(0),
    num_loops_(0)
{
  for (unsigned i = 0; i < 5; i++)
    {
      this->expected_[i] = 0;
      this->actual_[i] = 0;
    }
}


Foo_Statistics::~Foo_Statistics()
{
}


void
Foo_Statistics::init(unsigned num_clients, unsigned num_loops)
{
  this->num_clients_ = num_clients;
  this->num_loops_   = num_loops;
}


void
Foo_Statistics::expected(unsigned op_num, unsigned count)
{
  this->expected_[op_num-1] = count * this->num_clients_ * this->num_loops_;
}


void
Foo_Statistics::actual(unsigned op_num, unsigned count)
{
  this->actual_[op_num-1] += count;
}


bool
Foo_Statistics::actual_vs_expected()
{
  for (unsigned i = 0; i < 5; i++)
    {
      if (this->expected_[i] != this->actual_[i])
        {
          ACE_DEBUG((LM_DEBUG,
                     "Actual vs. Expected Results Failure: "
                     "op%d() expected: %d, got: %d\n",
                     i, this->expected_[i], this->actual_[i]));
          return false;
        }
    }

  return true;
}


unsigned
Foo_Statistics::total() const
{
  unsigned t = 0;

  for (unsigned i = 0 ; i < 5; i++)
    {
      t += this->expected_[i];
    }

  return t;
}
