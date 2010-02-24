// $Id$

#ifndef _ORB_TASK_
#define _ORB_TASK_

#include "tao/ORB_Core.h"

#include "ace/Task.h"

class ORB_Task : public ACE_Task_Base
{
 private:
  CORBA::ORB_var orb_;

 public:
  ORB_Task (CORBA::ORB_ptr orb)
    : orb_ (CORBA::ORB::_duplicate (orb))
    {
      if (CORBA::is_nil (orb_.in()) == 1) {
  ACE_ERROR ((LM_ERROR, "ORB_Task> Ctr> Orb is NULL\n"));
      }
    };

  virtual int svc ()
  {
    if (CORBA::is_nil (orb_.in()) == 0) {
      orb_->run();
    }

    return -1;
  };
};

#endif //  _ORB_TASK_
