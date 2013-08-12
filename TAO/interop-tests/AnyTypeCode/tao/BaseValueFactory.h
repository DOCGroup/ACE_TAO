#ifndef BASEVALUEFACTORY_H
#define BASEVALUEFACTORY_H

#include "DemoC.h"

class Demo_BaseValueFactory : public virtual Demo::BaseValue_init
{
public:
  virtual CORBA::ValueBase* create_for_unmarshal();
  virtual Demo::BaseValue* create(const char * str);
};

#endif
