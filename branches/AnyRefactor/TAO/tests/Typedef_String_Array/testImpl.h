// $Id$

#include "testS.h"

class TestImpl: public POA_SimpleStrings
{
public:
  TestImpl(CORBA::ORB_ptr orb);

  virtual void get_strings (ArrayOfString_out strings
                            ACE_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

private:
  CORBA::ORB_var orb_;
};
