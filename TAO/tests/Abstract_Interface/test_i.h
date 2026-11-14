// -*- C++ -*-
#ifndef TAO_TEST_I_H
#define TAO_TEST_I_H

#include "testS.h"

class foo_i : public virtual POA_foo
{
public:
  foo_i () = default;
  foo_i (foo_i const &) = delete;
  foo_i (foo_i &&) = delete;
  foo_i &operator= (foo_i const &) = delete;
  foo_i &operator= (foo_i &&) = delete;

  char * foo_op (char const *inarg) override;

  char * base_op (char const *inarg) override;
};

class passer_i : public virtual POA_passer
{
public:
  passer_i (CORBA::ORB_ptr orb, PortableServer::POA_ptr poa);
  passer_i (passer_i const &) = delete;
  passer_i (passer_i &&) = delete;
  passer_i &operator= (passer_i const &) = delete;
  passer_i &operator= (passer_i &&) = delete;

  void pass_ops (base_out outarg) override;

  void pass_state (base_out outarg) override;

  void pass_nil (base_out outarg) override;

  void shutdown () override;
private:
  CORBA::ORB_var orb_;
  PortableServer::POA_var poa_;
};

#endif /* TAO_TEST_I_H */
