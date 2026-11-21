// -*- C++ -*-
#ifndef NESTEDVALUEFACTORY_H
#define NESTEDVALUEFACTORY_H

#include "DemoC.h"

class Demo_NestedValueFactory : public virtual CORBA::ValueFactoryBase
{
public:
  CORBA::ValueBase *create_for_unmarshal () override;
};

#endif
