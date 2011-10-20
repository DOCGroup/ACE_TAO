// $Id$

#ifndef _TEST_I_
#define _TEST_I_

#include "TestS.h"

class Test_i : public virtual POA_Test
{
 public:
  Test_i (CORBA::ORB_ptr orb);
  virtual ~Test_i ();

  virtual void sleep (CORBA::Long sec, CORBA::Long msec);

  virtual void unsleep ();

  virtual void dummy_one_way (const char* msg);

  virtual void dummy_two_way ();

  virtual void shutdown ();

 private:
  volatile bool sleep_;
  volatile bool unsleep_;
  volatile bool shutdown_;
  CORBA::ORB_var orb_;
};

#endif // _TEST_I_
