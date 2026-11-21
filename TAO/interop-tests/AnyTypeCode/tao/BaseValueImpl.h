// -*- C++ -*-
#ifndef BASEVALUEIMPL_H
#define BASEVALUEIMPL_H

#include "DemoC.h"

class Demo_BaseValueImpl : public virtual OBV_Demo::BaseValue,
                           public virtual CORBA::DefaultValueRefCountBase
{
public:
  Demo_BaseValueImpl () = default;
  ~Demo_BaseValueImpl () override = default;
  CORBA::ValueBase *_copy_value () override;
};

#endif
