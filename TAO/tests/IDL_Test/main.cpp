
//=============================================================================
/**
 *  @file    main.cpp
 *
 *  We test the pragma prefixes generated in pragma.idl, and
 *  the values of some generated constants in constants.idl.
 *
 *  @author Jeff Parsons <parsons@cs.wustl.edu>
 */
//=============================================================================


#include "pragmaS.h"
#include "unionC.h"
#include "repo_id_modC.h"
#include "constantsC.h"
#include "nested_scopeS.h"
#include "typedefC.h"
#include "expressionsC.h"

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

template <typename Type>
void
expect_equals (int &error_count, const char *name, Type actual, Type expected)
{
  if (actual != expected)
    {
      *ACE_DEFAULT_LOG_STREAM
        << "ERROR: For " << name << " expected: " << expected
        << ", but got " << actual << "\n";
      ++error_count;
    }
}

void
test_expressions (int &error_count)
{
  expect_equals<CORBA::Short> (error_count, "ShortValues::a", ShortValues::a, 6);
  expect_equals<CORBA::Short> (error_count, "ShortValues::b", ShortValues::b, 3);
  expect_equals<CORBA::Short> (error_count, "ShortValues::div", ShortValues::div, 2);
  expect_equals<CORBA::Short> (error_count, "ShortValues::mul", ShortValues::mul, 18);
  expect_equals<CORBA::Short> (error_count, "ShortValues::add", ShortValues::add, 9);
  expect_equals<CORBA::Short> (error_count, "ShortValues::sub", ShortValues::sub, 3);
  expect_equals<CORBA::Short> (error_count, "ShortValues::mod", ShortValues::mod, 0);

  expect_equals<CORBA::Long> (error_count, "LongValues::a", LongValues::a, 6);
  expect_equals<CORBA::Long> (error_count, "LongValues::b", LongValues::b, 3);
  expect_equals<CORBA::Long> (error_count, "LongValues::div", LongValues::div, 2);
  expect_equals<CORBA::Long> (error_count, "LongValues::mul", LongValues::mul, 18);
  expect_equals<CORBA::Long> (error_count, "LongValues::add", LongValues::add, 9);
  expect_equals<CORBA::Long> (error_count, "LongValues::sub", LongValues::sub, 3);
  expect_equals<CORBA::Long> (error_count, "LongValues::mod", LongValues::mod, 0);

  expect_equals<CORBA::Long> (error_count, "MixedIntValues::div", MixedIntValues::div, 2);
  expect_equals<CORBA::Long> (error_count, "MixedIntValues::mul", MixedIntValues::mul, 18);
  expect_equals<CORBA::Long> (error_count, "MixedIntValues::add", MixedIntValues::add, 9);
  expect_equals<CORBA::Long> (error_count, "MixedIntValues::sub", MixedIntValues::sub, 3);
  expect_equals<CORBA::Long> (error_count, "MixedIntValues::mod", MixedIntValues::mod, 0);

  expect_equals<CORBA::Float> (error_count, "FloatValues::a", FloatValues::a, 6.0f);
  expect_equals<CORBA::Float> (error_count, "FloatValues::b", FloatValues::b, 3.0f);
  expect_equals<CORBA::Float> (error_count, "FloatValues::div", FloatValues::div, 2.0f);
  expect_equals<CORBA::Float> (error_count, "FloatValues::mul", FloatValues::mul, 18.0f);
  expect_equals<CORBA::Float> (error_count, "FloatValues::add", FloatValues::add, 9.0f);
  expect_equals<CORBA::Float> (error_count, "FloatValues::sub", FloatValues::sub, 3.0f);

  expect_equals<CORBA::Double> (error_count, "DoubleValues::a", DoubleValues::a, 6.0);
  expect_equals<CORBA::Double> (error_count, "DoubleValues::b", DoubleValues::b, 3.0);
  expect_equals<CORBA::Double> (error_count, "DoubleValues::div", DoubleValues::div, 2.0);
  expect_equals<CORBA::Double> (error_count, "DoubleValues::mul", DoubleValues::mul, 18.0);
  expect_equals<CORBA::Double> (error_count, "DoubleValues::add", DoubleValues::add, 9.0);
  expect_equals<CORBA::Double> (error_count, "DoubleValues::sub", DoubleValues::sub, 3.0);

  expect_equals<CORBA::Double> (error_count, "MixedFloatValues::div", MixedFloatValues::div, 2.0);
  expect_equals<CORBA::Double> (error_count, "MixedFloatValues::mul", MixedFloatValues::mul, 18.0);
  expect_equals<CORBA::Double> (error_count, "MixedFloatValues::add", MixedFloatValues::add, 9.0);
  expect_equals<CORBA::Double> (error_count, "MixedFloatValues::sub", MixedFloatValues::sub, 3.0);
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  int error_count = 0;

  const ACE_CDR::ULongLong test_ull =
    ACE_UINT64_LITERAL (122192928000000000);

  if (test_ull != AAA)
    {
      ++error_count;
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("Generated value of unsigned")
                  ACE_TEXT (" long long AAA in constants.idl")
                  ACE_TEXT (" is incorrect\n")));
    }

  const CORBA::LongLong test_nll = ACE_INT64_LITERAL (-122192928000000000);

  if (test_nll != NAAA)
    {
      ++error_count;
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("Generated value of signed")
                  ACE_TEXT (" long long NAAA in constants.idl")
                  ACE_TEXT (" is incorrect\n")));
    }

  const CORBA::LongLong test_pll = ACE_INT64_LITERAL (122192928000000000);

  if (test_pll != PAAA)
    {
      ++error_count;
      ACE_ERROR ((LM_ERROR,
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
          ACE_ERROR ((LM_ERROR,
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
          ACE_ERROR ((LM_ERROR,
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
          ACE_ERROR ((LM_ERROR,
                      "pragma prefix error in object 'sayonara'\n"));
        }

      // Check whether the implementation of Dubble and Toil are generated
      CommaList::Dubble dub;
      CommaList::Toil toi;
      dub.length (4);
      toi.length (4);

      ciao_i c;
      id = root_poa->activate_object (&c);
      obj = root_poa->id_to_reference (id.in ());
      obj = ciao::_narrow (obj.in ());

      if (ACE_OS::strcmp (obj->_interface_repository_id (),
                          "IDL:anvil.com/ciao:1.0"))
        {
          ++error_count;
          ACE_ERROR ((LM_ERROR,
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
          ACE_ERROR ((LM_ERROR,
                      "pragma prefix error in object 'aloha'\n"));
        }

      // Test of typeprefix, typeid, and #pragma version behavior.

      schmooze_i s_schmooze;

      if (ACE_OS::strcmp (s_schmooze._interface_repository_id (),
                          "IDL:gleep_prefix/gleep/schmooze:1.0"))
        {
          ++error_count;
          ACE_ERROR ((LM_ERROR,
                      "pragma prefix error in object 'schmooze'\n"));
        }

      schmeer_i s_schmeer;

      if (ACE_OS::strcmp (s_schmeer._interface_repository_id (),
                          "IDL:gleep_prefix/gleep/schmeer:1.0"))
        {
          ++error_count;
          ACE_ERROR ((LM_ERROR,
                      "pragma prefix error in object 'schmeer'\n"));
        }

      schlemiel_i s_schlemiel;

      if (ACE_OS::strcmp (s_schlemiel._interface_repository_id (),
                          "IDL:gleep_prefix/gleep/schlemiel:1.0"))
        {
          ++error_count;
          ACE_ERROR ((LM_ERROR,
                      "pragma prefix error in object 'schlemiel'\n"));
        }

      spilkis_i s_spilkis;

      if (ACE_OS::strcmp (s_spilkis._interface_repository_id (),
                          "IDL:gleep_prefix/gleep/spilkis:1.0"))
        {
          ++error_count;
          ACE_ERROR ((LM_ERROR,
                      "pragma prefix error in object 'spilkis'\n"));
        }

      schmuck_i s_schmuck;

      if (ACE_OS::strcmp (s_schmuck._interface_repository_id (),
                          "ABRA:cadabra/hocus/pocus:1.23"))
        {
          ++error_count;
          ACE_ERROR ((LM_ERROR,
                      "pragma prefix error in object 'schmuck'\n"));
        }

      schmendrick_i s_schmendrick;

      if (ACE_OS::strcmp (s_schmendrick._interface_repository_id (),
                          "IDL:floop_prefix/gleep/floop/schmendrick:524.23"))
        {
          ++error_count;
          ACE_ERROR ((LM_ERROR,
                      "pragma prefix error in object 'schmendrick'\n"));
        }
      if (ACE_OS::strcmp (gleep::floop::schmendrick::_desc_repository_id (),
                          "IDL:floop_prefix/gleep/floop/schmendrick:524.23"))
        {
          ++error_count;
          ACE_ERROR ((LM_ERROR,
                      "error in _desc_repository_id 'schmendrick'\n"));
        }
      if (ACE_OS::strcmp (gleep::floop::schmendrick::_desc_interface_name (),
                          "schmendrick"))
        {
          ++error_count;
          ACE_ERROR ((LM_ERROR,
                      "error in _desc_interface_name 'schmendrick'\n"));
        }

      schlemazel_i s_schlemazel;

      if (ACE_OS::strcmp (
              s_schlemazel._interface_repository_id (),
              "IDL:verklempt_prefix/gleep/floop/verklempt/schlemazel:1.0"
            ))
        {
          ++error_count;
          ACE_ERROR ((LM_ERROR,
                      "pragma prefix error in object 'schlemazel'\n"));
        }

      schmegegging_i s_schmegegging;

      if (ACE_OS::strcmp (s_schmegegging._interface_repository_id (),
                          "IDL:gleep_prefix/gleep/schmegegging:1.0"))
        {
          ++error_count;
          ACE_ERROR ((LM_ERROR,
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

        for (size_t i = 0; i != sizeof (base)/sizeof (base[0]); ++i)
          {
            if (!x._is_a (base[i]))
              {
                ++error_count;
                ACE_ERROR ((LM_ERROR,
                           "something_handler::_is_a should return true for %C\n",
                           base[i]));
              }
          }

        if (!dynamic_cast<POA_bug_1985_c::d::AMI_somethingHandler*> (&x))
          {
            ++error_count;
            ACE_ERROR ((LM_ERROR,
                       "mismatch in downcast for %C\n",
                        base[0]));
          }

        if (!dynamic_cast<POA_bug_1985_a::b::AMI_somethingHandler*> (&x))
          {
            ++error_count;
            ACE_ERROR ((LM_ERROR,
                       "mismatch in downcast for %C\n",
                       base[1]));
          }

        if (!dynamic_cast<POA_Messaging::ReplyHandler*> (&x))
          {
            ++error_count;
            ACE_ERROR ((LM_ERROR,
                       "mismatch in downcast for %C\n",
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
      const Field *outfield = 0;

      if ((any1 >>= outfield) == 0)
        {
          ++error_count;
          ACE_ERROR ((LM_ERROR,
                      "error in extraction of "
                      "duplicate case label value\n"));
        }

      const char *str = outfield->value.strValue ();

      if (ACE_OS::strcmp (str, "duplicate case label test string") != 0)
        {
          ++error_count;
          ACE_ERROR ((LM_ERROR,
                      "error - corruption of "
                      "duplicate case label value\n"));
        }

      field.value.defstr (CORBA::string_dup ("default case test string"));
      any1 <<= field;

      if ((any1 >>= outfield) == 0)
        {
          ++error_count;
          ACE_ERROR ((LM_ERROR,
                      "error in extraction of "
                      "default case label value\n"));
        }

      str = outfield->value.defstr ();

      if (ACE_OS::strcmp (str, "default case test string") != 0)
        {
          ++error_count;
          ACE_ERROR ((LM_ERROR,
                      "error - corruption of "
                      "default case label value\n"));
        }

      if (SignedGen::val !=  -3)
        {
          ++error_count;
          ACE_ERROR ((LM_ERROR,
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

  test_expressions (error_count);

  return error_count ? 1 : 0;
}
