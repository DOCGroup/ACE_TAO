// -*- C++ -*-
#ifndef BASEVALUEFACTORY_H
#define BASEVALUEFACTORY_H

#include "DemoC.h"

class Demo_BaseValueFactory : public virtual Demo::BaseValue_init
{
public:
  CORBA::ValueBase *create_for_unmarshal () override;
  Demo::BaseValue *create (char const *str) override;
};

#endif
