// $Id$

#ifndef AMI_SERVER_H
#define AMI_SERVER_H

#include "ace/Task.h"
#include "AMI_internal_i.h"

class AMI_server : public ACE_Task_Base
{
public:
  /// ctor
  AMI_server (
    CORBA::ORB_ptr orb,
    ::CCM_AMI::AMI_foo_ptr foo_receiver);
  virtual int svc (void);

private:
  CORBA::ORB_var orb_;
  ::CCM_AMI::AMI_foo_var foo_receiver_;
};

#endif /* AMI_SERVER_H */

