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
#include "unionC.h"
#include "repo_id_modC.h"

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

class schmooze_i : public virtual gleep::schmooze
{
};

class schmeer_i : public virtual gleep::schmeer
{
};

class schlemiel_i : public virtual gleep::schlemiel
{
};

class spilkis_i : public virtual gleep::spilkis
{
};

class schmuck_i : public virtual gleep::floop::schmuck
{
};

class schmendrick_i : public virtual gleep::floop::schmendrick
{
};

class schlemazel_i : public virtual gleep::floop::verklempt::schlemazel
{
};

class schmegegging_i : public virtual gleep::schmegegging
{
};

int
main (int argc , char *argv[])
{
  int error_count = 0;

  ACE_TRY_NEW_ENV
    {
      CORBA::ORB_var orb = CORBA::ORB_init (argc,
                                            argv,
                                            ""
                                            ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CORBA::Object_var poa_object =
        orb->resolve_initial_references ("RootPOA"
                                         ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (poa_object.in ()
                                      ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Test of #pragma prefix behavior.

      CORBA::Object_var obj;

      hello_i h;
      obj = h._this (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (ACE_OS::strcmp (obj->_interface_repository_id (),
                          "IDL:anvil.com/hello:1.0"))
        {
          ++error_count;
          ACE_DEBUG ((LM_DEBUG,
                      "pragma prefix error in object 'hello'\n"));
        }

      goodbye_i g;
      obj = g._this (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (ACE_OS::strcmp (obj->_interface_repository_id (),
                          "IDL:anvil.com/goodbye:1.0"))
        {
          ++error_count;
          ACE_DEBUG ((LM_DEBUG,
                      "pragma prefix error in object 'goodbye'\n"));
        }

      sayonara_i s;
      obj = s._this (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (ACE_OS::strcmp (obj->_interface_repository_id (),
                          "IDL:hammer.com/salutation/sayonara:1.0"))
        {
          ++error_count;
          ACE_DEBUG ((LM_DEBUG,
                      "pragma prefix error in object 'sayonara'\n"));
        }

      ciao_i c;
      obj = c._this (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (ACE_OS::strcmp (obj->_interface_repository_id (),
                          "IDL:anvil.com/ciao:1.0"))
        {
          ++error_count;
          ACE_DEBUG ((LM_DEBUG,
                      "pragma prefix error in object 'ciao'\n"));
        }

      aloha_i a;
      obj = a._this (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (ACE_OS::strcmp (obj->_interface_repository_id (),
                          "IDL:anvil.com/aloha:1.0"))
        {
          ++error_count;
          ACE_DEBUG ((LM_DEBUG,
                      "pragma prefix error in object 'aloha'\n"));
        }

      // Test of typeprefix, typeid, and #pragma version behavior.

      schmooze_i s_schmooze;

      if (ACE_OS::strcmp (s_schmooze._interface_repository_id (),
                          "IDL:gleep_prefix/gleep/schmooze:1.0"))
        {
          ++error_count;
          ACE_DEBUG ((LM_DEBUG,
                      "pragma prefix error in object 'schmooze'\n"));
        }

      schmeer_i s_schmeer;

      if (ACE_OS::strcmp (s_schmeer._interface_repository_id (),
                          "IDL:gleep_prefix/gleep/schmeer:1.0"))
        {
          ++error_count;
          ACE_DEBUG ((LM_DEBUG,
                      "pragma prefix error in object 'schmeer'\n"));
        }

      schlemiel_i s_schlemiel;

      if (ACE_OS::strcmp (s_schlemiel._interface_repository_id (),
                          "IDL:gleep_prefix/gleep/schlemiel:1.0"))
        {
          ++error_count;
          ACE_DEBUG ((LM_DEBUG,
                      "pragma prefix error in object 'schlemiel'\n"));
        }

      spilkis_i s_spilkis;

      if (ACE_OS::strcmp (s_spilkis._interface_repository_id (),
                          "IDL:gleep_prefix/gleep/spilkis:1.0"))
        {
          ++error_count;
          ACE_DEBUG ((LM_DEBUG,
                      "pragma prefix error in object 'spilkis'\n"));
        }

      schmuck_i s_schmuck;

      if (ACE_OS::strcmp (s_schmuck._interface_repository_id (),
                          "ABRA:cadabra/hocus/pocus:1.23"))
        {
          ++error_count;
          ACE_DEBUG ((LM_DEBUG,
                      "pragma prefix error in object 'schmuck'\n"));
        }

      schmendrick_i s_schmendrick;

      if (ACE_OS::strcmp (s_schmendrick._interface_repository_id (),
                          "IDL:floop_prefix/gleep/floop/schmendrick:524.23"))
        {
         cout << s_schmendrick._interface_repository_id () << endl;
          ++error_count;
          ACE_DEBUG ((LM_DEBUG,
                      "pragma prefix error in object 'schmendrick'\n"));
        }

      schlemazel_i s_schlemazel;

      if (ACE_OS::strcmp (
              s_schlemazel._interface_repository_id (),
              "IDL:verklempt_prefix/gleep/floop/verklempt/schlemazel:1.0"
            ))
        {
          ++error_count;
          ACE_DEBUG ((LM_DEBUG,
                      "pragma prefix error in object 'schlemazel'\n"));
        }

      schmegegging_i s_schmegegging;

      if (ACE_OS::strcmp (s_schmegegging._interface_repository_id (),
                          "IDL:gleep_prefix/gleep/schmegegging:1.0"))
        {
          ++error_count;
          ACE_DEBUG ((LM_DEBUG,
                      "pragma prefix error in object 'schmegegging'\n"));
        }

      // Testing (de)marshaling of IDL union values 
      // under duplicate and default case labels.

      Field field;
      field.value.strValue (
          CORBA::string_dup ("duplicate case label test string")
        );
      field.value._d (FTYPE_VARCHAR);
      CORBA::Any any1;
      any1 <<= field;
      Field *outfield;

      if ((any1 >>= outfield) == 0)
        {
          ++error_count;
          ACE_DEBUG ((LM_DEBUG,
                      "error in extraction of "
                      "duplicate case label value\n"));
        }

      const char *str = outfield->value.strValue ();
      
      if (ACE_OS::strcmp (str, "duplicate case label test string") != 0)
        {
          ++error_count;
          ACE_DEBUG ((LM_DEBUG,
                      "error - corruption of "
                      "duplicate case label value\n"));
        }

      field.value.defstr (CORBA::string_dup ("default case test string"));
      any1 <<= field;

      if ((any1 >>= outfield) == 0)
        {
          ++error_count;
          ACE_DEBUG ((LM_DEBUG,
                      "error in extraction of "
                      "default case label value\n"));
        }

      str = outfield->value.defstr ();
      
      if (ACE_OS::strcmp (str, "default case test string") != 0)
        {
          ++error_count;
          ACE_DEBUG ((LM_DEBUG,
                      "error - corruption of "
                      "default case label value\n"));
        }

      root_poa->destroy (1,
                         1
                         ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Unexpected exception in main");
      return 1;
    }
  ACE_ENDTRY;

  return error_count;
}
