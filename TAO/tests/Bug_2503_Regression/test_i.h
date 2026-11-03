#ifndef test_i_h
#define test_i_h

#include "TestS.h"

class test_i
  : public POA_Test
{
public:
  explicit test_i (CORBA::ORB_ptr orb);

  test_i (test_i const &) = delete;
  test_i &operator= (test_i const &) = delete;
  test_i (test_i &&) = delete;
  test_i &operator= (test_i &&) = delete;

  void the_operation (CORBA::Long &x) override;

  void shutdown () override;

  CORBA::String_var create_and_activate_server () const;
private:
  CORBA::ORB_var const orb_;
};

#endif // test_i_h
