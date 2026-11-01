#ifndef _ORB_TASK_
#define _ORB_TASK_

#include "tao/ORB_Core.h"

#include "ace/Task.h"

class ORB_Task : public ACE_Task_Base
{
private:
  CORBA::ORB_var orb_;

public:
  explicit ORB_Task (CORBA::ORB_ptr orb)
    : orb_ (CORBA::ORB::_duplicate (orb))
  {
    if (CORBA::is_nil (orb_.in ()))
    {
      ACE_ERROR ((LM_ERROR, "ORB_Task> Ctr> Orb is NULL\n"));
    }
  };

  ORB_Task (ORB_Task const &) = delete;
  ORB_Task &operator= (ORB_Task const &) = delete;
  ORB_Task (ORB_Task &&) = delete;
  ORB_Task &operator= (ORB_Task &&) = delete;

  int svc () override
  {
    if (!CORBA::is_nil (orb_.in ()))
    {
      orb_->run ();
    }

    return -1;
  };
};

#endif //  _ORB_TASK_
