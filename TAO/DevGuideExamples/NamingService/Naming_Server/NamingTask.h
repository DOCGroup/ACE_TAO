// $Id$

#ifndef NAMINGTASK_H
#define NAMINGTASK_H

#include "tao/corba.h"
#include "ace/Task.h"

class NamingTask : public ACE_Task<ACE_MT_SYNCH>
{
public:
  NamingTask (int argc, ACE_TCHAR** argv);
  virtual int svc();
  void waitInit ();
  void end();

private:
  int argc_;
  ACE_TCHAR **argv_;
  CORBA::ORB_var orb_;
  bool initialized_;
};

#endif
