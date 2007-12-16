// $Id$

#include "testS.h"

class TestImpl: public POA_SimpleStrings
{
public:
  TestImpl(CORBA::ORB_ptr orb);

  virtual void get_strings (ArrayOfString_out strings);

private:
  CORBA::ORB_var orb_;
};
