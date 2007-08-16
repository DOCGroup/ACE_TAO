// $Id$

#ifndef TAO_TEST_I_H
#define TAO_TEST_I_H

#include "testS.h"

class foo_i : public virtual POA_foo
{
  virtual char * foo_op (const char * inarg);

  virtual char * base_op (const char * inarg);
};

class passer_i : public virtual POA_passer
{
public:
  passer_i (PortableServer::POA_ptr poa);

  virtual void pass_ops (base_out outarg);

  virtual void pass_state (base_out outarg);

private:
  PortableServer::POA_var poa_;
};

#endif /* TAO_TEST_I_H */
