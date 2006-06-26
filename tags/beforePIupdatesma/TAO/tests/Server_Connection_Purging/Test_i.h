//
// $Id$
//

#ifndef TEST_I_H
#define TEST_I_H
#include /**/ "ace/pre.h"

#include "TestS.h"

class test_i
  : public virtual POA_test
{
public:
  // = The skeleton methods
  virtual void send_stuff (const char* string
                           ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

private:
};

#include /**/ "ace/post.h"
#endif /* TEST_I_H */
