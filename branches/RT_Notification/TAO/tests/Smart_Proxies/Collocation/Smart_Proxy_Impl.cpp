// $Id$

#include "Smart_Proxy_Impl.h"

Smart_Diamond_Top_Factory::Smart_Diamond_Top_Factory (void)
{
  ACE_DEBUG ((LM_DEBUG, "Smart_Diamond_Top_Factory\n"));
}

Diamond::Top_ptr
Smart_Diamond_Top_Factory::create_proxy (Diamond::Top_ptr proxy
                                                 ACE_ENV_ARG_DECL_NOT_USED
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
Smart_Diamond_Top_Proxy::shape (
                                                                ACE_ENV_SINGLE_ARG_DECL
                                                                )
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
{
  ACE_DEBUG ((LM_DEBUG, "Smart_proxy: This is "));

  CORBA::Short retval = 0;
  char *ans= 0;
  ACE_TRY
    {
          ans = ACE_OS::strdup (TAO_Diamond_Top_Smart_Proxy_Base::shape(ACE_ENV_SINGLE_ARG_PARAMETER));

          ACE_TRY_CHECK;

      ACE_DEBUG ((LM_DEBUG, "%s\n", ans));

    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "Uncaught Exception");
      return 0;
    }
  ACE_ENDTRY;

  return ans;
}
