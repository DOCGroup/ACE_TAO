#include "testC.h"
#include "tao/IFR_Client/IFR_BaseC.h"
#include "tao/TypeCodeFactory/TypeCodeFactory_Loader.h"
#include "tao/CDR.h"
#include "tao/debug.h"

#if TAO_HAS_MINIMUM_CORBA == 0

static void
dump_cdr (TAO_OutputCDR const &cdr,
          size_t const dump_limit,
          ACE_TCHAR const * const single_message,
          ACE_TCHAR const * const first_message,
          ACE_TCHAR const * const second_message)
{
  if (TAO_debug_level >= 2)
    {
      if (cdr.length () > dump_limit)
        {
          ACE_HEX_DUMP ((LM_INFO, cdr.buffer (), dump_limit, first_message));
          ACE_HEX_DUMP ((LM_INFO,
                         cdr.buffer () + dump_limit,
                         cdr.length () - dump_limit,
                         second_message));
        }
      else
        {
          ACE_HEX_DUMP ((LM_INFO, cdr.buffer (), cdr.length (), single_message));
        }
    }
}

static bool
recursive_union_test ()
{
  constexpr size_t dump_limit = 512;
  constexpr CORBA::Short test_short = 23901;
  bool rc {true};

  ACE_DEBUG ((LM_INFO,
              ACE_TEXT ("Executing recursive union test\n")));
  ACE_DEBUG ((LM_INFO,
              ACE_TEXT ("* Simple marshaling -> demarshaling\n")));
  {
    CORBA::Any the_any {}, the_out_any {};

    Test::RecursiveUnion3 foo3 {};
    foo3.a (test_short);

    the_any <<= foo3;

    TAO_OutputCDR out {};

    out << the_any;

    out.consolidate ();

    dump_cdr (out, dump_limit,
              ACE_TEXT ("Encoded union:"),
              ACE_TEXT ("Encoded union (1):"),
              ACE_TEXT ("Encoded union (2):"));

    TAO_InputCDR in (out);

    if (TAO_debug_level >= 1)
      ACE_DEBUG ((LM_INFO,
                  ACE_TEXT ("> Extracting any\n")));

    in >> the_out_any;

    if (TAO_debug_level >= 1)
      ACE_DEBUG ((LM_INFO,
                  ACE_TEXT ("> Extracted any\n")));
  }
  // Any's typecode out of scope
  if (TAO_debug_level >= 1)
    ACE_DEBUG ((LM_INFO,
                ACE_TEXT ("> Any destructed\n")));

  ACE_DEBUG ((LM_INFO,
              ACE_TEXT ("* Marshaling -> demarshaling with external reference\n")));
  {
    // Keep an external reference to the nested indirected typecode alive.
    CORBA::TypeCode_var tc {};

    {
      CORBA::Any the_any {}, the_out_any {};

      Test::RecursiveUnion3 foo3 {};
      foo3.a (test_short);

      the_any <<= foo3;

      TAO_OutputCDR out {};

      out << the_any;

      out.consolidate ();

      dump_cdr (out, dump_limit,
                ACE_TEXT ("Encoded union:"),
                ACE_TEXT ("Encoded union (1):"),
                ACE_TEXT ("Encoded union (2):"));

      TAO_InputCDR in (out);

      if (TAO_debug_level >= 1)
        ACE_DEBUG ((LM_INFO,
                    ACE_TEXT ("> Extracting any\n")));

      in >> the_out_any;

      tc = the_out_any._tao_get_typecode ()->member_type (1); // union case type -> alias
      tc = tc->content_type (); // alias content type -> sequence
      tc = tc->content_type (); // sequence content type -> recursive struct
      tc = tc->member_type (0); // struct member type -> union
      tc = tc->member_type (1); // union case type -> alias
      tc = tc->content_type (); // alias content type -> sequence
      tc = tc->content_type (); // sequence content type -> indirected struct

      if (TAO_debug_level >= 1)
        ACE_DEBUG ((LM_INFO,
                    ACE_TEXT ("> Extracted any and stored external reference to Indirected_Type typecode\n")));
    }
    // Any's typecode out of scope
    if (TAO_debug_level >= 1)
      ACE_DEBUG ((LM_INFO,
                  ACE_TEXT ("> Any destructed\n")));
  }
  // referenced recursive typecode out of scope
  // Any's typecode out of scope
  if (TAO_debug_level >= 1)
    ACE_DEBUG ((LM_INFO,
                ACE_TEXT ("> External Indirected_Type typecode reference destructed\n")));

  ACE_DEBUG ((LM_INFO,
              ACE_TEXT ("* Marshaling -> demarshaling -> marshaling -> demarshaling\n")));
  {
    CORBA::Any the_any {}, the_out_any {}, the_out_any2 {};

    Test::RecursiveUnion3 foo3 {};
    foo3.a (test_short);

    the_any <<= foo3;

    TAO_OutputCDR out {};

    out << the_any;

    out.consolidate ();

    dump_cdr (out, dump_limit,
              ACE_TEXT ("Encoded union:"),
              ACE_TEXT ("Encoded union (1):"),
              ACE_TEXT ("Encoded union (2):"));

    TAO_InputCDR in (out);

    if (TAO_debug_level >= 1)
      ACE_DEBUG ((LM_INFO,
                  ACE_TEXT ("> Extracting any\n")));

    in >> the_out_any;

    if (TAO_debug_level >= 1)
      ACE_DEBUG ((LM_INFO,
                  ACE_TEXT ("> Extracted any\n")));

    TAO_OutputCDR out2 {};

    out2 << the_out_any;

    out2.consolidate ();

    dump_cdr (out2, dump_limit,
              ACE_TEXT ("Re-encoded union:"),
              ACE_TEXT ("Re-encoded union (1):"),
              ACE_TEXT ("Re-encoded union (2):"));

    // Allocator initialization makes a byte-for-byte check unreliable.
    TAO_InputCDR in2 (out2);

    if (TAO_debug_level >= 1)
      ACE_DEBUG ((LM_INFO,
                  ACE_TEXT ("> Re-extracting any\n")));

    in2 >> the_out_any2;

    if (TAO_debug_level >= 1)
      ACE_DEBUG ((LM_INFO,
                  ACE_TEXT ("> Re-extracted any\n")));

    if (!the_out_any._tao_get_typecode ()->equal (the_out_any2._tao_get_typecode ()))
      {
        ACE_ERROR ((LM_ERROR,
                    ACE_TEXT ("extracted recursive typecodes differ\n")));
        rc = false;
      }
  }
  // Any's typecodes out of scope
  if (TAO_debug_level >= 1)
    ACE_DEBUG ((LM_INFO,
                ACE_TEXT ("> Anys destructed\n")));

  return rc;
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try
    {
      CORBA::ORB_var const orb =
        CORBA::ORB_init (argc, argv);

      bool const result = recursive_union_test ();
      orb->destroy ();
      return result ? 0 : 1;
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("Exception caught:");
      return 1;
    }
}

#else

int
ACE_TMAIN (int, ACE_TCHAR *[])
{
  return 0;
}

#endif  /* TAO_HAS_MINIMUM_CORBA != 0 */
