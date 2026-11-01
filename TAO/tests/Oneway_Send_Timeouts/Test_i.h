#ifndef _TEST_I_
#define _TEST_I_

#include "TestS.h"

class Test_i : public virtual POA_Test
{
public:
  explicit Test_i (CORBA::ORB_ptr orb);
  ~Test_i () override;

  Test_i (Test_i const &) = delete;
  Test_i &operator= (Test_i const &) = delete;
  Test_i (Test_i &&) = delete;
  Test_i &operator= (Test_i &&) = delete;

  void sleep (CORBA::Long sec, CORBA::Long msec) override;

  void unsleep () override;

  void dummy_one_way (char const *msg) override;

  void dummy_two_way () override;

  void shutdown () override;

private:
  volatile bool sleep_;
  volatile bool unsleep_;
  volatile bool shutdown_;
  CORBA::ORB_var orb_;
};

#endif // _TEST_I_
