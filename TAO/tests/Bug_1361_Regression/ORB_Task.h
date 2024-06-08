/**
 * @file ORB_Task.h
 *
 * @author Carlos O'Ryan <coryan@atdesk.com>
 */
#ifndef TAO_REGRESSION_ORB_TASK_H
#define TAO_REGRESSION_ORB_TASK_H
#include "tao/ORB.h"
#include "ace/Task.h"

class ORB_Task : public ACE_Task_Base
{
public:
  ORB_Task (CORBA::ORB_ptr orb);

  virtual int svc ();

private:
  CORBA::ORB_var orb_;
};

#endif /* TAO_REGRESSION_ORB_TASK_H*/
