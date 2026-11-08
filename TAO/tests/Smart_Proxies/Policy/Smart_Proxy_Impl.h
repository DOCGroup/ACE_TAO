#include "testC.h"

class Smart_Test_Factory : public virtual TAO_Test_Default_Proxy_Factory
{
public:
  explicit Smart_Test_Factory (bool one_shot_factory = true);
  Smart_Test_Factory (Smart_Test_Factory const &) = delete;
  Smart_Test_Factory (Smart_Test_Factory &&) = delete;
  Smart_Test_Factory &operator= (Smart_Test_Factory const &) = delete;
  Smart_Test_Factory &operator= (Smart_Test_Factory &&) = delete;

  Test_ptr create_proxy (Test_ptr proxy) override;
  // This method will create the smart_proxy.
};

class Smart_Test_Proxy : public virtual TAO_Test_Smart_Proxy_Base
{
public:
  explicit Smart_Test_Proxy (Test_ptr proxy);
  Smart_Test_Proxy (Smart_Test_Proxy const &) = delete;
  Smart_Test_Proxy (Smart_Test_Proxy &&) = delete;
  Smart_Test_Proxy &operator= (Smart_Test_Proxy const &) = delete;
  Smart_Test_Proxy &operator= (Smart_Test_Proxy &&) = delete;

  CORBA::Short method (CORBA::Short boo) override;
  // "Smartify" the method call!

  // Don't need to add any extra functionality into <shutdown>, hence
  // we don't define it.
};
