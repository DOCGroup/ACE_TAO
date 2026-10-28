#include "tao/ORB.h"
#include "TestS.h"

class TestImpl
  : public virtual POA_Test
{
public:
  TestImpl (CORBA::ORB_ptr orb);

  TestImpl (TestImpl const &) = delete;
  TestImpl (TestImpl &&) = delete;
  TestImpl &operator= (TestImpl const &) = delete;
  TestImpl &operator= (TestImpl &&) = delete;

  // = The skeleton methods
  void myMethod () override;

  void shutdown () override;

private:
  CORBA::ORB_var orb_;
};
