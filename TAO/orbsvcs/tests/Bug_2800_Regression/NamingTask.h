#ifndef NAMINGTASK_H
#define NAMINGTASK_H

#include "tao/CORBA_String.h"
#include "tao/ORB.h"
#include "ace/Task.h"
#include "ace/Synch_Traits.h"

class NamingTask : public ACE_Task_Base
{
public:
  NamingTask (char const * orbname, int argc, ACE_TCHAR ** argv, unsigned short port);
  NamingTask (NamingTask const &) = delete;
  NamingTask (NamingTask &&) = delete;
  NamingTask &operator= (NamingTask const &) = delete;
  NamingTask &operator= (NamingTask &&) = delete;

  int svc () override;
  void waitInit ();
  void end ();

  char const * ior ();

private:
  CORBA::ORB_var orb_;
  bool initialized_;
  CORBA::String_var ior_;
};

#endif
