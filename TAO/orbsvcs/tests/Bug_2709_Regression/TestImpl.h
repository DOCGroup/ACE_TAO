// $Id$

#include "tao/ORB.h"
#include "TestS.h"

class TestImpl
  : public virtual POA_Test
{
public:
  TestImpl (CORBA::ORB_ptr orb);

  // = The skeleton methods
  virtual void myMethod (void);

  virtual void shutdown (void);

private:
  CORBA::ORB_var orb_;
};
