/* -*- C++ -*-  */

#ifndef TEST_I_H_
#define TEST_I_H_

#include "TestS.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class Terminator;

class  Test_i : public virtual POA_Test
{
public:
  //Constructor
  Test_i ();

  //Destructor
  virtual ~Test_i ();

  virtual void foo ();

  virtual CORBA::Short get_call_count ();

private:
  CORBA::Short count_;
};

#endif /* TEST_I_H_ */
