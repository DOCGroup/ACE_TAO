#ifndef TAO_TEST_I_H
#define TAO_TEST_I_H

#include "testS.h"

class test_impl : public virtual POA_test
{
  char * test_op (char const *in_arg) override;
};

#endif /* TAO_TEST_I_H */
