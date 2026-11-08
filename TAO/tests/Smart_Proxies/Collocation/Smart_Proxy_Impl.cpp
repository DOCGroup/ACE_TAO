#include "Smart_Proxy_Impl.h"

#include "ace/Log_Msg.h"

Smart_Diamond_Top_Factory::Smart_Diamond_Top_Factory ()
{
  ACE_DEBUG ((LM_DEBUG, "Smart_Diamond_Top_Factory\n"));
}

Diamond::Top_ptr
Smart_Diamond_Top_Factory::create_proxy (Diamond::Top_ptr proxy)
{
  ACE_DEBUG ((LM_DEBUG, "create_smart_proxy\n"));
  if (!CORBA::is_nil (proxy))
    ACE_NEW_RETURN (proxy, Smart_Diamond_Top_Proxy (proxy), nullptr);
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

  CORBA::String_var answer;
  try
    {
      answer = Diamond::TAO_Diamond_Top_Smart_Proxy_Base::shape ();
      ACE_DEBUG ((LM_DEBUG, "%C\n", answer.in ()));
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("Uncaught Exception");
      return nullptr;
    }

  return answer._retn ();
}
