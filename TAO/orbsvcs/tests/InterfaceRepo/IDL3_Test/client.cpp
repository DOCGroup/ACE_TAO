// -*- C++ -*-
// $Id$

#include "idl3_client.h"

ACE_RCSID (Application_Test, 
           client, 
           "$Id$")
/*
static const CORBA::Long _oc_mod_test_component[] =
{
    TAO_ENCAP_BYTE_ORDER, // byte order
  27,
  ACE_NTOHL (0x49444c3a), 
  ACE_NTOHL (0x6d6f642f), 
  ACE_NTOHL (0x74657374), 
  ACE_NTOHL (0x5f636f6d), 
  ACE_NTOHL (0x706f6e65), 
  ACE_NTOHL (0x6e743a31), 
  ACE_NTOHL (0x2e300000),  // repository ID = IDL:mod/test_component:1.0
    15,
  ACE_NTOHL (0x74657374), 
  ACE_NTOHL (0x5f636f6d), 
  ACE_NTOHL (0x706f6e65), 
  ACE_NTOHL (0x6e740000),  // name = test_component
  };

static CORBA::TypeCode _tc_TAO_tc_mod_test_component (
    CORBA::tk_component,
    sizeof (_oc_mod_test_component),
    (char *) &_oc_mod_test_component,
    0,
    0//sizeof (mod::test_component)
  );

TAO_NAMESPACE_TYPE (CORBA::TypeCode_ptr)
TAO_NAMESPACE_BEGIN (mod)
TAO_NAMESPACE_DEFINE (
    ::CORBA::TypeCode_ptr,
    _tc_test_component,
    &_tc_TAO_tc_mod_test_component
  )
TAO_NAMESPACE_END
*/
int
main (int argc, char *argv[])
{
  IDL3_Client client;

  ACE_TRY_NEW_ENV
    {
      if (client.init (argc,
                       argv
                       ACE_ENV_ARG_PARAMETER)
           == -1)
        {
          return 1;
        }
      else
        {
           int status = client.run (ACE_ENV_SINGLE_ARG_PARAMETER);
           ACE_TRY_CHECK;

           if (status == -1)
             {
               return 1;
             }
        }
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Client Exception");
      return -1;
    }
  ACE_ENDTRY;

  return 0;
}
