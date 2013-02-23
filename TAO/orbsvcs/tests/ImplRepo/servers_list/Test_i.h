/* -*- C++ -*- $Id$ */

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
  Test_i (short server_num, Terminator &terminator);

  //Destructor
  virtual ~Test_i ();

  virtual CORBA::Short get_server_num (CORBA::Short delay_secs);

  virtual void abort (CORBA::Short delay_secs);

private:
  CORBA::Short server_num_;
  Terminator& terminator_;
};

#endif /* TEST_I_H_ */
