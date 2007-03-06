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
      size_t min_rate_;

      /// Maximum execution rate (in Hz).
      size_t max_rate_;

      /// Current execution rate (in Hz).
      size_t curr_rate_;

      /// Current RT-CORBA priority.
      size_t priority_;

      /// Relative importance.
      size_t importance_;

      /// End-to-end deadline.
      double deadline_;

      /// Subtaks of this task.
      std::vector <Subtask> subtasks_;
    };
  }
}

#endif /* TASK_H */
