//$Id$

#include "testC.h"

extern bool dtor_called;

class Test_Smart_Proxy: public virtual TAO_Test_Smart_Proxy_Base
{
public:
  Test_Smart_Proxy (Test_ptr proxy);
  virtual ~Test_Smart_Proxy ();
};

class  Test_Smart_Factory: public TAO_Test_Default_Proxy_Factory
{
public:
  virtual Test_ptr create_proxy (Test_ptr proxy);
};
