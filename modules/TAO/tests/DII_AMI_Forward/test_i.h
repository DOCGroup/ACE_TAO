// -*- C++ -*-

//
// $Id$
//

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

  char * do_forward (const char *text);
  void shutdown ();

private:
  CORBA::ORB_var orb_;
};

#endif /* TEST_I_H */
