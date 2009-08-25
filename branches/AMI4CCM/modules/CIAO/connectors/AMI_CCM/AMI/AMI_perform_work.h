
#ifndef AMI_PERFORM_WORK_H
#define AMI_PERFORM_WORK_H

#include "ace/Task.h"
#include "AMIC.h"

class AMI_perform_work : public ACE_Task_Base
{
public:
  /// ctor
  AMI_perform_work (CORBA::ORB_ptr orb);
  virtual int svc (void);

private:
  CORBA::ORB_var orb_;
};

#endif /* AMI_PERFORM_WORK_H */

