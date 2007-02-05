// $Id$

#include "tao/ORB.h"
#include "TestS.h"

class TestImpl
  : public virtual POA_Test
{
public:
  TestImpl (CORBA::ORB_ptr orb);

  // = The skeleton methods
  virtual void myMethod (void)
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual void shutdown (void)
    ACE_THROW_SPEC ((CORBA::SystemException));

private:
  CORBA::ORB_var orb_;
};
