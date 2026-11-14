#include "testC.h"

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
