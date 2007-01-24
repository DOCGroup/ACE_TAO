// $Id$

#include "TestC.h"
#include "tao/IFR_Client/IFR_BaseC.h"
#include "tao/TypeCodeFactory/TypeCodeFactory_Loader.h"

#include "ace/Get_Opt.h"

#include <algorithm>
#include <functional>


ACE_RCSID (Recursive,
           client,
           "$Id$")


const char * ior = "file://test.ior";

int
parse_args (int argc, char *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, "k:");
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

template<>
void
dump<Test::RecursiveStruct> (Test::RecursiveStruct * data)
{
  ACE_DEBUG ((LM_DEBUG,
              "Test::RecursiveStruct\n"
              "%d\n"
              "%u\n"
              "%d\n"
              "%d\n",
              data->i,
              data->recursive_structs.length (),
              data->recursive_structs[0].i,
              data->recursive_structs[1].i));
}

template<>
void
dump<Test::NestedRecursiveStruct> (Test::NestedRecursiveStruct * data)
{
  ACE_DEBUG ((LM_DEBUG,
              "Test::NestedRecursiveStruct\n"
              "%d\n"
              "%u\n"
              "%d\n"
              "%d\n",
              data->i,
              data->ins.recursive_structs.length (),
              data->ins.recursive_structs[0].i,
              data->ins.recursive_structs[1].i));
}

template<>
void
dump<Test::RecursiveUnion> (Test::RecursiveUnion * data)
{
  ACE_DEBUG ((LM_DEBUG, "Test::RecursiveUnion\n"));

  switch (data->_d ())
    {
    case 0:
      {
        Test::RecursiveUnionSeq const & seq = data->recursive_unions ();

        ACE_DEBUG ((LM_DEBUG,
                    "%u\n"
                    "%d\n"
                    "%u\n",
                    seq.length (),
                    seq[0].i (),
                    seq[1].recursive_unions ().length ()));
      }
      break;
    default:
      ACE_DEBUG ((LM_DEBUG,
                  "%d\n",
                  data->i ()));
      break;
    }
}

template<typename T>
void
perform_invocation (Test::Hello_ptr hello,
                    CORBA::Any const & the_any
                    ACE_ENV_ARG_DECL)
{
  // Execute more than once to help verify that mutable recursive
  // TypeCode state is managed correctly.
  for (unsigned int n = 0; n < 2; ++n)
    {
      CORBA::Any_var my_any =
        hello->get_any (the_any
                        ACE_ENV_ARG_PARAMETER);

      T * my_foo = 0;
      if (!(my_any.in () >>= my_foo))
        ACE_THROW (Test::Demarshaling_From_Any_Failed ());

//       ACE_DEBUG ((LM_DEBUG, "Data dump:\n"));
//       dump<T> (my_foo);

      CORBA::TypeCode_var the_tc = the_any.type ();
      CORBA::TypeCode_var my_tc  = my_any->type ();

      CORBA::Boolean const equal_tc =
        the_tc->equal (my_tc.in ()
                       ACE_ENV_ARG_PARAMETER);

      if (!equal_tc)
        ACE_THROW (Test::Recursive_Type_In_Any_Test_Failed ());

      CORBA::Boolean const equiv_tc =
        the_tc->equivalent (my_tc.in ()
                            ACE_ENV_ARG_PARAMETER);

      if (!equiv_tc)
        ACE_THROW (Test::Recursive_Type_In_Any_Test_Failed ());
    }
}

void
recursive_struct_test (CORBA::ORB_ptr /* orb */,
                       Test::Hello_ptr hello
                       ACE_ENV_ARG_DECL)
{
  ACE_DEBUG ((LM_INFO,
              "Executing recursive struct test\n"));

  Test::RecursiveStruct foo;

  foo.recursive_structs.length (2);
  foo.recursive_structs[0].i = 37;
  foo.recursive_structs[1].i = 11034;
  foo.i = 12;

  CORBA::Any the_any;
  the_any <<= foo;

  ::perform_invocation<Test::RecursiveStruct> (hello,
                                               the_any
                                               ACE_ENV_ARG_PARAMETER);
}

void
nested_recursive_struct_test (CORBA::ORB_ptr /* orb */,
                       Test::Hello_ptr hello
                       ACE_ENV_ARG_DECL)
{
  ACE_DEBUG ((LM_INFO,
              "Executing nested recursive struct test\n"));

  Test::NestedRecursiveStruct foo;

  foo.ins.recursive_structs.length (2);
  foo.ins.recursive_structs[0].i = 37;
  foo.ins.recursive_structs[1].i = 11034;
  foo.i = 12;

  CORBA::Any the_any;
  the_any <<= foo;

  ::perform_invocation<Test::NestedRecursiveStruct> (hello,
                                               the_any
                                               ACE_ENV_ARG_PARAMETER);
}

