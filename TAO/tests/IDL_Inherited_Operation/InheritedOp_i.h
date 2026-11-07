// -*- C++ -*-
#ifndef INHERITED_OP_I_H
#define INHERITED_OP_I_H


#include "InheritedOpS.h"

class Derived_i : public virtual POA_Outer::Derived
{
public:
  explicit Derived_i (CORBA::ORB_ptr orb);

  void BaseOp () override;
  void shutdown () override;

  Derived_i (Derived_i const &) = delete;
  Derived_i (Derived_i &&) = delete;
  Derived_i &operator= (Derived_i const &) = delete;
  Derived_i &operator= (Derived_i &&) = delete;

private:
  CORBA::ORB_var orb_ {};
};

#endif // INHERITED_OP_I_H
