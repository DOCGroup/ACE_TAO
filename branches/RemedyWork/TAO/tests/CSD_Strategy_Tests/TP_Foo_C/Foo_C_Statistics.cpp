// $Id$
#include "Foo_C_Statistics.h"
#include "Foo_C_ClientEngine.h"
#include "Foo_C_Custom_ClientEngine.h"

Foo_C_Statistics::Foo_C_Statistics(unsigned num_remote_clients,
                                   unsigned num_collocated_clients)
: num_remote_clients_ (num_remote_clients),
  num_collocated_clients_ (num_collocated_clients)
{
  for (unsigned i = 0; i < 10; i++)
    {
      this->expected_[i] = 0;
      this->actual_[i] = 0;
    }
}


Foo_C_Statistics::~Foo_C_Statistics()
{
}


void
Foo_C_Statistics::expected(unsigned op_num, unsigned count, bool remote_client)
{
  if (remote_client)
    {
      this->expected_[op_num-1] += count * this->num_remote_clients_;
    }
  else
    {
      this->expected_[op_num-1] += count * this->num_collocated_clients_;
    }
}


void
Foo_C_Statistics::actual(unsigned op_num, unsigned count)
{
  this->actual_[op_num-1] += count;
}


void
Foo_C_Statistics::actual_in_values(unsigned op_num, LongVector lv)
{
  size_t sz = lv.size();
  for (size_t i = 0; i < sz; i++)
    {
      this->actual_in_values_[op_num-1].push_back (lv[i]);
    }
}


bool
Foo_C_Statistics::actual_vs_expected()
{
  // Get the expected statistics for a single remote client.
  Foo_C_Statistics remote_client_stats (1, 0);
  Foo_C_ClientEngine::expected_results (remote_client_stats);
  // Get the expected statistics for a single collocated client.
  Foo_C_Statistics collocated_client_stats (0, 1);
  Foo_C_Custom_ClientEngine::expected_results (collocated_client_stats);

  for (unsigned i = 0; i < 10; i++)
    {
      if (this->expected_[i] != this->actual_[i])
        {
          return false;
        }

      // Verify the "in" values.
      // Skip op1/op5 and cust_op1/cust_op5 since they do not have
      // any "in" values.
      if (i % 5 == 0 || i % 5 == 4)
        {
          continue;
        }

      unsigned actual_size
        = this->actual_in_values_[i].size ();
      unsigned expected_size
        =  this->num_remote_clients_ * remote_client_stats.expected_[i]
           + this->num_collocated_clients_ * collocated_client_stats.expected_[i];

      if (actual_size == 0 && expected_size == 0)
        {
          continue;
        }

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



