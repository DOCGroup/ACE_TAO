
//=============================================================================
/**
 *  @file    main.cpp
 *
 *  $Id$
 *
 *  We test the pragma prefixes generated in pragma.idl, and
 *  the values of some generated constants in constants.idl.
 *
 *
 *  @author Jeff Parsons <parsons@cs.wustl.edu>
 */
//=============================================================================


#include "pragmaS.h"
#include "unionC.h"
#include "repo_id_modC.h"
#include "constantsC.h"
#include "nested_scopeS.h"

#include "ace/Log_Msg.h"
#include "ace/OS_NS_string.h"
#include "tao/AnyTypeCode/Any.h"

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

struct something_handler
  : public POA_bug_1985_c::d::AMI_somethingHandler
{
};

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  int error_count = 0;

  const ACE_CDR::ULongLong test_ull =
    ACE_UINT64_LITERAL (122192928000000000);

  if (test_ull != AAA)
    {
      ++error_count;
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("Generated value of unsigned")
                  ACE_TEXT (" long long AAA in constants.idl")
                  ACE_TEXT (" is incorrect\n")));
    }

  const CORBA::LongLong test_nll = ACE_INT64_LITERAL (-122192928000000000);

  if (test_nll != NAAA)
    {
      ++error_count;
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("Generated value of signed")
                  ACE_TEXT (" long long NAAA in constants.idl")
                  ACE_TEXT (" is incorrect\n")));
    }

  const CORBA::LongLong test_pll = ACE_INT64_LITERAL (122192928000000000);

  if (test_pll != PAAA)
    {
      ++error_count;
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("Generated value of signed")
                  ACE_TEXT (" long long PAAA in constants.idl")
                  ACE_TEXT (" is incorrect\n")));
    }

  try
    {
      CORBA::ORB_var orb = CORBA::ORB_init (argc,
                                            argv);

      CORBA::Object_var poa_object =
        orb->resolve_initial_references ("RootPOA");

      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (poa_object.in ());

      // Test of #pragma prefix behavior.

      CORBA::Object_var obj;

      hello_i h;
      PortableServer::ObjectId_var id = root_poa->activate_object (&h);
      obj = root_poa->id_to_reference (id.in ());
      obj = hello::_narrow (obj.in ());

      if (ACE_OS::strcmp (obj->_interface_repository_id (),
                          "IDL:anvil.com/hello:1.0"))
        {
          ++error_count;
          ACE_DEBUG ((LM_DEBUG,
                      "pragma prefix error in object 'hello'\n"));
        }

      goodbye_i g;
      id = root_poa->activate_object (&g);
      obj = root_poa->id_to_reference (id.in ());
      obj = goodbye::_narrow (obj.in ());

      if (ACE_OS::strcmp (obj->_interface_repository_id (),
                          "IDL:anvil.com/goodbye:1.0"))
        {
          ++error_count;
          ACE_DEBUG ((LM_DEBUG,
                      "pragma prefix error in object 'goodbye'\n"));
        }

      sayonara_i s;
      id = root_poa->activate_object (&s);
      obj = root_poa->id_to_reference (id.in ());
      obj = salutation::sayonara::_narrow (obj.in ());

      if (ACE_OS::strcmp (obj->_interface_repository_id (),
                          "IDL:hammer.com/salutation/sayonara:1.0"))
        {
          ++error_count;
          ACE_DEBUG ((LM_DEBUG,
                      "pragma prefix error in object 'sayonara'\n"));
        }

      ciao_i c;
      id = root_poa->activate_object (&c);
      obj = root_poa->id_to_reference (id.in ());
      obj = ciao::_narrow (obj.in ());

      if (ACE_OS::strcmp (obj->_interface_repository_id (),
                          "IDL:anvil.com/ciao:1.0"))
        {
          ++error_count;
          ACE_DEBUG ((LM_DEBUG,
                      "pragma prefix error in object 'ciao'\n"));
        }

      aloha_i a;
      id = root_poa->activate_object (&a);
      obj = root_poa->id_to_reference (id.in ());
      obj = aloha::_narrow (obj.in ());

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

      {
        something_handler x;
        char const * base[] =
        {
          "IDL:bug_1985_c/d/AMI_somethingHandler:1.0",
          "IDL:bug_1985_a/b/AMI_somethingHandler:1.0",
          "IDL:omg.org/Messaging/ReplyHandler:1.0",
          "IDL:omg.org/CORBA/Object:1.0"
        };

        for (int i = 0; i != sizeof (base)/sizeof (base[0]); ++i)
          {
            if (!x._is_a (base[i]))
              {
                ++error_count;
                ACE_DEBUG ((LM_DEBUG,
                           "something_handler::_is_a should return true for %s\n",
                           base[i]));
              }
          }

        if (!dynamic_cast<POA_bug_1985_c::d::AMI_somethingHandler*> (&x))
          {
            ++error_count;
            ACE_DEBUG( (LM_DEBUG,
                       "mismatch in downcast for %s\n",
                        base[0]));
          }

        if (!dynamic_cast<POA_bug_1985_a::b::AMI_somethingHandler*> (&x))
          {
            ++error_count;
            ACE_DEBUG ((LM_DEBUG,
                       "mismatch in downcast for %s\n",
                       base[1]));
          }

        if (!dynamic_cast<POA_Messaging::ReplyHandler*> (&x))
          {
            ++error_count;
            ACE_DEBUG ((LM_DEBUG,
                       "mismatch in downcast for %s\n",
                       base[2]));
          }
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

      if (SignedGen::val !=  -3)
        {
          ++error_count;
          ACE_DEBUG ((LM_DEBUG,
                      "error - signed integer literal "
                      "generated as unsigned\n"));
        }

      root_poa->destroy (true, true);
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Unexpected exception in main");
      return 1;
    }

  return error_count;
}
