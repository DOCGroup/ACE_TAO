/**
 * @file Client_Task.h
 *
 * $Id$
 *
 */

#ifndef ECS_CLIENT_TASK_H
#define ECS_CLIENT_TASK_H

#include "tao/ORB.h"
#include "ace/Task.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

/**
 * @class ECS_Client_Task
 *
 * @brief Implement a simple ACE_Task to receive the events
 *
 */
class ECS_Client_Task : public ACE_Task_Base
{
public:
  /// Constructor
  ECS_Client_Task (CORBA::ORB_ptr orb);

  /// Run the experiment
  int svc (void);

private:
  /// The consumer
  CORBA::ORB_var orb_;
};


#endif /* ECS_CLIENT_TASK_H */
