// $Id$

#include "Smart_Proxy_Impl.h"

smart_test_factory::smart_test_factory (void)
{
  ACE_DEBUG ((LM_DEBUG, "smart_test_factory\n"));
}

test_ptr
smart_test_factory::create_proxy (test_ptr proxy,
                                  CORBA::Environment &
                                  )
 {
   ACE_DEBUG ((LM_DEBUG, "create_smart_proxy\n"));
   return (!CORBA::is_nil (proxy) ? new smart_test_proxy (proxy) : proxy);
   
 }

smart_test_proxy::smart_test_proxy (test_ptr proxy)
  : TAO_test_Smart_Proxy_Base (proxy)
{
}

CORBA::Short 
smart_test_proxy::method (
                     CORBA::Short boo,
                     CORBA::Environment &ACE_TRY_ENV 
                     )
  ACE_THROW_SPEC ((
                   CORBA::SystemException,
                   test::Oops
                   ))
{
  ACE_DEBUG ((LM_DEBUG, "Yahoo I am smart\n"));
  
  CORBA::Short retval = 0;
  ACE_TRY 
    {
      retval = TAO_test_Smart_Proxy_Base::method (boo, ACE_TRY_ENV);
      ACE_TRY_CHECK;
    }
  ACE_CATCH (test::Oops, reason)
    {
      ACE_PRINT_EXCEPTION (reason, "User Exception");
      return -1;
    }
  ACE_ENDTRY;
  
  return retval;
}
