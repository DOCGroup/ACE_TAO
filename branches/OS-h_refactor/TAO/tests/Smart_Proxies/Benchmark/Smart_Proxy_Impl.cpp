// $Id$

#include "Smart_Proxy_Impl.h"
Smart_Test_Factory::Smart_Test_Factory (void)
{
  ACE_DEBUG ((LM_DEBUG,
              "Smart_Test_Factory\n"));
}

Test_ptr
Smart_Test_Factory::create_proxy (Test_ptr proxy
                                  ACE_ENV_ARG_DECL_NOT_USED)
 {
   ACE_DEBUG ((LM_DEBUG,
               "create_smart_proxy\n"));

   if (CORBA::is_nil (proxy) == 0)
       ACE_NEW_RETURN (proxy, Smart_Test_Proxy (proxy), 0);

   return proxy;

 }

Smart_Test_Proxy::Smart_Test_Proxy (Test_ptr proxy)
  : TAO_Smart_Proxy_Base (proxy),
    price_ (0)
{
}

CORBA::Short
Smart_Test_Proxy::box_prices (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  if (this->price_ == 0)
    {
      this->price_ = TAO_Test_Smart_Proxy_Base::box_prices (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK_RETURN (0);
    }
  return this->price_;
}
