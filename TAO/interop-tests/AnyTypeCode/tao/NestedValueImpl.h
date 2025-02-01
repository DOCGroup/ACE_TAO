// -*- C++ -*-
#ifndef NESTEDVALUE_H
#define NESTEDVALUE_H

#include "DemoC.h"

class Demo_NestedValueImpl : public virtual OBV_Demo::NestedValue,
                             public virtual CORBA::DefaultValueRefCountBase
{
public:
  Demo_NestedValueImpl ();
  ~Demo_NestedValueImpl ();
  virtual CORBA::ValueBase* _copy_value ();
};

#endif
