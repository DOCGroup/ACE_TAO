#include "Smart_Proxy_Impl.h"

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
}

