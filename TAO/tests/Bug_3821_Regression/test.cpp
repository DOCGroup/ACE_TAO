#include "testC.h"
#include "tao/CDR.h"

namespace
{
  void
  start_test (char const *&current_test, char const *name)
  {
    current_test = name;
    ACE_DEBUG ((LM_DEBUG, "testing: %C\n", current_test));
  }

  template<typename T>
  int
  marshal (TAO_OutputCDR &out, T const &t)
  {
    if (!(out << t))
      {
        ACE_ERROR_RETURN ((LM_ERROR,
                           "ERROR: Cannot marshal\n"),
                          -1);
      }
    return 0;
  }

  template<typename T>
  int
  demarshal (TAO_InputCDR &in, T &t,
             test::Enumeration wrong_disc,
             test::Enumeration expect_disc)
  {
    if (!(in >> t))
      {
        ACE_ERROR_RETURN ((LM_ERROR,
                           "ERROR: Cannot demarshal (test::Enumeration)\n"),
                          -1);
      }
    if (t._d () == wrong_disc || t._d () != expect_disc)
      {
        ACE_ERROR_RETURN ((LM_ERROR,
                           "ERROR: Demarshalled (test::Enumeration) _d=%d, wrong=%d, expected=%d\n",
                           t._d (), wrong_disc, expect_disc),
                          -1);
      }
    return 0;
  }

  template<typename T>
  int
  demarshal (TAO_InputCDR &in, T &t,
             CORBA::Boolean wrong_disc,
             CORBA::Boolean expect_disc)
  {
    if (!(in >> t))
      {
        ACE_ERROR_RETURN ((LM_ERROR,
                           "ERROR: Cannot demarshal (CORBA::Boolean)\n"),
                          -1);
      }
    if (t._d () == wrong_disc || t._d () != expect_disc)
      {
        ACE_ERROR_RETURN ((LM_ERROR,
                           "ERROR: Demarshalled (CORBA::Boolean) _d=%d, wrong=%d, expected=%d\n",
                           t._d (), wrong_disc, expect_disc),
                          -1);
      }
    return 0;
  }

  template<typename T>
  int
  demarshal (TAO_InputCDR &in, T &t,
             CORBA::Char wrong_disc,
             CORBA::Char expect_disc)
  {
    if (!(in >> t))
      {
        ACE_ERROR_RETURN ((LM_ERROR,
                           "ERROR: Cannot demarshal (CORBA::Char)\n"),
                          -1);
      }
    if (t._d () == wrong_disc || t._d () != expect_disc)
      {
        ACE_ERROR_RETURN ((LM_ERROR,
                           "ERROR: Demarshalled (CORBA::Char) _d=%d, wrong=%d, expected=%d\n",
                           t._d (), wrong_disc, expect_disc),
                          -1);
      }
    return 0;
  }

  template<typename T>
  int
  demarshal (TAO_InputCDR &in, T &t,
             CORBA::Long wrong_disc,
             CORBA::Long expect_disc)
  {
    if (!(in >> t))
      {
        ACE_ERROR_RETURN ((LM_ERROR,
                           "ERROR: Cannot demarshal (CORBA::Long)\n"),
                          -1);
      }
    if (t._d () == wrong_disc || t._d () != expect_disc)
      {
        ACE_ERROR_RETURN ((LM_ERROR,
                           "ERROR: Demarshalled (CORBA::Long) _d=%d, wrong=%d, expected=%d\n",
                           t._d (), wrong_disc, expect_disc),
                          -1);
      }
    return 0;
  }
}

