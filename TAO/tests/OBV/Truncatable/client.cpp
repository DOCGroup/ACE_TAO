// $Id$

#include "TruncatableC.h"
#include "ExtraC.h"
#include "ace/Get_Opt.h"

const ACE_TCHAR *ior = ACE_TEXT("file://test.ior");

int fail = 0;
int pretest = 0;
int verbose = 0;

#if defined (VERIFY)
#undef VERIFY
#endif /* VERIFY */

#define VERIFY(Condition) \
{ \
    if ((Condition)==0) \
    { \
        fail++; \
        if (!verbose) \
          ACE_DEBUG ((LM_DEBUG, ACE_TEXT("(%P|%t) client - Failure at line %l\n"))); \
    } \
}

int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("k:v"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'k':
        ior = get_opts.optarg;
        break;
      case 'v':
        verbose = 1;
        break;
      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-k <ior> "
                           "\n",
                           argv [0]),
                          -1);
      }
  // Indicates sucessful parsing of the command line
  return 0;
}

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  try
    {
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv);

      if (parse_args (argc, argv) != 0)
        return 1;

      // Create factories.

      OBV_TruncatableTest::BaseValue_init *base_factory = 0;
      ACE_NEW_RETURN (base_factory,
                      OBV_TruncatableTest::BaseValue_init,
                      1);

      orb->register_value_factory (base_factory->tao_repository_id (),
                                   base_factory);
      base_factory->_remove_ref (); // release ownership



      OBV_TruncatableTest::TValue1_init *value1_factory = 0;
      ACE_NEW_RETURN (value1_factory,
                      OBV_TruncatableTest::TValue1_init,
                      1);

      orb->register_value_factory (value1_factory->tao_repository_id (),
                                   value1_factory);
      value1_factory->_remove_ref ();



      OBV_TruncatableTest::TValue2_init *value2_factory = 0;
      ACE_NEW_RETURN (value2_factory,
                      OBV_TruncatableTest::TValue2_init,
                      1);

      orb->register_value_factory (value2_factory->tao_repository_id (),
                                   value2_factory);
      value2_factory->_remove_ref ();


      OBV_TruncatableTest::TValue3_init *value3_factory = 0;
      ACE_NEW_RETURN (value3_factory,
                      OBV_TruncatableTest::TValue3_init,
                      1);

      orb->register_value_factory (value3_factory->tao_repository_id (),
                                   value3_factory);
      value3_factory->_remove_ref ();

      OBV_TruncatableTest::TValue4_init *value4_factory = 0;
      ACE_NEW_RETURN (value4_factory,
                      OBV_TruncatableTest::TValue4_init,
                      1);

      orb->register_value_factory (value4_factory->tao_repository_id (),
                                   value4_factory);
      value4_factory->_remove_ref ();

      OBV_TruncatableTest::TValue5_init *value5_factory = 0;
      ACE_NEW_RETURN (value5_factory,
                      OBV_TruncatableTest::TValue5_init,
                      1);

      orb->register_value_factory (value5_factory->tao_repository_id (),
                                   value5_factory);
      value5_factory->_remove_ref ();

      OBV_TruncatableTest::NestedValue_init *nested_value_factory = 0;
      ACE_NEW_RETURN (nested_value_factory,
                      OBV_TruncatableTest::NestedValue_init,
                      1);

      orb->register_value_factory (nested_value_factory->tao_repository_id (),
                                   nested_value_factory);
      nested_value_factory->_remove_ref ();

      // Obtain reference to the object
      CORBA::Object_var tmp =
        orb->string_to_object(ior);

      OBV_TruncatableTest::Test_var test =
        OBV_TruncatableTest::Test::_narrow(tmp.in ());

      if (CORBA::is_nil (test.in ()))
        {
          ACE_ERROR_RETURN ((LM_DEBUG,
                             "Nil OBV_TruncatableTest::Test reference <%s>\n",
                             ior),
                            1);
        }

      CORBA::String_var desc;;
      { //isolate the scope of objects to avoid using wrong values
        OBV_OBV_TruncatableTest::TValue1 v1;
        v1.basic_data (9);
        v1.data1 (99);

        OBV_TruncatableTest::BaseValue_var ov1;

        desc = CORBA::string_dup ("A<-tB, truncate B to A");
        if (verbose)
          ACE_DEBUG ((LM_DEBUG,ACE_TEXT("Case 1: %C: "),
                      desc.in()));
        pretest = fail;

        test->op1 ("case1", &v1, ov1.out (), desc.inout ());


        VERIFY (! ACE_OS::strcmp (desc.in (),
                                  "case1: A<-tB, truncate B to A"));
        VERIFY (v1.basic_data () == ov1->basic_data ());
        if (verbose)
          ACE_DEBUG ((LM_DEBUG,ACE_TEXT ("%s\n"),
                      (pretest == fail) ?
                      ACE_TEXT ("passed") : ACE_TEXT ("failed")));
      }

      { //isolate the scope of objects to avoid using wrong values
        OBV_OBV_TruncatableTest::TValue2 v2;

        v2.basic_data (9);
        v2.data1 (99);
        v2.data2 (99 * 2);

        OBV_TruncatableTest::BaseValue_var ov2;
        desc = CORBA::string_dup ("A<-tB<-tC, truncate C to A");
        if (verbose)
          ACE_DEBUG ((LM_DEBUG,ACE_TEXT("Case 2: %C: "),
                      desc.in()));
        pretest = fail;

        test->op1 ("case2", &v2, ov2.out (), desc.inout ());

        VERIFY (! ACE_OS::strcmp (desc.in (),
                                  "case2: A<-tB<-tC, truncate C to A"));
        VERIFY (v2.basic_data () == ov2->basic_data ());
        if (verbose)
          ACE_DEBUG ((LM_DEBUG,ACE_TEXT ("%s\n"),
                      (pretest == fail) ?
                      ACE_TEXT ("passed") : ACE_TEXT ("failed")));

        OBV_TruncatableTest::TValue1_var otv1;
        desc = CORBA::string_dup ("A<-tB<-tC, truncate C to B");
        if (verbose)
          ACE_DEBUG ((LM_DEBUG,ACE_TEXT("Case 3: %C: "),
                      desc.in()));
        pretest = fail;

        test->op2 (&v2, "case3", otv1.out (), desc.inout ());

        VERIFY (! ACE_OS::strcmp (desc.in (),
                                  "case3: A<-tB<-tC, truncate C to B"));
        VERIFY (v2.basic_data () == otv1->basic_data ());
        VERIFY (v2.data1 () == otv1->data1 ());
        if (verbose)
          ACE_DEBUG ((LM_DEBUG,ACE_TEXT ("%s\n"),
                      (pretest == fail) ?
                      ACE_TEXT ("passed") : ACE_TEXT ("failed")));
      }

      { //isolate the scope of objects to avoid using wrong values
        OBV_OBV_TruncatableTest::TValue1 itv1b;

        itv1b.basic_data(7);
        itv1b.data1(8);
        OBV_TruncatableTest::TValue1_var otv1b;
        desc = CORBA::string_dup ("A<-tB, truncatable but no truncation");
        if (verbose)
          ACE_DEBUG ((LM_DEBUG,ACE_TEXT("Case 3b: %C: "),
                      desc.in()));
        pretest = fail;

        test->op2 (&itv1b, "case3b", otv1b.out (), desc.inout ());

        VERIFY (! ACE_OS::strcmp (desc.in (),
                "case3b: A<-tB, truncatable but no truncation"));
        VERIFY (itv1b.basic_data () == otv1b->basic_data ());
        VERIFY (itv1b.data1 () == otv1b->data1 ());
        if (verbose)
          ACE_DEBUG ((LM_DEBUG,ACE_TEXT ("%s\n"),
                      (pretest == fail) ?
                      ACE_TEXT ("passed") : ACE_TEXT ("failed")));
      }

      { //isolate the scope of objects to avoid using wrong values
        OBV_OBV_TruncatableTest::TValue3 v3;
        v3.basic_data (9);
        v3.data1 (99);
        v3.data3 (99 * 3);

        bool caught_expected_exception = false;
        try
          {
            OBV_TruncatableTest::BaseValue_var ov3;
            desc = CORBA::string_dup
              ("A<-tB<-C, try truncate C to A, MARSHAL exception");
            if (verbose)
              ACE_DEBUG ((LM_DEBUG,ACE_TEXT("Case 4: %C: "),
                          desc.in()));
            test->op1 ("case4", &v3, ov3.out (), desc.inout ());
          }
        catch (const CORBA::MARSHAL&)
          {
            if (verbose)
              ACE_DEBUG ((LM_DEBUG,ACE_TEXT ("passed\n")));
            caught_expected_exception = true;
          }

        if ( ! caught_expected_exception)
          {
            fail++;
            if (verbose)
              ACE_DEBUG ((LM_DEBUG,ACE_TEXT ("faileded\n")));
          }
      } //end marshaling exception test.

      { //isolate the scope of objects to avoid using wrong values
        OBV_OBV_TruncatableTest::NestedValue nv;
        nv.data (2);

        OBV_OBV_TruncatableTest::TValue5 v5;

        v5.basic_data (9);
        v5.nv4 (&nv);
        v5.data4 (99 * 4);
        v5.str1 (CORBA::string_dup ("str1"));
        v5.data5 (99 * 5);
        v5.nv5 (&nv);
        v5.str2 (CORBA::string_dup ("str2"));

        OBV_TruncatableTest::BaseValue_var ov5;
        desc = CORBA::string_dup
          ("A<-tB<-tC, B & C have nested value type, truncate C to A");
        if (verbose)
          ACE_DEBUG ((LM_DEBUG,ACE_TEXT("Case 5: %C: "),
                      desc.in()));
        pretest = fail;
        test->op1 ("case5", &v5, ov5.out (), desc.inout ());

        VERIFY (! ACE_OS::strcmp (desc.in (),
          "case5: A<-tB<-tC, B & C have nested value type, truncate C to A"));
        VERIFY (v5.basic_data () == ov5->basic_data ());
        if (verbose)
          ACE_DEBUG ((LM_DEBUG,ACE_TEXT ("%s\n"),
                      (pretest == fail) ?
                      ACE_TEXT ("passed") : ACE_TEXT ("failed")));

        OBV_TruncatableTest::TValue4_var otv4;
        desc = CORBA::string_dup
          ("A<-tB<-tC, B & C have nested value type, truncate C to B");
        if (verbose)
          ACE_DEBUG ((LM_DEBUG,ACE_TEXT("Case 6: %C: "),
                      desc.in()));
        pretest = fail;
        test->op3 ("case6", &v5, otv4.out (), desc.inout ());

        VERIFY (! ACE_OS::strcmp (desc.in (),
           "case6: A<-tB<-tC, B & C have nested value type, truncate C to B"));
        VERIFY (v5.basic_data () == otv4->basic_data ());
        VERIFY (v5.nv4 ()->data () == otv4->nv4 ()->data ());
        VERIFY (v5.data4 () == otv4->data4 ());
        if (verbose)
          ACE_DEBUG ((LM_DEBUG,ACE_TEXT ("%s\n"),
                      (pretest == fail) ? ACE_TEXT ("passed") : ACE_TEXT ("failed")));
      }

      { //isolate the scope of objects to avoid using wrong values
        OBV_OBV_TruncatableTest::TValue6 iv;
        iv.basic_data (9);

        OBV_TruncatableTest::BaseValue_var ov;

        desc = CORBA::string_dup ("A<-tB, B has no data, truncate B to A");
        if (verbose)
          ACE_DEBUG ((LM_DEBUG,ACE_TEXT("Case 7: %C: "),
                      desc.in()));
        pretest = fail;
        test->op1 ("case7", &iv, ov.out (), desc.inout ());

        VERIFY (! ACE_OS::strcmp (desc.in (),
                "case7: A<-tB, B has no data, truncate B to A"));
        VERIFY (iv.basic_data () == ov->basic_data ());
        if (verbose)
          ACE_DEBUG ((LM_DEBUG,ACE_TEXT ("%s\n"),
                      (pretest == fail) ?
                      ACE_TEXT ("passed") : ACE_TEXT ("failed")));
      }

      { //isolate the scope of objects to avoid using wrong values
        OBV_OBV_TruncatableTest::TValue1 v1;
        v1.basic_data (8);
        v1.data1 (88);

        OBV_OBV_TruncatableTest::TValue1 v4;
        v4.basic_data (9);
        v4.data1 (99);

        OBV_OBV_TruncatableTest::NestedValue nv;
        nv.data (2);

        OBV_OBV_TruncatableTest::TValue4 v2;

        v2.basic_data (7);
        v2.nv4 (&nv);
        v2.data4 (77);

        OBV_OBV_TruncatableTest::TValue4 v3;
        v3.basic_data (6);
        v3.nv4 (&nv);
        v3.data4 (66);

        desc = CORBA::string_dup
          ("multiple IN truncatable valuetype parameters"
           " and return truncatable valuetype");
        if (verbose)
          ACE_DEBUG ((LM_DEBUG,ACE_TEXT("Case 8: %C: "),
                      desc.in()));
        pretest = fail;
        OBV_TruncatableTest::BaseValue_var ov
          = test->op4 ("case8", &v1, 5, &v2, &v3, &v4, desc.inout ());

        VERIFY (! ACE_OS::strcmp (desc.in (),
                 "case8: multiple IN truncatable valuetype parameters"
                        " and return truncatable valuetype"));
        CORBA::ULong total = 5 * (v1.basic_data () + v2.basic_data () +
                                  v3.basic_data () + v4.basic_data ());
        VERIFY (ov->basic_data () == total);
        if (verbose)
          ACE_DEBUG ((LM_DEBUG,ACE_TEXT ("%s\n"),
                      (pretest == fail) ?
                      ACE_TEXT ("passed") : ACE_TEXT ("failed")));
      }

      {
        OBV_OBV_TruncatableTest::Extra1 v1;
        v1.basic_data (9);
        v1.data1 (99);
        v1.edata1 (1234);

        OBV_TruncatableTest::TValue1_var ov1;

        desc = CORBA::string_dup ("A<-tB, truncate unknown B to A");
        if (verbose)
          ACE_DEBUG ((LM_DEBUG,ACE_TEXT("Case 9: %C: "),
                      desc.in()));
        pretest = fail;

        test->op2 (&v1, "case9", ov1.out (), desc.inout ());


        VERIFY (! ACE_OS::strcmp (desc.in (),
                                  "case9: A<-tB, truncate unknown B to A"));
        VERIFY (v1.basic_data () == ov1->basic_data ()
                && v1.data1() == ov1->data1());
        if (verbose)
          ACE_DEBUG ((LM_DEBUG,ACE_TEXT ("%s\n"),
                      (pretest == fail) ?
                      ACE_TEXT ("passed") : ACE_TEXT ("failed")));
      }


      {
        OBV_OBV_TruncatableTest::TValue1 v1;
        v1.basic_data (9);
        v1.data1 (99);

        OBV_TruncatableTest::TValue1_var ov1;
        CORBA::Any a;
        a <<= &v1;
        desc = CORBA::string_dup ("A<-tB, known truncatable via Any");
        if (verbose)
          ACE_DEBUG ((LM_DEBUG,ACE_TEXT("Case 10: %C: "),
                      desc.in()));
        pretest = fail;

        test->op5 (a, "case10", ov1.out (), desc.inout ());


        VERIFY (! ACE_OS::strcmp (desc.in (),
                                  "case10: A<-tB, known truncatable via Any"));
        VERIFY (v1.basic_data () == ov1->basic_data ()
                && v1.data1() == ov1->data1());
        if (verbose)
          ACE_DEBUG ((LM_DEBUG,ACE_TEXT ("%s\n"),
                      (pretest == fail) ?
                      ACE_TEXT ("passed") : ACE_TEXT ("failed")));
      }

      {
        OBV_OBV_TruncatableTest::Extra1 v1;
        v1.basic_data (9);
        v1.data1 (99);
        v1.edata1 (1234);

        OBV_TruncatableTest::TValue1_var ov1;
        CORBA::Any a;
        a <<= &v1;
        desc = CORBA::string_dup ("A<-tB, unknown truncatable via Any");
        if (verbose)
          ACE_DEBUG ((LM_DEBUG,ACE_TEXT("Case 11: %C: "),
                      desc.in()));
        pretest = fail;

        test->op5 (a, "case11", ov1.out (), desc.inout ());


        VERIFY (! ACE_OS::strcmp (desc.in (),
                                  "case11: A<-tB, unknown truncatable via Any"));
        VERIFY (v1.basic_data () == ov1->basic_data ()
                && v1.data1() == ov1->data1());
        if (verbose)
          ACE_DEBUG ((LM_DEBUG,ACE_TEXT ("%s\n"),
                      (pretest == fail) ?
                      ACE_TEXT ("passed") : ACE_TEXT ("failed")));
      }

      ACE_DEBUG ((LM_DEBUG, "(%P|%t) client - shutdown orb\n"));


      test->shutdown ();

      orb->destroy ();

      if (fail)
        {
          ACE_ERROR ((LM_ERROR, "(%P|%t) client: test failed\n"));
          return 1;
        }
      else
        ACE_DEBUG((LM_DEBUG, "(%P|%t) client: test passed\n"));
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Exception caught:");
      return 1;
    }

  return 0;
}
