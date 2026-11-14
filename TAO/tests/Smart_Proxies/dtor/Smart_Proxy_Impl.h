#include "testC.h"

extern bool dtor_called;

class Test_Smart_Proxy : public virtual TAO_Test_Smart_Proxy_Base
{
public:
  explicit Test_Smart_Proxy (Test_ptr proxy);
  ~Test_Smart_Proxy () override;
  Test_Smart_Proxy (Test_Smart_Proxy const &) = delete;
  Test_Smart_Proxy (Test_Smart_Proxy &&) = delete;
  Test_Smart_Proxy &operator= (Test_Smart_Proxy const &) = delete;
  Test_Smart_Proxy &operator= (Test_Smart_Proxy &&) = delete;
};

class Test_Smart_Factory : public TAO_Test_Default_Proxy_Factory
{
public:
  Test_Smart_Factory () = default;
  Test_Smart_Factory (Test_Smart_Factory const &) = delete;
  Test_Smart_Factory (Test_Smart_Factory &&) = delete;
  Test_Smart_Factory &operator= (Test_Smart_Factory const &) = delete;
  Test_Smart_Factory &operator= (Test_Smart_Factory &&) = delete;

  Test_ptr create_proxy (Test_ptr proxy) override;
};
