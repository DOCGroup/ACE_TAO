//=============================================================================
/**
 *  @file    OrbShutdownTask.h
 *
 *  @author  Tim Bradley <bradley_t@ociweb.com>
 */
//=============================================================================

#ifndef ORB_SHUTDOWN_TASK_H
#define ORB_SHUTDOWN_TASK_H

#include "ace/Task.h"
#include "tao/ORB.h"

class OrbShutdownTask : public ACE_Task_Base
{
public:
  OrbShutdownTask () = default;
  ~OrbShutdownTask () override = default;
  OrbShutdownTask (OrbShutdownTask const &) = delete;
  OrbShutdownTask (OrbShutdownTask &&) = delete;
  OrbShutdownTask & operator= (OrbShutdownTask const &) = delete;
  OrbShutdownTask & operator= (OrbShutdownTask &&) = delete;

  void orb (CORBA::ORB_ptr orb);

  int open (void *) override;
  int svc () override;
  int close (u_long) override;

private:
  CORBA::ORB_var orb_;
};

using TheOrbShutdownTask = ACE_Singleton<OrbShutdownTask, TAO_SYNCH_MUTEX>;

#endif
