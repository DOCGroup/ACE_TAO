#include "Test2C.h"
#include "tao/IFR_Client/IFR_BaseC.h"
#include "tao/TypeCodeFactory/TypeCodeFactory_Loader.h"

#include "ace/Get_Opt.h"

#include <array>
#include <string>

int
parse_args (int argc, ACE_TCHAR *argv[], std::basic_string<ACE_TCHAR> &ior)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT ("k:"));
  int option {};

  while ((option = get_opts ()) != -1)
    switch (option)
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

template <typename T>
void
perform_invocation (Test::Hello_ptr hello,
                    CORBA::Any const & the_any)
{
  constexpr unsigned int invocation_count = 2;
  for (unsigned int n {}; n < invocation_count; ++n)
    {
      CORBA::Any_var my_any =
        hello->get_any (the_any);

      T const *my_foo {};
      if (!(my_any.in () >>= my_foo))
        throw Test::Demarshaling_From_Any_Failed ();

      CORBA::TypeCode_var const the_tc = the_any.type ();
      CORBA::TypeCode_var const my_tc = my_any->type ();

      CORBA::Boolean const equal_tc = the_tc->equal (my_tc.in ());

      if (!equal_tc)
        throw Test::Recursive_Type_In_Any_Test_Failed ();

      CORBA::Boolean const equiv_tc = the_tc->equivalent (my_tc.in ());

      if (!equiv_tc)
        throw Test::Recursive_Type_In_Any_Test_Failed ();
    }
}

void
recursive_typecode_test (CORBA::ORB_ptr /* orb */, Test::Hello_ptr hello)
{
  ACE_DEBUG ((LM_INFO,
              "Executing recursive typecode test\n"));

  CORBA::Any the_any;

  Test::MyAttRefSequence test;

  Test::MyAttRef attr;
  attr.attRefName = "attr";
  attr.attRefValue.attrValue ("value");
  attr.attRefQualifier = "atrQ1";

  Test::MyAttRef attr2;
  attr2.attRefName = "attr2";
  attr2.attRefValue.attrValue ("value2");
  attr2.attRefQualifier = "atrQ2";

  Test::MyAttRef comp;
  comp.attRefName = "comp1";
  comp.attRefQualifier = "compQ";

  Test::MyAttRef::MyAttRefValue::_compValue_seq comp_seq (1);
  comp_seq.length (1);
  comp_seq[0] = attr2;
  comp.attRefValue.compValue (comp_seq);

  Test::MyAttRef inti;
  inti.attRefName = "intval";
  inti.attRefValue.intValue (0xAFFEAFFE);

  test.length (3);
  test[0] = comp;
  test[1] = inti;
  test[2] = attr;

  the_any <<= test;

  ::perform_invocation<Test::MyAttRefSequence> (hello, the_any);
}

void
nested_recursive_typecode_test (CORBA::ORB_ptr /* orb */, Test::Hello_ptr hello)
{
  ACE_DEBUG ((LM_INFO,
              "Executing nested recursive typecode test\n"));

  CORBA::Any the_any;

  Test2::MyAttRefSeqStruct test;

  Test::MyAttRef attr;
  attr.attRefName = "attr";
  attr.attRefValue.attrValue ("value");
  attr.attRefQualifier = "atrQ1";

  Test::MyAttRef attr2;
  attr2.attRefName = "attr2";
  attr2.attRefValue.attrValue ("value2");
  attr2.attRefQualifier = "atrQ2";

  Test::MyAttRef comp;
  comp.attRefName = "comp1";
  comp.attRefQualifier = "compQ";

  Test::MyAttRef::MyAttRefValue::_compValue_seq comp_seq (1);
  comp_seq.length (1);
  comp_seq[0] = attr2;
  comp.attRefValue.compValue (comp_seq);

  Test::MyAttRef inti;
  inti.attRefName = "intval";
  inti.attRefValue.intValue (0xAFFEAFFE);

  test.attRefSeq.length (3);
  test.attRefSeq[0] = comp;
  test.attRefSeq[1] = inti;
  test.attRefSeq[2] = attr;

  the_any <<= test;

  ::perform_invocation<Test2::MyAttRefSeqStruct> (hello, the_any);
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
  explicit Caller (CORBA::ORB_ptr const o, Test::Hello_ptr const h)
    : orb (CORBA::ORB::_duplicate (o))
    , hello (Test::Hello::_duplicate (h))
    , success {true}
  {
  }

  Caller (Caller const &) = delete;
  Caller (Caller &&) = delete;
  Caller &operator= (Caller const &) = delete;
  Caller &operator= (Caller &&) = delete;

  /// Function call operator overload.
  void operator() (T const f)
  {
    try
      {
        f (this->orb.in (), this->hello.in ());
      }
    catch (CORBA::Exception const &ex)
      {
        ex._tao_print_exception ("Exception thrown:");

        this->success = false;
      }
  }

  CORBA::ORB_var orb {};
  Test::Hello_var hello {};
  bool success {};
};

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try
    {
      CORBA::ORB_var const orb {CORBA::ORB_init (argc, argv)};

      std::basic_string<ACE_TCHAR> ior {ACE_TEXT ("file://test.ior")};
      if (parse_args (argc, argv, ior) != 0)
        return 1;

      CORBA::Object_var const tmp =
        orb->string_to_object (ACE_TEXT_ALWAYS_CHAR (ior.c_str ()));

      Test::Hello_var const hello = Test::Hello::_narrow (tmp.in ());

      if (CORBA::is_nil (hello.in ()))
        {
          ACE_ERROR_RETURN ((LM_DEBUG,
                             "Nil Test::Hello reference <%s>\n",
                             ior.c_str ()),
                            1);
        }

      using test_func = void (*) (CORBA::ORB_ptr, Test::Hello_ptr);
      std::array<test_func, 2> const tests {{
        recursive_typecode_test,
        nested_recursive_typecode_test
      }};

      Caller<test_func> caller (orb.in (), hello.in ());
      for (test_func const test : tests)
        caller (test);

      if (!caller.success)
        throw Test::Recursive_Type_In_Any_Test_Failed ();

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
