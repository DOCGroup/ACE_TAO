// $Id$

// ============================================================================
//
// = LIBRARY
//   TAO/tests/Param_Test
//
// = FILENAME
//   anyop.cpp
//
// = DESCRIPTION
//
// = AUTHORS
//   Carlos O'Ryan
//
// ============================================================================

#include "param_testC.h"
#include "tao/corba.h"
#include "tao/debug.h"
#include "tao/Stub.h"
#include "ace/Get_Opt.h"

ACE_RCSID(Param_Test, anyop, "$Id$")

int
main (int argc, char *argv[])
{
  int n = 1024;

  ACE_TRY_NEW_ENV
    {
      CORBA::ORB_var orb = CORBA::ORB_init (argc,
                                            argv,
                                            0
                                            ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      ACE_Get_Opt get_opt (argc, argv, "dn:l:");
      int opt;

      while ((opt = get_opt ()) != EOF)
        {
          switch (opt)
            {
            case 'd':
              TAO_debug_level++;
              break;
            case 'n':
              n = ACE_OS::atoi (get_opt.opt_arg ());
              break;
            case '?':
            default:
              ACE_DEBUG ((LM_DEBUG,
                          "Usage: %s "
                          "-d debug"
                          "-n <num> "
                          "\n",
                          argv[0]));
              return -1;
            }
        }

      for (int i = 0; i != n; ++i)
        {
          CORBA::Any any;

#if 0
          // @@ TODO @@ This one crashes in deep_free!!!
          {
            Param_Test::Var_Array var_array;
            any <<= Param_Test::Var_Array_forany (var_array);

            Param_Test::Var_Array_forany forany;
            if (!(any >>= forany))
              {
                ACE_DEBUG ((LM_DEBUG,
                            "Failure for Param_Test::Var_Array\n"));
              }
            Param_Test::Var_Array_var var =
              Param_Test::Var_Array_dup (forany.in ());
            any <<= Param_Test::Var_Array_forany (var.inout ());
            if (!(any >>= forany))
              {
                ACE_DEBUG ((LM_DEBUG,
                            "Failure for Param_Test::Var_Array[2]\n"));
              }
          }
#endif /* 0 */

          {
            CORBA::Object_var obj =
              orb->string_to_object ("corbaloc:iiop:localhost:1234/Foo/Bar"
                                     ACE_ENV_ARG_PARAMETER);
            ACE_TRY_CHECK;

            Param_Test_var param_test =
              Param_Test::_unchecked_narrow (obj.in ()
                                             ACE_ENV_ARG_PARAMETER);
            ACE_TRY_CHECK;
            TAO_Stub *stub = param_test->_stubobj ();
            stub->type_id = CORBA::string_dup ("IDL:Param_Test:1.0");

            any <<= param_test.in ();

            Param_Test_ptr o;
            if (!(any >>= o))
              {
                ACE_DEBUG ((LM_DEBUG,
                            "Cannot extract Param_Test (oh the horror)\n"));
              }
            CORBA::Boolean equiv =
              param_test->_is_equivalent (o ACE_ENV_ARG_PARAMETER);
            ACE_TRY_CHECK;
            if (!equiv)
              {
                ACE_DEBUG ((LM_DEBUG,
                            "Mismatched Param_Test extraction\n"));
              }

            CORBA::Object_var other;
            if (!(any >>= CORBA::Any::to_object (other.inout ())))
              {
                ACE_DEBUG ((LM_DEBUG,
                            "Cannot extract Param_Test as Object\n"));
              }
          }

          {
            CORBA::Short i = 123;
            any <<= i;

            CORBA::Short o;
            if (!(any >>= o)
                || i != o)
              {
                ACE_DEBUG ((LM_DEBUG,
                            "Failure for CORBA::Short (%d,%d)\n",
                            i, o));
              }
          }

          {
            CORBA::Long i = 123;
            any <<= i;

            CORBA::Long o;
            if (!(any >>= o)
                || i != o)
              {
                ACE_DEBUG ((LM_DEBUG,
                            "Failure for CORBA::Long (%d,%d)\n",
                            i, o));
              }
          }

          {
            CORBA::ULongLong i = 123;
            any <<= i;

            CORBA::ULongLong o;
            if (!(any >>= o)
                || i != o)
              {
#if defined (ACE_LACKS_LONGLONG_T)
                char bufferi[32];
                char buffero[32];
                ACE_DEBUG ((LM_DEBUG,
                            "Failure for CORBA::ULongLong (%s,%s)\n",
                            i.as_string (bufferi),
                            o.as_string (buffero)));
#else
                ACE_DEBUG ((LM_DEBUG,
                            "Failure for CORBA::ULongLong (%Q,%Q)\n",
                            i, o));
#endif
              }
          }

          {
            CORBA::Double i = 123;
            any <<= i;

            CORBA::Double o;
            if (!(any >>= o)
                || i != o)
              {
                ACE_DEBUG ((LM_DEBUG,
                            "Failure for CORBA::Double (%f,%f)\n",
                            i, o));
              }
          }

          {
            CORBA::Any i;
            i <<= CORBA::Short (123);
            any <<= i;

            const CORBA::Any *o;
            CORBA::Short oo;

            if (!(any >>= o)
                || !(*o >>= oo)
                || 123 != oo)
              {
                ACE_DEBUG ((LM_DEBUG,
                            "Failure for CORBA::Any (%d)\n",
                            oo));
              }
          }

          {
            const char i[] = "123";
            any <<= i;

            const char *o;
            if (!(any >>= o)
                || ACE_OS::strcmp (i, o) != 0)
              {
                ACE_DEBUG ((LM_DEBUG,
                            "Failure for char* (%s,%s)\n",
                            i, o));
              }
          }

        }
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "Basic_Types");
      return 1;
    }
  ACE_ENDTRY;

  return 0;
}
