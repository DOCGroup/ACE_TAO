// $Id$
#include "Foo_B_Statistics.h"
#include "Foo_B_ClientEngine.h"

extern const char* ONEWAY_ARG_TEST_STR;
extern CORBA::Boolean special_value;

Foo_B_Statistics::Foo_B_Statistics(unsigned num_remote_clients,
                                   unsigned num_collcated_clients)
: num_clients_ (num_remote_clients + num_collcated_clients),
  num_collcated_clients_ (num_collcated_clients),
  expected_callbacks_ (0),
  actual_callbacks_ (0),
  servant_error_count_ (0)
{
  for (unsigned i = 0; i < 16; i++)
    {
      this->expected_op_count_[i] = 0;
      this->actual_op_count_[i] = 0;
    }
}


Foo_B_Statistics::~Foo_B_Statistics()
{
}


void
Foo_B_Statistics::expected(unsigned op_num, unsigned count, In_Value_Type type)
{
  this->expected_op_count_[op_num-1] = count * this->num_clients_;
  this->expected_in_value_type_[op_num-1] = type;
}


void
Foo_B_Statistics::actual(unsigned op_num, unsigned count)
{
  this->actual_op_count_[op_num-1] += count;
}


void
Foo_B_Statistics::actual(unsigned op_num, LongVector lv)
{
  unsigned sz = lv.size();
  for (unsigned i = 0; i < sz; i++)
    {
      this->actual_in_long_[op_num-1].push_back (lv[i]);
    }
}


void
Foo_B_Statistics::actual(unsigned op_num, StringVector sv)
{
  unsigned sz = sv.size();
  for (unsigned i = 0; i < sz; i++)
    {
      this->actual_in_string_[op_num-1].push_back (sv[i]);
    }
}


void
Foo_B_Statistics::expected_callbacks (unsigned num_cbs)
{
  expected_callbacks_ = num_cbs * this->num_collcated_clients_;
}


void
Foo_B_Statistics::actual_callbacks (unsigned num_cbs)
{
  actual_callbacks_ += num_cbs;
}


void
Foo_B_Statistics::servant_error_count (unsigned error_count)
{
  this->servant_error_count_ += error_count;
}


bool
Foo_B_Statistics::actual_vs_expected()
{
  bool ret = true;

  // Verify the checking results in servant operation code.
  if (this->servant_error_count_ > 0)
    {
      ACE_ERROR((LM_ERROR, "(%P|%t)Foo_B_Statistics::actual_vs_expected  "
        "servant_error_count_=%u\n", servant_error_count_));
      if (ret)
        ret = false;
    }

  // Verify the number of callbacks received for the collocated
  // client.
  if (actual_callbacks_ != expected_callbacks_)
    {
      ACE_ERROR((LM_ERROR, "(%P|%t)Foo_B_Statistics::actual_vs_expected  "
        "actual_callbacks_=%u expected_callbacks_=%u\n",
        actual_callbacks_, expected_callbacks_));
      if (ret)
        ret = false;
    }

  // Verify the number of operations server received.
  for (unsigned int z = 0; z < 16; z++)
    {
      if (this->expected_op_count_[z] != this->actual_op_count_[z])
        {
          ACE_ERROR((LM_ERROR, "(%P|%t)Foo_B_Statistics::actual_vs_expected  "
            "expected_op_count_[%u]=%u actual_op_count_[%u]=%u\n",
            z, expected_op_count_[z], z, actual_op_count_[z]));
          if (ret)
            ret = false;
        }
    }

  Foo_B_Statistics stats (1, 0);
  Foo_B_ClientEngine::expected_results (stats);

  for (unsigned i = 0; i < 16; i++)
    {
      switch (expected_in_value_type_[i])
      {
        case FOO_B_STAT_LONG:
          {
            unsigned actual_size
              = this->actual_in_long_[i].size ();
            unsigned expected_size
              = this->num_clients_ * stats.expected_op_count_[i];
            if (actual_size != expected_size)
              {
                ACE_ERROR((LM_ERROR, "(%P|%t)Foo_B_Statistics::actual_vs_expected  "
                  "actual_size=%u expected_size=%u\n", actual_size, expected_size));
                if (ret)
                  ret = false;
              }
            sort (this->actual_in_long_[i]);

            for (unsigned j = 0; j < actual_size - 1; j++)
              {
                if (this->actual_in_long_[i][j] != this->actual_in_long_[i][j + 1] - 1)
                  {
                    ACE_ERROR((LM_ERROR, "(%P|%t)Foo_B_Statistics::actual_vs_expected  "
                      "actual_in_long_[%u][%u]=%d actual_in_long_[%u][%u]=%d\n",
                      i, j, actual_in_long_[i][j], i, j+1, actual_in_long_[i][j + 1]));
                    if (ret)
                      ret = false;
                  }
              }
          }
          break;

        case FOO_B_STAT_STRING:
          {
            unsigned actual_size
              = this->actual_in_string_[i].size ();
            unsigned expected_size
              = this->num_clients_ * stats.expected_op_count_[i];
            if (actual_size != expected_size)
              {
                ACE_ERROR((LM_ERROR, "(%P|%t)Foo_B_Statistics::actual_vs_expected  "
                  "actual_size=%u expected_size=%u\n", actual_size, expected_size));
                if (ret)
                  ret = false;
              }

            LongVector lvec;
            char buffer[50];

            for (unsigned k = 0; k < actual_size; k++)
              {
                unsigned client_id;
                sscanf (this->actual_in_string_[i][k].c_str(), "%u %s", &client_id, buffer);
                if (ACE_OS::strcmp (buffer, ONEWAY_ARG_TEST_STR) != 0)
                  {
                    ACE_ERROR((LM_ERROR, "(%P|%t)Foo_B_Statistics::actual_vs_expected  "
                      "actual_in_string_[%u][%u]=%s\n",
                      i, k, actual_in_string_[i][k].c_str()));
                    if (ret)
                      ret = false;
                  }
                lvec.push_back (client_id);
              }

            sort (lvec);

            for (unsigned j = 0; j < actual_size - 1; j++)
              {
                if (lvec[j] != lvec[j + 1] - 1)
                  {
                    ACE_ERROR((LM_ERROR, "(%P|%t)Foo_B_Statistics::actual_vs_expected  "
                      "lvec[%u]=%d lvec[%u]=%d\n",
                      j, lvec[j], j+1, lvec[j + 1]));
                    if (ret)
                      ret = false;
                  }
              }
          }
          break;

        case FOO_B_STAT_NONE:
        default:
          break;
      }

    }

  return ret;
}

