// $Id$

#include "testC.h"
class Smart_Test_Factory : public virtual TAO_Test_Default_Proxy_Factory
{
public:

  Smart_Test_Factory (void);

  virtual Test_ptr create_proxy (Test_ptr proxy
                                 ACE_ENV_ARG_DECL_WITH_DEFAULTS);
  // This method will create the smart_proxy.
};

class Smart_Test_Proxy : public virtual TAO_Test_Smart_Proxy_Base
{
public:
  Smart_Test_Proxy (Test_ptr proxy);

  virtual CORBA::Short box_prices(ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));
  // "Smartify" the method call!

 private:
  CORBA::Short price_;
};
