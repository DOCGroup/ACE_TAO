// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/tests/IDL_Test
//
// = FILENAME
//    main.cpp
//
// = DESCRIPTION
//    The only things that needs to be tested in execution
//    are the pragma prefixes generated in pragma.idl, so
//    we check them here. The rest needs only to build cleanly
//
// = AUTHORS
//    Jeff Parsons <parsons@cs.wustl.edu>
//
// ============================================================================

#include "ace/Log_Msg.h"
#include "pragmaS.h"

class hello_i : public virtual POA_hello
{
};

class goodbye_i : public virtual POA_goodbye
{
};

class sayonara_i : public virtual POA_salutation::sayonara
{
};

class ciao_i : public virtual POA_ciao
{
};

class aloha_i : public virtual POA_aloha
{
};

int 
main (int, char *[])
{
  ACE_TRY_NEW_ENV
    {
      CORBA::ULong error_count = 0;

      CORBA::Object_var obj;

      hello_i h;

      obj = h._this (ACE_TRY_ENV);

      ACE_TRY_CHECK;

      if (ACE_OS::strcmp (obj->_interface_repository_id (),
                          "IDL:anvil.com/hello:1.0"))
        {
          ACE_DEBUG ((LM_DEBUG,
                      "pragma prefix error in object 'hello'\n"));

          error_count++;
        }

      goodbye_i g;

      obj = g._this (ACE_TRY_ENV);

      ACE_TRY_CHECK;

      if (ACE_OS::strcmp (obj->_interface_repository_id (),
                          "IDL:anvil.com/goodbye:1.0"))
        {          
          ACE_DEBUG ((LM_DEBUG,
                      "pragma prefix error in object 'goodbye'\n"));

          error_count++;
        }

      sayonara_i s;

      obj = s._this (ACE_TRY_ENV);

      ACE_TRY_CHECK;

      if (ACE_OS::strcmp (obj->_interface_repository_id (),
                          "IDL:hammer.com/salutation/sayonara:1.0"))
        {          
          ACE_DEBUG ((LM_DEBUG,
                      "pragma prefix error in object 'sayonara'\n"));

          error_count++;
        }

      ciao_i c;

      obj = c._this (ACE_TRY_ENV);

      ACE_TRY_CHECK;

      if (ACE_OS::strcmp (obj->_interface_repository_id (),
                          "IDL:anvil.com/ciao:1.0"))
        {          
          ACE_DEBUG ((LM_DEBUG,
                      "pragma prefix error in object 'ciao'\n"));

          error_count++;
        }

      aloha_i a;

      obj = a._this (ACE_TRY_ENV);

      ACE_TRY_CHECK;

      if (ACE_OS::strcmp (obj->_interface_repository_id (),
                          "IDL:anvil.com/aloha:1.0"))
        {          
          ACE_DEBUG ((LM_DEBUG,
                      "pragma prefix error in object 'aloha'\n"));

          error_count++;
        }

      ACE_DEBUG ((LM_DEBUG,
                 "Pragma prefix test completed with %d errors\n"
                 "Check compiler output for build warnings\n",
                 error_count));

    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, 
                           "Unexpected exception in main");
      return 1;
    }
  ACE_ENDTRY;

  return 0;
}
