// -*- C++ -*-
// $Id$

#ifndef INHERITED_OP_I_H
#define INHERITED_OP_I_H


#include "InheritedOpS.h"

class Derived_i : public virtual POA_Outer::Derived
{
public:
  Derived_i (CORBA::ORB_ptr orb);

  virtual void BaseOp (void);
  virtual void shutdown (void);

private:
  CORBA::ORB_ptr orb_;
};

#endif // INHERITED_OP_I_H
