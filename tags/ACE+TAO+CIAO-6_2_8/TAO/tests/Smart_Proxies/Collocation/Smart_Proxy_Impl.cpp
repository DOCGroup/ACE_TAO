// $Id$

#include "Smart_Proxy_Impl.h"

#include "ace/OS_NS_string.h"
#include "ace/Log_Msg.h"

Smart_Diamond_Top_Factory::Smart_Diamond_Top_Factory (void)
{
  ACE_DEBUG ((LM_DEBUG, "Smart_Diamond_Top_Factory\n"));
}

Diamond::Top_ptr
Smart_Diamond_Top_Factory::create_proxy (Diamond::Top_ptr proxy
                                                    )
 {
   ACE_DEBUG ((LM_DEBUG, "create_smart_proxy\n"));
   if (CORBA::is_nil (proxy) == 0)
           ACE_NEW_RETURN (proxy,
                               Smart_Diamond_Top_Proxy (proxy),
                                           0);
   return proxy;

 }

Smart_Diamond_Top_Proxy::Smart_Diamond_Top_Proxy (Diamond::Top_ptr proxy)
  : TAO_Smart_Proxy_Base (proxy)
{
}

char *
Smart_Diamond_Top_Proxy::shape ()
{
  ACE_DEBUG ((LM_DEBUG, "Smart_proxy: This is "));

  char *ans= 0;
  try
    {
      ans = ACE_OS::strdup (
        Diamond::TAO_Diamond_Top_Smart_Proxy_Base::shape ());

      ACE_DEBUG ((LM_DEBUG, "%C\n", ans));
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Uncaught Exception");
      return 0;
    }

  return ans;
}
