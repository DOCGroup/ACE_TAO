#ifndef _TEST_I_
#define _TEST_I_

#include "TestS.h"

class Test_i : public virtual POA_Test
{
 public:
  Test_i (CORBA::ORB_ptr orb);
  virtual ~Test_i ();

  virtual void sleep (CORBA::Long sec, CORBA::Long msec)
    ACE_THROW_SPEC ((::CORBA::SystemException));

  virtual void unsleep ()
    ACE_THROW_SPEC ((::CORBA::SystemException));

  virtual void dummy_one_way (const char* msg)
    ACE_THROW_SPEC ((::CORBA::SystemException));

  virtual void dummy_two_way ()
    ACE_THROW_SPEC ((::CORBA::SystemException));

  virtual void shutdown ()
    ACE_THROW_SPEC ((::CORBA::SystemException));

 private:
  volatile bool sleep_;
  volatile bool unsleep_;
  CORBA::ORB_var orb_;
};

#endif // _TEST_I_
