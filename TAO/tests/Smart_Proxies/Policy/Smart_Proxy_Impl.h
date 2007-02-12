// $Id$

#include "testC.h"

class Smart_Test_Factory : public virtual TAO_Test_Default_Proxy_Factory
{
public:

  Smart_Test_Factory (int one_shot_factory = 1);

  virtual Test_ptr create_proxy (Test_ptr proxy);
  // This method will create the smart_proxy.
};

class Smart_Test_Proxy : public virtual TAO_Test_Smart_Proxy_Base
{
public:
  Smart_Test_Proxy (Test_ptr proxy);

  virtual CORBA::Short method (CORBA::Short boo);
  // "Smartify" the method call!

  // Don't need to add any extra functionality into <shutdown>, hence
  // we don't define it.
};
