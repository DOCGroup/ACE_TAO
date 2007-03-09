#ifndef SUBTASK_H
#define SUBTASK_H

#include <string>
#include <vector>

#include "Domain.h"

namespace CIAO
{
  namespace RACE
  {

    /// Structure of the time value.
    struct time_val
    {
      /// Seconds.
      size_t sec;

      /// Microseconds.
      size_t usec;
    };


    /// Estimated execution times.
    struct Execution_Time
    {
      /// Best case execution time.
      time_val BCET;

      /// Worst case execution time.
      time_val WCET;
    };

    /// Structure of a subtask.
    struct Subtask
    {
      /// Unique identifier.
      std::string UUID;

      /// Logical name.
      std::string label;

      /// "Reference" to the node on which the subtaks is deployed. This is
      /// actually the index into the nodes vector in the domain structure.
      size_t node;

      /// Estimated execution time of the subtask.
      Execution_Time exec_time;
    };
  }
}

#endif /* SUBTASK_H */
