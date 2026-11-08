#include "Smart_Proxy_Impl.h"

Smart_Test_Factory::Smart_Test_Factory (bool one_shot_factory)
  : TAO_Test_Default_Proxy_Factory (one_shot_factory)
{
  ACE_DEBUG ((LM_DEBUG,
              "Smart_Test_Factory\n"));
}

Test_ptr
Smart_Test_Factory::create_proxy (Test_ptr proxy)
{
  ACE_DEBUG ((LM_DEBUG,
              "create_smart_proxy\n"));

  if (!CORBA::is_nil (proxy))
    ACE_NEW_RETURN (proxy, Smart_Test_Proxy (proxy), nullptr);

  return proxy;
}

Smart_Test_Proxy::Smart_Test_Proxy (Test_ptr proxy)
  : TAO_Smart_Proxy_Base (proxy)
{
}

CORBA::Short
Smart_Test_Proxy::method (CORBA::Short boo)
{
  ACE_DEBUG ((LM_DEBUG,
              "Yahoo, I am smart\n"));

  try
    {
      return TAO_Test_Smart_Proxy_Base::method (boo);
    }
  catch (Test::Oops const &reason)
    {
      reason._tao_print_exception ("User Exception");
      return -1;
    }

  return -1;
}
