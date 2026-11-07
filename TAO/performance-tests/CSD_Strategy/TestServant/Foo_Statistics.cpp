#include "Foo_Statistics.h"
#include "Foo_ClientEngine.h"

Foo_Statistics::Foo_Statistics () = default;

Foo_Statistics::~Foo_Statistics () = default;

void
Foo_Statistics::init (unsigned num_clients, unsigned num_loops)
{
  this->num_clients_ = num_clients;
  this->num_loops_   = num_loops;
}

void
Foo_Statistics::expected (unsigned op_num, unsigned count)
{
  this->expected_[op_num - 1] = count * this->num_clients_ * this->num_loops_;
}

void
Foo_Statistics::actual (unsigned op_num, unsigned count)
{
  this->actual_[op_num - 1] += count;
}

bool
Foo_Statistics::actual_vs_expected () const
{
  for (unsigned i = 0; i < this->expected_.size (); i++)
    {
      if (this->expected_[i] != this->actual_[i])
        {
          ACE_DEBUG ((LM_DEBUG,
                     "Actual vs. Expected Results Failure: "
                     "op%u() expected: %u, got: %u\n",
                     i + 1, this->expected_[i], this->actual_[i]));
          return false;
        }
    }

  return true;
}

unsigned
Foo_Statistics::total () const
{
  unsigned total {};

  for (unsigned i = 0; i < this->expected_.size (); i++)
    {
      total += this->expected_[i];
    }

  return total;
}
