// $Id$

#ifndef cubit_ih
#define cubit_ih

#include <corba/cool.H>
#include "cubit.H"
#include "Task_Client.h"

class Cubit_Impl {
public:
  Cubit_Impl (Task_State *ts);
  virtual CORBA_Octet cube_octet (CORBA_Octet o, CORBA_Environment &IT_env=_environment);
  virtual CORBA_Short cube_short (CORBA_Short s, CORBA_Environment &IT_env=_environment);
  virtual CORBA_Long cube_long (CORBA_Long l, CORBA_Environment &IT_env=_environment);
  virtual Cubit::Many cube_struct (const Cubit::Many& values, CORBA_Environment &IT_env=_environment);
  virtual Cubit::oneof cube_union (const Cubit::oneof& values, CORBA_Environment &IT_env=_environment);
  virtual void please_exit (CORBA_Environment &IT_env=_environment);

private:
  Task_State *ts_;
  //pointer to the task state
  int util_started_;
  // flag to indicate utilization test has started
};

#endif
