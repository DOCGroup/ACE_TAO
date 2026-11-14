
#ifndef TEST_I_H
#define TEST_I_H
#include /**/ "ace/pre.h"

#include "TestS.h"

class test_i
  : public virtual POA_test
{
public:
  explicit test_i (CORBA::ORB_ptr orb);
  test_i (test_i const &) = delete;
  test_i (test_i &&) = delete;
  test_i &operator= (test_i const &) = delete;
  test_i &operator= (test_i &&) = delete;

  // = The skeleton methods
  void send_stuff (char const *string) override;
  void shutdown () override;

private:
  CORBA::ORB_var const orb_;
};

#include /**/ "ace/post.h"
#endif /* TEST_I_H */
