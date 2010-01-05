// $Id$

#ifndef test_i_h
#define test_i_h

#include "TestS.h"

class test_i
  : public POA_Test
{
public:
  test_i(CORBA::ORB_ptr);

  virtual void the_operation(CORBA::Long & x);

  virtual void shutdown (void);

  char * create_and_activate_server(void);
private:
  CORBA::ORB_var orb_;
};

#endif // test_i_h
