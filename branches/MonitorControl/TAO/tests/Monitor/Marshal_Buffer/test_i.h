
#ifndef TAO_TEST_I_H
#define TAO_TEST_I_H

#include "testS.h"

class test_impl : public virtual POA_test
{
  virtual char * test_op (const char * in_arg);
};

#endif /* TAO_TEST_I_H */
