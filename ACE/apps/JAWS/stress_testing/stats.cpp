#include "stats.h"
#include <algorithm>
#include <iostream>

Stats::Stats (int size)
  : throughput_ (size > 0 ? static_cast<size_t> (size) : 0),
    latency_ (size > 0 ? static_cast<size_t> (size) : 0),
    init_fini_ (size > 0 ? static_cast<size_t> (2 * size) : 0),
    thread_count_ (size > 0 ? size : 0)
{
}

void
Stats::log (int id, float throughput, float latency)
{
  if (id < 0 || id >= this->thread_count_)
    return;
  throughput_[id] = throughput;
  latency_[id] = latency;
}

void
Stats::output ()
{
  if (this->thread_count_ == 0)
    {
      std::cout << " 0 0 0";
      return;
    }

  std::sort (this->init_fini_.begin (), this->init_fini_.end (),
             [] (Init_Fini_t const &lhs, Init_Fini_t const &rhs)
             {
               if (!(lhs.timestamp < rhs.timestamp)
                   && !(rhs.timestamp < lhs.timestamp))
                 return lhs.type == THREAD_START && rhs.type == THREAD_END;
               return lhs.timestamp < rhs.timestamp;
             });

  int max {};
  int thread_peak {};
  for (Init_Fini_t const &event : this->init_fini_)
    if (event.type == THREAD_START)
      {
        if (++thread_peak > max)
          max = thread_peak;
      }
    else
      --thread_peak;

  float tavg {};
  float lavg {};
  for (int i = 0; i < this->thread_count_; ++i)
    {
      tavg += this->throughput_[i];
      lavg += this->latency_[i];
    }
  std::cout << " " << tavg / this->thread_count_ << " "
       << lavg / this->thread_count_ << " " << max;
}

void
Stats::i_have_started (int id)
{
  if (id < 0 || id >= this->thread_count_)
    return;
  this->init_fini_[2 * id].type = THREAD_START;
  this->init_fini_[2 * id].timestamp = ACE_OS::gettimeofday ();
}

void
Stats::i_am_done (int id)
{
  if (id < 0 || id >= this->thread_count_)
    return;
  this->init_fini_[(2 * id) + 1].type = THREAD_END;
  this->init_fini_[(2 * id) + 1].timestamp = ACE_OS::gettimeofday ();
}

