/* -*- C++ -*- */
// $Id$

#include "ace/Log_Msg.h"
#include "Test.hh"

class Test_i
{
public:
  virtual void method (long input,
		       ACE_CORBA_1 (Environment) &IT_env = ACE_CORBA_1 (default_environment));
};

DEF_TIE_Test (Test_i)