void
recursive_union_test (CORBA::ORB_ptr /* orb */,
                      Test::Hello_ptr hello
                      ACE_ENV_ARG_DECL)
{
  ACE_DEBUG ((LM_INFO,
              "Executing recursive union test\n"));

  Test::RecursiveUnion foo;
  Test::EnumUnion foo_enum;
  static CORBA::Long const test_long = 238901;
  CORBA::Any the_any;

  // First simple case, just an union with an enum as discriminator
  foo_enum.i (test_long);
  the_any <<= foo_enum;

  ::perform_invocation<Test::EnumUnion> (hello,
                                         the_any
                                         ACE_ENV_ARG_PARAMETER);

  // Non-recursive member case.
  foo.i (test_long);

  the_any <<= foo;

  ::perform_invocation<Test::RecursiveUnion> (hello,
                                              the_any
                                              ACE_ENV_ARG_PARAMETER);

  // Recursive member case.
  Test::RecursiveUnionSeq seq;
  seq.length (2);
  seq[0].i (37);
  seq[1].recursive_unions (Test::RecursiveUnionSeq ());

  foo.recursive_unions (seq);

  the_any <<= foo;

  ::perform_invocation<Test::RecursiveUnion> (hello,
                                              the_any
                                              ACE_ENV_ARG_PARAMETER);

  // Recursive member case with no default member
  Test::RecursiveUnionSeqNoDefault seqnodefault;
  seqnodefault.length (2);
  seqnodefault[0].a (37);
  seqnodefault[1].recursive_unions (Test::RecursiveUnionSeqNoDefault ());

  Test::RecursiveUnionNoDefault foonodefault;
  foonodefault.recursive_unions (seqnodefault);

  the_any <<= foonodefault;

  ::perform_invocation<Test::RecursiveUnionNoDefault> (hello,
                                                       the_any
                                                       ACE_ENV_ARG_PARAMETER);

  // Recursive member case with enum .
  Test::VSortRecursiveUnionSeq vsortseq;
  vsortseq.length (2);
  vsortseq[0].i (37);
  vsortseq[1].recursive_unions (Test::VSortRecursiveUnionSeq ());

  Test::VSortRecursiveUnion vsort_foo;
  vsort_foo.recursive_unions (vsortseq);

  the_any <<= vsort_foo;

  ::perform_invocation<Test::VSortRecursiveUnion> (hello,
                                                   the_any
                                                   ACE_ENV_ARG_PARAMETER);

  // Non-recursive member case with enum .
  Test::NonRecursiveUnionWithEnum val;
  the_any <<= val;

  ::perform_invocation<Test::NonRecursiveUnionWithEnum> (hello,
                                                         the_any
                                                         ACE_ENV_ARG_PARAMETER);
}


void
indirectly_recursive_valuetype_test (CORBA::ORB_ptr /* orb */,
                                     Test::Hello_ptr /* hello */
                                     ACE_ENV_ARG_DECL_NOT_USED)
{
  ACE_DEBUG ((LM_INFO,
              "Executing indirectly recursive valuetype test\n"));

  ACE_DEBUG ((LM_WARNING,
              "  Currently unimplemented.\n"));
}

void
directly_recursive_valuetype_test (CORBA::ORB_ptr /* orb */,
                                   Test::Hello_ptr /* hello */
                                   ACE_ENV_ARG_DECL_NOT_USED)
{
  ACE_DEBUG ((LM_INFO,
              "Executing directly recursive valuetype test\n"));

  ACE_DEBUG ((LM_WARNING,
              "  Currently unimplemented.\n"));
}

#if TAO_HAS_MINIMUM_CORBA == 0

void
recursive_struct_typecodefactory_test (CORBA::ORB_ptr orb,
                                       Test::Hello_ptr hello
                                       ACE_ENV_ARG_DECL)
{
  ACE_DEBUG ((LM_INFO,
              "Executing recursive struct via TypeCodeFactory test\n"));

  Test::RecursiveStruct foo;

  foo.recursive_structs.length (2);
  foo.recursive_structs[0].i = 37;
  foo.recursive_structs[1].i = 11034;
  foo.i = 12;

  CORBA::Any the_any;
  the_any <<= foo;

  CORBA::TypeCode_var recursive_tc =
    orb->create_recursive_tc ("IDL:Test/RecursiveStruct:1.0"
                              ACE_ENV_ARG_PARAMETER);

  CORBA::TypeCode_var seq_tc =
    orb->create_sequence_tc (0,
                             recursive_tc.in ()
                             ACE_ENV_ARG_PARAMETER);

  CORBA::StructMemberSeq members (3);
  members.length (3);
  members[0].name = "recursive_structs";
  members[0].type = seq_tc;
  members[1].name = "i";
  members[1].type = CORBA::TypeCode::_duplicate (CORBA::_tc_long);
  members[2].name = "recursive_structs_second";
  members[2].type = seq_tc;

  CORBA::TypeCode_var struct_tc =
    orb->create_struct_tc ("IDL:Test/RecursiveStruct:1.0",
                           "RecursiveStruct",
                           members
                           ACE_ENV_ARG_PARAMETER);

  // Reset the underlying TypeCode to the one we just created with the
  // TypeCodeFactory.
  the_any.type (struct_tc.in ()
                ACE_ENV_ARG_PARAMETER);

  ::perform_invocation<Test::RecursiveStruct> (hello,
                                               the_any
                                               ACE_ENV_ARG_PARAMETER);
}

