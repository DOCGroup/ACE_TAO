// $Id$

#ifndef test_i_h
#define test_i_h

#include "TestS.h"

class test_i
  : public POA_Test
{
public:
  test_i();

  virtual void the_operation(CORBA::Long & x)
    throw(CORBA::SystemException);

  static char * create_and_activate_server(CORBA::ORB_ptr);
};

#endif // test_i_h
