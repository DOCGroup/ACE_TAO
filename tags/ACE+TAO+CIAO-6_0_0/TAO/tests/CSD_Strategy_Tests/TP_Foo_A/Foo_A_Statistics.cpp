// $Id$
#include "Foo_A_Statistics.h"
#include "Foo_A_ClientEngine.h"

Foo_A_Statistics::Foo_A_Statistics(unsigned num_clients)
: num_clients_ (num_clients)
{
  for (unsigned i = 0; i < 5; i++)
    {
      this->expected_[i] = 0;
      this->actual_[i] = 0;
    }
}


Foo_A_Statistics::~Foo_A_Statistics()
{
}


void
Foo_A_Statistics::expected(unsigned op_num, unsigned count)
{
  this->expected_[op_num-1] = count * this->num_clients_;
}


void
Foo_A_Statistics::actual(unsigned op_num, unsigned count)
{
  this->actual_[op_num-1] += count;
}


void
Foo_A_Statistics::actual_in_values(unsigned op_num, LongVector lv)
{
  size_t sz = lv.size();
  for (size_t i = 0; i < sz; i++)
    {
      this->actual_in_values_[op_num-1].push_back (lv[i]);
    }
}


bool
Foo_A_Statistics::actual_vs_expected()
{
  Foo_A_Statistics stats (1);
  Foo_A_ClientEngine::expected_results (stats);

  for (unsigned i = 0; i < 5; i++)
    {
      if (this->expected_[i] != this->actual_[i])
        {
          return false;
        }

      // Verify the "in" values.
      // Skip op1/op5 since they do not have any "in" values.
      if (i % 5 == 0 || i % 5 == 4)
        {
          continue;
        }

      unsigned actual_size
        = this->actual_in_values_[i].size ();
      unsigned expected_size
        = this->num_clients_ * stats.expected_[i];
      if (actual_size != expected_size)
        {
          return false;
        }
      sort (this->actual_in_values_[i]);

      for (unsigned j = 0; j < actual_size - 1; j++)
        {
          if (this->actual_in_values_[i][j] != this->actual_in_values_[i][j + 1] - 1)
            {
              return false;
            }
        }
    }
  return true;
}



