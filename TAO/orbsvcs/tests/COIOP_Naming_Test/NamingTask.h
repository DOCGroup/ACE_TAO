#ifndef NAMINGTASK_H
#define NAMINGTASK_H

#include "tao/corba.h"
#include "ace/Task.h"
#include "orbsvcs/CosNamingC.h"
#include "ace/Synch_Traits.h"

class NamingTask : public ACE_Task_Base
{
public:
  NamingTask (char const* orbname, int argc, ACE_TCHAR** argv);

  NamingTask (NamingTask const &) = delete;
  NamingTask (NamingTask &&) = delete;
  NamingTask &operator= (NamingTask const &) = delete;
  NamingTask &operator= (NamingTask &&) = delete;

  int svc () override;
  void waitInit ();
  void end ();

  CosNaming::NamingContext_ptr root () const;

private:
  CORBA::ORB_var orb_;
  bool initialized_;
  CosNaming::NamingContext_var root_;
};

#endif
