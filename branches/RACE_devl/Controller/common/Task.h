#ifndef TASK_H
#define TASK_H
#include <string>
#include <vector>

#include "Subtask.h"

namespace CIAO
{
  namespace RACE
  {
    /// Structure of an end-to-end task.
    struct Task
    {
      /// Unique identifier.
      std::string UUID;

      /// Logical name.
      std::string label;

      /// Minimum execution rate (in Hz).
      size_t min_rate;

      /// Maximum execution rate (in Hz).
      size_t max_rate;

      /// Current execution rate (in Hz).
      size_t curr_rate;

      /// Current RT-CORBA priority.
      size_t priority;

      /// Relative importance.
      size_t importance;

      /// End-to-end deadline.
      double deadline;

      /// Subtaks of this task.
      std::vector <Subtask> subtasks;
    };
  }
}

#endif /* TASK_H */
