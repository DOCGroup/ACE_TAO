//=============================================================================
/**
 *  @file    OrbShutdownTask.h
 *
 *  @author  Tim Bradley <bradley_t@ociweb.com>
 */
//=============================================================================

#ifndef ORB_SHUTDOWN_TASK_H
#define ORB_SHUTDOWN_TASK_H

#include "CSD_TP_Test_Export.h"
#include "ace/Task.h"
#include "tao/ORB.h"

class CSD_TP_Test_Export OrbShutdownTask : public ACE_Task_Base
{
public:
  OrbShutdownTask () = default;
  ~OrbShutdownTask () override = default;

  void orb(CORBA::ORB_ptr orb);

  int open(void* = nullptr) override;
  int svc() override;
  int close(u_long) override;

  OrbShutdownTask (OrbShutdownTask const &) = delete;
  OrbShutdownTask (OrbShutdownTask &&) = delete;
  OrbShutdownTask & operator= (OrbShutdownTask const &) = delete;
  OrbShutdownTask & operator= (OrbShutdownTask &&) = delete;

private:
  CORBA::ORB_var orb_;
};

#endif
