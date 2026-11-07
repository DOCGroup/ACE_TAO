//=============================================================================
/**
 *  @file    OrbTask.h
 *
 *  @author  Tim Bradley <bradley_t@ociweb.com>
 */
//=============================================================================

#ifndef ORB_TASK_H
#define ORB_TASK_H

#include "ace/Task.h"
#include "tao/ORB.h"

class OrbTask : public ACE_Task_Base
{
public:
  explicit OrbTask (CORBA::ORB_ptr orb, unsigned num_threads = 1);
  ~OrbTask () override = default;

  OrbTask (OrbTask const &) = delete;
  OrbTask (OrbTask &&) = delete;
  OrbTask &operator= (OrbTask const &) = delete;
  OrbTask &operator= (OrbTask &&) = delete;

  int open (void * = nullptr) override;
  int svc() override;
  int close(u_long) override;

private:
  CORBA::ORB_var orb_;
  unsigned num_threads_ {1};
};

#endif
