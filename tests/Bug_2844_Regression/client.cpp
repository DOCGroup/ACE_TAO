// $Id$

#include "TestC.h"
#include "tao/IFR_Client/IFR_BaseC.h"
#include "tao/TypeCodeFactory/TypeCodeFactory_Loader.h"

#include "ace/Get_Opt.h"

#include <algorithm>
#include <functional>

const ACE_TCHAR *ior = ACE_TEXT("file://test.ior");

int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("k:"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'k':
        ior = get_opts.opt_arg ();
        break;

      case '?':
      case 'h':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-k <ior> "
                           "\n",
                           argv [0]),
                          -1);
      }

  // Successful command line parsing.
  return 0;
}

template<typename T> void dump (T *); // Forward declaration.

template<typename T>
void
perform_invocation (Test::Hello_ptr hello,
                    CORBA::Any const & the_any)
{
  // Execute more than once to help verify that mutable recursive
  // TypeCode state is managed correctly.
  for (unsigned int n = 0; n < 2; ++n)
    {
      CORBA::Any_var my_any =
        hello->get_any (the_any);

      T * my_foo = 0;
      if (!(my_any.in () >>= my_foo))
        throw Test::Demarshaling_From_Any_Failed ();

      CORBA::TypeCode_var the_tc = the_any.type ();
      CORBA::TypeCode_var my_tc  = my_any->type ();

      CORBA::Boolean const equal_tc =
        the_tc->equal (my_tc.in ());

      if (!equal_tc)
        throw Test::Recursive_Type_In_Any_Test_Failed ();

      CORBA::Boolean const equiv_tc =
        the_tc->equivalent (my_tc.in ());

      if (!equiv_tc)
        throw Test::Recursive_Type_In_Any_Test_Failed ();
    }
}

void
nested_recursive_struct_test (CORBA::ORB_ptr /* orb */,
                              Test::Hello_ptr hello)
{
  ACE_DEBUG ((LM_INFO,
              "Executing nested recursive struct test\n"));

  CORBA::Any the_any;

  Test::NestedRecursiveStruct5 foo5;

  the_any <<= foo5;

  ::perform_invocation<Test::NestedRecursiveStruct5> (hello, the_any);

}

/**
 * @struct Caller
 *
 * @brief Test method invocation functor.
 *
 * Test method invocation functor.
 */
template <typename T>
struct Caller : public std::unary_function<T, void>
{
  /// Constructor.
  Caller (CORBA::ORB_ptr o, Test::Hello_ptr h)
    : orb (CORBA::ORB::_duplicate (o))
    , hello (Test::Hello::_duplicate (h))
    , success (true)
  {
  }

  /// Function call operator overload.
  void operator() (T f)
  {
    try
      {
        f (orb.in (),
           hello.in ());
      }
    catch (const CORBA::Exception& ex)
      {
        ex._tao_print_exception ("Exception thrown:");

        success = false;
      }
  }

  CORBA::ORB_var orb;
  Test::Hello_var hello;
  bool success;
};

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  try
    {
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv);

      if (parse_args (argc, argv) != 0)
        return 1;

      CORBA::Object_var tmp =
        orb->string_to_object(ior);

      Test::Hello_var hello =
        Test::Hello::_narrow(tmp.in ());

      if (CORBA::is_nil (hello.in ()))
        {
          ACE_ERROR_RETURN ((LM_DEBUG,
                             "Nil Test::Hello reference <%s>\n",
                             ior),
                            1);
        }

      typedef void (*test_func) (CORBA::ORB_ptr,
                                 Test::Hello_ptr);

      static test_func const tests[] =
        {
          nested_recursive_struct_test
        };

      static size_t const test_count = sizeof (tests) / sizeof (test_func);

      // Have some fun with the STL.  :-)
      Caller<test_func> c =
        std::for_each (tests,
                       tests + test_count,
                       Caller<test_func> (orb.in (),
                                          hello.in ()));

      if (!c.success)
        throw Test::Recursive_Type_In_Any_Test_Failed ();

      hello->shutdown ();

      orb->destroy ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Exception caught:");
      return 1;
    }

  return 0;
}
