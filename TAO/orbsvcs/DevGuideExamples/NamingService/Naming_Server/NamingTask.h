#ifndef NAMINGTASK_H
#define NAMINGTASK_H

#include "tao/corba.h"
#include "ace/Task.h"

#include <atomic>

class NamingTask : public ACE_Task<ACE_MT_SYNCH>
{
public:
  NamingTask (int argc, ACE_TCHAR *argv[]);
  int svc () override;
  void waitInit ();
  void end ();

  NamingTask (NamingTask const &) = delete;
  NamingTask (NamingTask &&) = delete;
  NamingTask &operator= (NamingTask const &) = delete;
  NamingTask &operator= (NamingTask &&) = delete;

private:
  CORBA::ORB_var const orb_;
  std::atomic<bool> initialized_ {};
};

#endif
