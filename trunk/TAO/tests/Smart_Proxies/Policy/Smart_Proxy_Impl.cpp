// $Id$

#include "Smart_Proxy_Impl.h"

Smart_Test_Factory::Smart_Test_Factory (int one_shot_factory)
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

   if (CORBA::is_nil (proxy))
       ACE_NEW_RETURN (proxy, Smart_Test_Proxy (proxy), 0);

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

  CORBA::Short retval = 0;
  try
    {
      retval = TAO_Test_Smart_Proxy_Base::method (boo);
    }
  catch (const Test::Oops& reason)
    {
      reason._tao_print_exception ("User Exception");
      return -1;
    }

  return retval;
}
