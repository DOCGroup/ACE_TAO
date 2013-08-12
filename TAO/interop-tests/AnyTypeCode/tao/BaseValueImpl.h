#ifndef BASEVALUEIMPL_H
#define BASEVALUEIMPL_H

#include "DemoC.h"

class Demo_BaseValueImpl : public virtual OBV_Demo::BaseValue,
                           public virtual CORBA::DefaultValueRefCountBase
{
public:
  Demo_BaseValueImpl();
  virtual ~Demo_BaseValueImpl();
  virtual CORBA::ValueBase* _copy_value();
};

#endif