void
recursive_union_typecodefactory_test (CORBA::ORB_ptr /* orb */,
                                      Test::Hello_ptr /* hello */
                                      ACE_ENV_ARG_DECL_NOT_USED)
{
  ACE_DEBUG ((LM_INFO,
              "Executing recursive union via TypeCodeFactory test\n"));

  ACE_DEBUG ((LM_WARNING,
              "  Currently unimplemented.\n"));
}

void
indirectly_recursive_valuetype_typecodefactory_test (
  CORBA::ORB_ptr /* orb */,
  Test::Hello_ptr /* hello */
  ACE_ENV_ARG_DECL_NOT_USED)
{
  ACE_DEBUG ((LM_INFO,
              "Executing indirectly recursive valuetype via "
              "TypeCodeFactory test\n"));

  ACE_DEBUG ((LM_WARNING,
              "  Currently unimplemented.\n"));
}

void
directly_recursive_valuetype_typecodefactory_test (CORBA::ORB_ptr /* orb */,
                                                   Test::Hello_ptr /* hello */
                                                   ACE_ENV_ARG_DECL_NOT_USED)
{
  ACE_DEBUG ((LM_INFO,
              "Executing directly recursive valuetype via "
              "TypeCodeFactory test\n"));

  ACE_DEBUG ((LM_WARNING,
              "  Currently unimplemented.\n"));
}

#endif  /* TAO_HAS_MINIMUM_CORBA == 0 */


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
    ACE_DECLARE_NEW_ENV;
    ACE_TRY
      {
        f (orb.in (),
           hello.in ()
           ACE_ENV_ARG_PARAMETER);
      }
    ACE_CATCHANY
      {
        ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                             "Exception thrown:");

        success = false;
      }
    ACE_ENDTRY;
  }

  CORBA::ORB_var orb;
  Test::Hello_var hello;
  bool success;
};

int
main (int argc, char *argv[])
{
  ACE_TRY_NEW_ENV
    {
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv, "" ACE_ENV_ARG_PARAMETER);

      if (parse_args (argc, argv) != 0)
        return 1;

      CORBA::Object_var tmp =
        orb->string_to_object(ior ACE_ENV_ARG_PARAMETER);

      Test::Hello_var hello =
        Test::Hello::_narrow(tmp.in () ACE_ENV_ARG_PARAMETER);

      if (CORBA::is_nil (hello.in ()))
        {
          ACE_ERROR_RETURN ((LM_DEBUG,
                             "Nil Test::Hello reference <%s>\n",
                             ior),
                            1);
        }

      typedef void (*test_func) (CORBA::ORB_ptr,
                                 Test::Hello_ptr
                                 ACE_ENV_ARG_DECL);

      static test_func const tests[] =
        {
          recursive_struct_test
          , nested_recursive_struct_test
          , recursive_union_test
          , indirectly_recursive_valuetype_test
          , directly_recursive_valuetype_test
#if TAO_HAS_MINIMUM_CORBA == 0
          , recursive_struct_typecodefactory_test
          , recursive_union_typecodefactory_test
          , indirectly_recursive_valuetype_typecodefactory_test
          , directly_recursive_valuetype_typecodefactory_test
#endif  /* TAO_HAS_MINIMUM_CORBA == 0 */
        };

      static size_t const test_count = sizeof (tests) / sizeof (test_func);

      // Have some fun with the STL.  :-)
      Caller<test_func> c =
        std::for_each (tests,
                       tests + test_count,
                       Caller<test_func> (orb.in (),
                                          hello.in ()));

      if (!c.success)
        ACE_TRY_THROW (Test::Recursive_Type_In_Any_Test_Failed ());

      hello->shutdown ();

      orb->destroy ();
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Exception caught:");
      return 1;
    }
  ACE_ENDTRY;

  return 0;
}
