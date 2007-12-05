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

	  //Change rate fields to double, Yingming Chen. 3.23.2007

      /// Minimum execution rate (in Hz).
      double min_rate;

      /// Maximum execution rate (in Hz).
      double max_rate;

      /// Current execution rate (in Hz).
      double curr_rate;

      // Rate change after EUCON, new rate = curr_rate +
      // delta_rate. Yingming Chen. 3.23.2007
	  double delta_rate;

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
