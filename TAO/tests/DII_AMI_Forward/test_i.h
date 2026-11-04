// -*- C++ -*-


#ifndef TEST_I_H
#define TEST_I_H

#include "testS.h"

class Forward_Test_i : public POA_Forward::Test
{
  // = TITLE
  //   Location Forward exception tester
  //
  // = DESCRIPTION
  //   servant throws a location forward exception when constructed with a
  //   target IOR, otherwise it echos the source string.

public:
  Forward_Test_i (CORBA::ORB_ptr orb);
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
