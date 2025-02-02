//=============================================================================
/**
 *  @file    OrbTask.h
 *
 *  @author  Tim Bradley <bradley_t@ociweb.com>
 */
//=============================================================================

#ifndef ORB_TASK_H
#define ORB_TASK_H

#include "CSD_TP_Test_Export.h"
#include "ace/Task.h"
#include "tao/ORB.h"

class CSD_TP_Test_Export OrbTask : public ACE_Task_Base
{
public:
  OrbTask(CORBA::ORB_ptr orb, unsigned num_threads = 1);
  ~OrbTask() override = default;

  int open(void* x = 0) override;
  int svc() override;
  int close(u_long) override;

private:
  CORBA::ORB_var orb_;
  unsigned       num_threads_;
};

#endif
