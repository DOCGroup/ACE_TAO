// -*- C++ -*-

#ifndef TEST_I_H
#define TEST_I_H

#include "testS.h"

/// Servant that throws a location-forward exception when given a target IOR;
/// otherwise, it echoes the source string.
class Forward_Test_i : public POA_Forward::Test
{
public:
  explicit Forward_Test_i (CORBA::ORB_ptr orb);
  Forward_Test_i (Forward_Test_i const &) = delete;
  Forward_Test_i (Forward_Test_i &&) = delete;
  Forward_Test_i &operator= (Forward_Test_i const &) = delete;
  Forward_Test_i &operator= (Forward_Test_i &&) = delete;

  char * do_forward (char const *text) override;
  void shutdown () override;

private:
  CORBA::ORB_var const orb_;
};

#endif /* TEST_I_H */
