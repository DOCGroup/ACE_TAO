/**
 * @file Receive_Task.h
 *
 * $Id$
 *
 */

#ifndef ECCL_RECEIVE_TASK_H
#define ECCL_RECEIVE_TASK_H

#include "tao/ORB.h"
#include "ace/Task.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

/**
 * @class ECCL_Receive_Task
 *
 * @brief Implement a simple ACE_Task to receive the events
 *
 */
class ECCL_Receive_Task : public ACE_Task_Base
{
public:
  /// Constructor
  ECCL_Receive_Task (CORBA::ORB_ptr orb);

  /// Run the experiment
  int svc (void);

private:
  /// The consumer
  CORBA::ORB_var orb_;
};


#endif /* ECCL_RECEIVE_TASK_H */