int
ACE_TMAIN (int, ACE_TCHAR *[])
{
  int error= 0;
  char const *current_test = "initialization";

  try
    {
      // Marshal part.

      TAO_OutputCDR cdr_out;

      start_test (current_test, "uac_ed_out");
      test::UnionAllCovered_EnumerationDiscriminant uac_ed_out;
      // UnionAllCovered_EnumerationDiscriminant must have _default()
      // despite it has cases for all possible
      // values of enumeration. But since enumeration is just int it has
      // much more values than those listed in IDL.
      uac_ed_out._default ();
      marshal (cdr_out, uac_ed_out);

      start_test (current_test, "uac_ed2_out");
      test::UnionAllCovered_EnumerationDiscriminant uac_ed2_out;
      uac_ed2_out._default ();
      uac_ed2_out._d (static_cast<test::Enumeration> (10));
      marshal (cdr_out, uac_ed2_out);

      start_test (current_test, "uwd_ed_out");
      test::UnionWithDefault_EnumerationDiscriminant uwd_ed_out;
      uwd_ed_out._d (static_cast<test::Enumeration> (20));
      marshal (cdr_out, uwd_ed_out);

      start_test (current_test, "uwid_ed_out");
      test::UnionWithImplicitDefault_EnumerationDiscriminant uwid_ed_out;
      uwid_ed_out._default ();
      marshal (cdr_out, uwid_ed_out);

      start_test (current_test, "uwid_ed2_out");
      test::UnionWithImplicitDefault_EnumerationDiscriminant uwid_ed2_out;
      uwid_ed2_out._d (static_cast<test::Enumeration> (30));
      marshal (cdr_out, uwid_ed2_out);

      start_test (current_test, "uac_ted_out");
      test::UnionAllCovered_TypedefedEnumerationDiscriminant uac_ted_out;
      // UnionAllCovered_TypedefedEnumerationDiscriminant must have _default()
      // despite it has cases for all possible
      // values of enumeration. But since enumeration is just int it has
      // much more values than those listed in IDL.
      uac_ted_out._default ();
      marshal (cdr_out, uac_ted_out);

      start_test (current_test, "uac_ted2_out");
      test::UnionAllCovered_TypedefedEnumerationDiscriminant uac_ted2_out;
      uac_ted2_out._default ();
      uac_ted2_out._d (static_cast<test::Enumeration> (40));
      marshal (cdr_out, uac_ted2_out);

      start_test (current_test, "uwd_ted_out");
      test::UnionWithDefault_TypedefedEnumerationDiscriminant uwd_ted_out;
      uwd_ted_out._d (static_cast<test::Enumeration> (50));
      marshal (cdr_out, uwd_ted_out);

      start_test (current_test, "uwid_ted_out");
      test::UnionWithImplicitDefault_TypedefedEnumerationDiscriminant uwid_ted_out;
      uwid_ted_out._default ();
      marshal (cdr_out, uwid_ted_out);

      start_test (current_test, "uwid_ted2_out");
      test::UnionWithImplicitDefault_TypedefedEnumerationDiscriminant uwid_ted2_out;
      uwid_ted2_out._d (static_cast<test::Enumeration> (60));
      marshal (cdr_out, uwid_ted2_out);

      start_test (current_test, "uac_bd_out");
      test::UnionAllCovered_BooleanDiscriminant uac_bd_out;
      // uac_bd_out._default (); // no _default() since all labels are covered.
      uac_bd_out._d (static_cast<CORBA::Boolean> (3));
      marshal (cdr_out, uac_bd_out);

      start_test (current_test, "uac_bd2_out");
      test::UnionAllCovered_BooleanDiscriminant uac_bd2_out;
      uac_bd2_out._d (static_cast<CORBA::Boolean> (5));
      marshal (cdr_out, uac_bd2_out);

      start_test (current_test, "uwd_bd_out");
      test::UnionWithDefault_BooleanDiscriminant uwd_bd_out;
      uwd_bd_out.e_true (0);
      uwd_bd_out._d (static_cast<CORBA::Boolean> (7));
      marshal (cdr_out, uwd_bd_out);

      start_test (current_test, "uwid_bd_out");
      test::UnionWithImplicitDefault_BooleanDiscriminant uwid_bd_out;
      uwid_bd_out._default ();
      marshal (cdr_out, uwid_bd_out);

      start_test (current_test, "uwid_bd2_out");
      test::UnionWithImplicitDefault_BooleanDiscriminant uwid_bd2_out;
      uwid_bd2_out.e_true (16);
      uwid_bd2_out._d (static_cast<CORBA::Boolean> (9));
      marshal (cdr_out, uwid_bd2_out);

      start_test (current_test, "uwid_bdf_out");
      test::UnionWithImplicitDefaultFalse_BooleanDiscriminant uwid_bdf_out;
      uwid_bdf_out.e_false (17);
      marshal (cdr_out, uwid_bdf_out);

      start_test (current_test, "uac_cd_out");
      test::UnionAllCovered_CharDiscriminant uac_cd_out;
      // uac_cd_out._default (); // no _default() since all labels are covered.
      marshal (cdr_out, uac_cd_out);

      start_test (current_test, "uac_cd2_out");
      test::UnionAllCovered_CharDiscriminant uac_cd2_out;
      uac_cd2_out._d (100);
      marshal (cdr_out, uac_cd2_out);

      start_test (current_test, "uwd_cd_out");
      test::UnionWithDefault_CharDiscriminant uwd_cd_out;
      uwd_cd_out._d (110);
      marshal (cdr_out, uwd_cd_out);

      start_test (current_test, "uwid_cd_out");
      test::UnionWithImplicitDefault_CharDiscriminant uwid_cd_out;
      uwid_cd_out._default ();
      marshal (cdr_out, uwid_cd_out);

      start_test (current_test, "uwid_cd2_out");
      test::UnionWithImplicitDefault_CharDiscriminant uwid_cd2_out;
      uwid_cd2_out._d (120);
      marshal (cdr_out, uwid_cd2_out);

      start_test (current_test, "uwd_ld_out");
      test::UnionWithDefault_LongDiscriminant uwd_ld_out;
      uwd_ld_out._d (1000);
      marshal (cdr_out, uwd_ld_out);

      start_test (current_test, "uwid_ld_out");
      test::UnionWithImplicitDefault_LongDiscriminant uwid_ld_out;
      uwid_ld_out._default ();
      marshal (cdr_out, uwid_ld_out);

      start_test (current_test, "uwid_ld2_out");
      test::UnionWithImplicitDefault_LongDiscriminant uwid_ld2_out;
      uwid_ld2_out._d (2000);
      marshal (cdr_out, uwid_ld2_out);

      start_test (current_test, "labeled_from_default_out");
      test::UnionWithImplicitDefault_EnumerationDiscriminant labeled_from_default_out;
      labeled_from_default_out.e_enum1 (42);
      marshal (cdr_out, labeled_from_default_out);

      // Demarshal part.

      TAO_InputCDR cdr_in (cdr_out);

      start_test (current_test, "uac_ed_in");
      test::UnionAllCovered_EnumerationDiscriminant uac_ed_in;
      uac_ed_in.e_enum1 (0);
      demarshal (cdr_in, uac_ed_in, test::Enum1, uac_ed_out._d ());

      start_test (current_test, "uac_ed2_in");
      test::UnionAllCovered_EnumerationDiscriminant uac_ed2_in;
      uac_ed2_in.e_enum1 (0);
      demarshal (cdr_in, uac_ed2_in, test::Enum1, uac_ed2_out._d ());

      start_test (current_test, "uwd_ed_in");
      test::UnionWithDefault_EnumerationDiscriminant uwd_ed_in;
      uwd_ed_in.e_enum1 (0);
      demarshal (cdr_in, uwd_ed_in, test::Enum1, uwd_ed_out._d ());

      start_test (current_test, "uwid_ed_in");
      test::UnionWithImplicitDefault_EnumerationDiscriminant uwid_ed_in;
      uwid_ed_in.e_enum1 (0);
      demarshal (cdr_in, uwid_ed_in, test::Enum1, uwid_ed_out._d ());

      start_test (current_test, "uwid_ed2_in");
      test::UnionWithImplicitDefault_EnumerationDiscriminant uwid_ed2_in;
      uwid_ed2_in.e_enum1 (0);
      demarshal (cdr_in, uwid_ed2_in, test::Enum1, uwid_ed2_out._d ());

      start_test (current_test, "uac_ted_in");
      test::UnionAllCovered_TypedefedEnumerationDiscriminant uac_ted_in;
      uac_ted_in.e_enum1 (0);
      demarshal (cdr_in, uac_ted_in, test::Enum1, uac_ted_out._d ());

      start_test (current_test, "uac_ted2_in");
      test::UnionAllCovered_TypedefedEnumerationDiscriminant uac_ted2_in;
      uac_ted2_in.e_enum1 (0);
      demarshal (cdr_in, uac_ted2_in, test::Enum1, uac_ted2_out._d ());

      start_test (current_test, "uwd_ted_in");
      test::UnionWithDefault_TypedefedEnumerationDiscriminant uwd_ted_in;
      uwd_ted_in.e_enum1 (0);
      demarshal (cdr_in, uwd_ted_in, test::Enum1, uwd_ted_out._d ());

      start_test (current_test, "uwid_ted_in");
      test::UnionWithImplicitDefault_TypedefedEnumerationDiscriminant uwid_ted_in;
      uwid_ted_in.e_enum1 (0);
      demarshal (cdr_in, uwid_ted_in, test::Enum1, uwid_ted_out._d ());

      start_test (current_test, "uwid_ted2_in");
      test::UnionWithImplicitDefault_TypedefedEnumerationDiscriminant uwid_ted2_in;
      uwid_ted2_in.e_enum1 (0);
      demarshal (cdr_in, uwid_ted2_in, test::Enum1, uwid_ted2_out._d ());

      start_test (current_test, "uac_bd_in");
      test::UnionAllCovered_BooleanDiscriminant uac_bd_in;
      uac_bd_in.e_false (0);
      demarshal (cdr_in, uac_bd_in, false, uac_bd_out._d ());

      start_test (current_test, "uac_bd2_in");
      test::UnionAllCovered_BooleanDiscriminant uac_bd2_in;
      uac_bd2_in.e_false (0);
      demarshal (cdr_in, uac_bd2_in, false, uac_bd2_out._d ());

      start_test (current_test, "uwd_bd_in");
      test::UnionWithDefault_BooleanDiscriminant uwd_bd_in;
      uwd_bd_in.e_def (0);
      demarshal (cdr_in, uwd_bd_in, false, uwd_bd_out._d ());

      start_test (current_test, "uwid_bd_in");
      test::UnionWithImplicitDefault_BooleanDiscriminant uwid_bd_in;
      uwid_bd_in.e_true (0);
      demarshal (cdr_in, uwid_bd_in, true, uwid_bd_out._d ());

      start_test (current_test, "uwid_bd2_in");
      test::UnionWithImplicitDefault_BooleanDiscriminant uwid_bd2_in;
      uwid_bd2_in._d (false);
      demarshal (cdr_in, uwid_bd2_in, false, uwid_bd2_out._d ());
      if (uwid_bd2_in.e_true () != 16)
        {
          ACE_ERROR ((LM_ERROR,
                      "ERROR: Demarshalled true boolean member has the wrong value\n"));
          error = 1;
        }

      start_test (current_test, "uwid_bdf_in");
      test::UnionWithImplicitDefaultFalse_BooleanDiscriminant uwid_bdf_in;
      uwid_bdf_in._default ();
      demarshal (cdr_in, uwid_bdf_in, true, false);
      if (uwid_bdf_in.e_false () != 17)
        {
          ACE_ERROR ((LM_ERROR,
                      "ERROR: Demarshalled false boolean member has the wrong value\n"));
          error = 1;
        }

      start_test (current_test, "uac_cd_in");
      test::UnionAllCovered_CharDiscriminant uac_cd_in;
      uac_cd_in._d ('a');
      demarshal (cdr_in, uac_cd_in, 'a', uac_cd_out._d ());

      start_test (current_test, "uac_cd2_in");
      test::UnionAllCovered_CharDiscriminant uac_cd2_in;
      uac_cd2_in._d ('a');
      demarshal (cdr_in, uac_cd2_in, 'a', uac_cd2_out._d ());

      start_test (current_test, "uwd_cd_in");
      test::UnionWithDefault_CharDiscriminant uwd_cd_in;
      uwd_cd_in.e_chars (0);
      demarshal (cdr_in, uwd_cd_in, '\1', uwd_cd_out._d ());

      start_test (current_test, "uwid_cd_in");
      test::UnionWithImplicitDefault_CharDiscriminant uwid_cd_in;
      uwid_cd_in.e_chars (0);
      demarshal (cdr_in, uwid_cd_in, '\1', uwid_cd_out._d ());

      start_test (current_test, "uwid_cd2_in");
      test::UnionWithImplicitDefault_CharDiscriminant uwid_cd2_in;
      uwid_cd2_in.e_chars (0);
      demarshal (cdr_in, uwid_cd2_in, '\1', uwid_cd2_out._d ());

      start_test (current_test, "uwd_ld_in");
      test::UnionWithDefault_LongDiscriminant uwd_ld_in;
      uwd_ld_in.e_case1 (0);
      demarshal (cdr_in, uwd_ld_in, 1, uwd_ld_out._d ());

      start_test (current_test, "uwid_ld_in");
      test::UnionWithImplicitDefault_LongDiscriminant uwid_ld_in;
      uwid_ld_in.e_case1 (0);
      demarshal (cdr_in, uwid_ld_in, 1, uwid_ld_out._d ());

      start_test (current_test, "uwid_ld2_in");
      test::UnionWithImplicitDefault_LongDiscriminant uwid_ld2_in;
      uwid_ld2_in.e_case1 (0);
      demarshal (cdr_in, uwid_ld2_in, 1, uwid_ld2_out._d ());

      // Verify that CDR input can replace an implicit-default state with a
      // labeled member without bypassing public discriminator validation.
      start_test (current_test, "labeled_from_default_in");
      test::UnionWithImplicitDefault_EnumerationDiscriminant labeled_from_default_in;
      labeled_from_default_in._default ();
      demarshal (cdr_in, labeled_from_default_in, test::Enum2, test::Enum1);
      if (labeled_from_default_in.e_enum1 () != 42)
        {
          ACE_ERROR ((LM_ERROR,
                      "ERROR: Demarshalled labeled union member has the wrong value\n"));
          error = 1;
        }
    }
  catch (const CORBA::Exception& ex)
    {
      ACE_ERROR ((LM_ERROR,
                  "ERROR: CORBA exception while testing %C\n",
                  current_test));
      ex._tao_print_exception ("Exception caught:");
      error= 1;
    }

  return error;
}
