//=============================================================================
/**
 *  @file    OrbTask.h
 *
 *  @author  Tim Bradley <bradley_t@ociweb.com>
 */
//=============================================================================

#ifndef ORB_TASK_H
#define ORB_TASK_H

#include "CSD_PT_TestInf_Export.h"
#include "ace/Task.h"
#include "tao/ORB.h"

class CSD_PT_TestInf_Export OrbTask : public ACE_Task_Base
{
public:
  explicit OrbTask (CORBA::ORB_ptr orb, unsigned num_threads = 1);
  OrbTask (OrbTask const &) = delete;
  OrbTask (OrbTask &&) = delete;
  OrbTask &operator= (OrbTask const &) = delete;
  OrbTask &operator= (OrbTask &&) = delete;
  ~OrbTask () override = default;

  int open (void *) override;
  int svc () override;
  int close (u_long) override;

private:
  CORBA::ORB_var orb_;
  unsigned       num_threads_;
};

#endif
