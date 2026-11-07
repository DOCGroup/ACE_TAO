#include "TestC.h"
#include "tao/IFR_Client/IFR_BaseC.h"
#include "tao/TypeCodeFactory/TypeCodeFactory_Loader.h"
#include "ace/Get_Opt.h"

int
parse_args (int argc, ACE_TCHAR *argv[], ACE_TCHAR const *&ior)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("k:"));
  int c {};

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

template<typename T>
void
perform_invocation (Test::Hello_ptr hello,
                    CORBA::Any const & the_any)
{
  // Execute more than once to help verify that mutable repeated
  // TypeCode state is managed correctly.
  for (unsigned int n = 0; n < 2; ++n)
    {
      CORBA::Any_var const my_any =
        hello->get_any (the_any);

      T const *my_foo {};
      if (!(my_any.in () >>= my_foo))
        throw Test::Demarshaling_From_Any_Failed ();

      CORBA::TypeCode_var const the_tc = the_any.type ();
      CORBA::TypeCode_var const my_tc  = my_any.in ().type ();

      CORBA::Boolean const equal_tc =
        the_tc->equal (my_tc.in ());

      if (!equal_tc)
        throw Test::Repeated_Type_In_Any_Test_Failed ();

      CORBA::Boolean const equiv_tc =
        the_tc->equivalent (my_tc.in ());

      if (!equiv_tc)
        throw Test::Repeated_Type_In_Any_Test_Failed ();
    }
}

void
repeated_struct_test (CORBA::ORB_ptr /* orb */,
                      Test::Hello_ptr hello)
{
  ACE_DEBUG ((LM_INFO,
              "Executing repeated struct test\n"));

  Test::FooStruct foo;

  foo.Foo1 = 2;
  foo.Foo2 = 37;

  CORBA::Any the_any;
  the_any <<= foo;

  ::perform_invocation<Test::FooStruct> (hello, the_any);
}

/**
 * @struct Caller
 *
 * @brief Test method invocation functor.
 *
 * Test method invocation functor.
 */
template <typename T>
struct Caller
{
  /// Constructor.
  Caller (CORBA::ORB_ptr o, Test::Hello_ptr h)
    : orb (CORBA::ORB::_duplicate (o))
    , hello (Test::Hello::_duplicate (h))
  {
  }

  Caller (Caller const &) = delete;
  Caller &operator= (Caller const &) = delete;
  Caller (Caller &&) = delete;
  Caller &operator= (Caller &&) = delete;

  /// Function call operator overload.
  void operator() (T const f)
  {
    try
      {
        f (orb.in (),
           hello.in ());
      }
    catch (CORBA::Exception const &ex)
      {
        ex._tao_print_exception ("Exception thrown:");

        success = false;
      }
  }

  CORBA::ORB_var const orb;
  Test::Hello_var const hello;
  bool success {true};
};

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  ACE_TCHAR const *ior = ACE_TEXT ("file://test.ior");
  try
    {
      CORBA::ORB_var const orb =
        CORBA::ORB_init (argc, argv);

      if (parse_args (argc, argv, ior) != 0)
        return 1;

      CORBA::Object_var const tmp =
        orb->string_to_object (ior);

      Test::Hello_var const hello =
        Test::Hello::_narrow (tmp.in ());

      if (CORBA::is_nil (hello.in ()))
        {
          ACE_ERROR_RETURN ((LM_DEBUG,
                             "Nil Test::Hello reference <%s>\n",
                             ior),
                            1);
        }

      using test_func = void (*) (CORBA::ORB_ptr,
                                  Test::Hello_ptr);

      static constexpr test_func tests[] =
        {
          repeated_struct_test
        };

      Caller<test_func> c (orb.in (), hello.in ());
      for (test_func const test : tests)
        c (test);

      if (!c.success)
        throw Test::Repeated_Type_In_Any_Test_Failed ();

      hello->shutdown ();

      orb->destroy ();
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("Exception caught:");
      return 1;
    }

  return 0;
}
