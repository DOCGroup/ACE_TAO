#ifndef TAO_ORBSVCS_TESTS_BUG_3673_REGRESSION_NAMING_TASK_H
#define TAO_ORBSVCS_TESTS_BUG_3673_REGRESSION_NAMING_TASK_H

#include "tao/CORBA_String.h"
#include "tao/ORB.h"
#include "ace/Task.h"
#include "ace/Synch_Traits.h"

#include <atomic>

class NamingTask : public ACE_Task_Base
{
public:
  NamingTask (char const *orbname, int argc, ACE_TCHAR **argv, unsigned short port);

  NamingTask (NamingTask const &) = delete;
  NamingTask (NamingTask &&) = delete;
  NamingTask &operator= (NamingTask const &) = delete;
  NamingTask &operator= (NamingTask &&) = delete;

  int svc () override;
  void waitInit ();
  void end ();

  char const *ior () const;

private:
  CORBA::ORB_var orb_ {};
  std::atomic<bool> initialized_ { false };
  CORBA::String_var ior_ {};
};

#endif /* TAO_ORBSVCS_TESTS_BUG_3673_REGRESSION_NAMING_TASK_H */
