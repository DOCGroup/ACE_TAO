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
  ORB_Task (ORB_Task const &) = delete;
  ORB_Task &operator= (ORB_Task const &) = delete;
  ORB_Task (ORB_Task &&) = delete;
  ORB_Task &operator= (ORB_Task &&) = delete;

  int svc () override;

private:
  CORBA::ORB_var orb_;
};

#endif /* TAO_REGRESSION_ORB_TASK_H*/
