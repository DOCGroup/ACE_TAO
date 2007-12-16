//$Id$

#include "Smart_Proxy_Impl.h"

Test_ptr
Test_Smart_Factory::create_proxy (Test_ptr proxy)
{
  // creat Smart_Proxy
  ACE_DEBUG ((LM_DEBUG,
            "create a smart_proxy\n"));

  if (CORBA::is_nil (proxy) == 0)
    ACE_NEW_RETURN (proxy,
                    Test_Smart_Proxy(proxy),
                    0);

  return proxy;
}

Test_Smart_Proxy::Test_Smart_Proxy (Test_ptr proxy)
 : TAO_Smart_Proxy_Base (proxy)
{
  dtor_called = false;
  ACE_DEBUG ((LM_DEBUG, "Smart Proxy constructor called\n"));
}

Test_Smart_Proxy::~Test_Smart_Proxy()
{
  dtor_called = true;
  ACE_DEBUG ((LM_DEBUG, "Smart Proxy destructor called\n"));
}

