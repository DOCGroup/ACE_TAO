/* -*- C++ -*-  */

#ifndef TEST_I_H_
#define TEST_I_H_

#include "TestS.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class  Test_i : public virtual POA_Test
{
public:
  //Constructor
  Test_i (CORBA::ORB_ptr orb);

  //Destructor
  virtual ~Test_i ();

  virtual void arm ();

  virtual void trigger ();

  int exit_code () const;

private:
  CORBA::ORB_var orb_;
  bool armed_;
  int exit_code_;
};

#endif /* TEST_I_H_ */
