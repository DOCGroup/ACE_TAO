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

#include "tao/debug.h"
#include "ace/OS_NS_string.h"

// Not normally needed, but we create an object reference in this test,
// and we have to narrow it.
#include "tao/Object_T.h"
#include "tao/Stub.h"

#include "ace/Get_Opt.h"

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  int n = 1024;

  try
    {
      CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);

      ACE_Get_Opt get_opt (argc, argv, ACE_TEXT("dn:"));
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

          {
            CORBA::Object_var obj =
              orb->string_to_object ("corbaloc:iiop:localhost:1234/Foo/Bar");

            Param_Test_var param_test =
              TAO::Narrow_Utils<Param_Test>::unchecked_narrow (
                  obj.in (),
                  _TAO_Param_Test_Proxy_Broker_Factory_function_pointer
                );
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
              param_test->_is_equivalent (o);

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
                            "Failure for CORBA::ULongLong (%C,%C)\n",
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

            if (!(any >>= o) || !ACE::is_equal (i, o))
              {
                ACE_DEBUG ((LM_DEBUG,
                            "Failure for CORBA::Double (%f,%f)\n",
                            i, o));
              }
          }

          {
            CORBA::Any any;
            CORBA::Any *i = 0;
            ACE_NEW_RETURN (i,
                            CORBA::Any,
                            -1);
            *i <<= CORBA::Short (123);
            any <<= *i;

            const CORBA::Any *o;
            CORBA::Short oo;

            if (!(any >>= o)
                || !(*o >>= oo)
                || 123 != oo)
              {
                ACE_DEBUG ((LM_DEBUG,
                            "Failure for CORBA::Any "
                            "(copying insertion, %d)\n",
                            oo));
              }

            any <<= i;

            if (!(any >>= o)
                || !(*o >>= oo)
                || 123 != oo)
              {
                ACE_DEBUG ((LM_DEBUG,
                            "Failure for CORBA::Any "
                            "(non-copying insertion, %d)\n",
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
                            "Failure for char* (%C,%C)\n",
                            i,
                            o));
              }
          }

          {
            CORBA::Any any;
            Param_Test::Fixed_Struct *i = 0;
            ACE_NEW_RETURN (i,
                            Param_Test::Fixed_Struct,
                            -1);
            i->l = -7;
            i->c = 'c';
            i->s = 5;
            i->o = 255;
            i->f = 2.3f;
            i->b = 0;
            i->d = 3.1416;

            any <<= *i;
            Param_Test::Fixed_Struct *o;

            if (!(any >>= o)
                || o->l != i->l
                || o->c != i->c
                || o->s != i->s
                || o->o != i->o
                || !ACE::is_equal (o->f, i->f)
                || o->b != i->b
                || !ACE::is_equal (o->d, i->d))
              {
                ACE_DEBUG ((LM_DEBUG,
                            "Failure for Fixed_Struct "
                            "(copying insertion)\n"));
              }

            any <<= i;

            if (!(any >>= o)
                || o->l != i->l
                || o->c != i->c
                || o->s != i->s
                || o->o != i->o
                || !ACE::is_equal (o->f, i->f)
                || o->b != i->b
                || !ACE::is_equal (o->d, i->d))
              {
                ACE_DEBUG ((LM_DEBUG,
                            "Failure for Fixed_Struct "
                            "(non-copying insertion)\n"));
              }
          }

          {
            CORBA::ULong len = 3;
            CORBA::Any any;
            CORBA::LongSeq *i = 0;
            ACE_NEW_RETURN (i,
                            CORBA::LongSeq (len),
                            -1);
            i->length (len);

            for (CORBA::ULong k = 0; k < len; ++k)
              {
                (*i)[k] = k;
              }

            any <<= *i;
            CORBA::LongSeq *o;

            if (!(any >>= o)
                || (*i)[0] != (*o)[0]
                || (*i)[1] != (*o)[1]
                || (*i)[2] != (*o)[2])
              {
                ACE_DEBUG ((LM_DEBUG,
                            "Failure for Long_Seq "
                            "(copying insertion)\n"));
              }

            any <<= i;

            if (!(any >>= o)
                || (*i)[0] != (*o)[0]
                || (*i)[1] != (*o)[1]
                || (*i)[2] != (*o)[2])
              {
                ACE_DEBUG ((LM_DEBUG,
                            "Failure for Long_Seq "
                            "(non-copying insertion)\n"));
              }
          }
        }
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("IDL Types");
      return 1;
    }

  return 0;
}
