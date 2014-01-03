// $Id$

#include "testC.h"
#include "tao/IFR_Client/IFR_BaseC.h"
#include "tao/TypeCodeFactory/TypeCodeFactory_Loader.h"
#include "tao/CDR.h"
#include "tao/debug.h"

#include "ace/OS_NS_string.h"

#if TAO_HAS_MINIMUM_CORBA == 0

bool
recursive_union_test ()
{
  bool rc = true;

  ACE_DEBUG ((LM_INFO,
              "Executing recursive union test\n"));


  ACE_DEBUG ((LM_INFO,
              "* Simple marshaling -> demarshaling\n"));
  {
    CORBA::Any the_any, the_out_any;

    static CORBA::Short const test_short = 23901;
    Test::RecursiveUnion3 foo3;
    foo3.a (test_short);

    the_any <<= foo3;

    TAO_OutputCDR out;

    out << the_any;

    out.consolidate ();

    if (TAO_debug_level >= 2)
    {
      if (out.length () > 512)
      {
        ACE_HEX_DUMP ((LM_INFO, out.buffer (), 512, "Encoded union (1):"));
        ACE_HEX_DUMP ((LM_INFO, out.buffer ()+512, out.length ()-512, "Encoded union (2):"));
      }
      else
      {
        ACE_HEX_DUMP ((LM_INFO, out.buffer (), out.length (), "Encoded union:"));
      }
    }

    TAO_InputCDR in(out);

    if (TAO_debug_level >= 1)
      ACE_DEBUG ((LM_INFO,
                  "> Extracting any\n"));

    in >> the_out_any;

    if (TAO_debug_level >= 1)
      ACE_DEBUG ((LM_INFO,
                  "> Extracted any\n"));
  }
  // Any's typecode out of scope
  if (TAO_debug_level >= 1)
    ACE_DEBUG ((LM_INFO,
                "> Any destructed\n"));

  ACE_DEBUG ((LM_INFO,
              "* Marshaling -> demarshaling with external reference\n"));
  {
    CORBA::TypeCode_var tc;

    {
      CORBA::Any the_any, the_out_any;

      static CORBA::Short const test_short = 23901;
      Test::RecursiveUnion3 foo3;
      foo3.a (test_short);

      the_any <<= foo3;

      TAO_OutputCDR out;

      out << the_any;

      out.consolidate ();

      if (TAO_debug_level >= 2)
      {
        if (out.length () > 512)
        {
          ACE_HEX_DUMP ((LM_INFO, out.buffer (), 512, "Encoded union (1):"));
          ACE_HEX_DUMP ((LM_INFO, out.buffer ()+512, out.length ()-512, "Encoded union (2):"));
        }
        else
        {
          ACE_HEX_DUMP ((LM_INFO, out.buffer (), out.length (), "Encoded union:"));
        }
      }

      TAO_InputCDR in(out);

      if (TAO_debug_level >= 1)
        ACE_DEBUG ((LM_INFO,
                    "> Extracting any\n"));

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
                    "> Extracted any and stored external reference to Indirected_Type typecode\n"));
    }
    // Any's typecode out of scope
    if (TAO_debug_level >= 1)
      ACE_DEBUG ((LM_INFO,
                  "> Any destructed\n"));
  }
  // referenced recursive typecode out of scope
  // Any's typecode out of scope
  if (TAO_debug_level >= 1)
    ACE_DEBUG ((LM_INFO,
                "> External Indirected_Type typecode reference destructed\n"));


  ACE_DEBUG ((LM_INFO,
              "* Marshaling -> demarshaling -> marshaling -> demarshaling\n"));
  {
    CORBA::Any the_any, the_out_any, the_out_any2;

    static CORBA::Short const test_short = 23901;
    Test::RecursiveUnion3 foo3;
    foo3.a (test_short);

    the_any <<= foo3;

    TAO_OutputCDR out;

    out << the_any;

    out.consolidate ();

    if (TAO_debug_level >= 2)
    {
      if (out.length () > 512)
      {
        ACE_HEX_DUMP ((LM_INFO, out.buffer (), 512, "Encoded union (1):"));
        ACE_HEX_DUMP ((LM_INFO, out.buffer ()+512, out.length ()-512, "Encoded union (2):"));
      }
      else
      {
        ACE_HEX_DUMP ((LM_INFO, out.buffer (), out.length (), "Encoded union:"));
      }
    }

    TAO_InputCDR in(out);

    if (TAO_debug_level >= 1)
      ACE_DEBUG ((LM_INFO,
                  "> Extracting any\n"));

    in >> the_out_any;

    if (TAO_debug_level >= 1)
      ACE_DEBUG ((LM_INFO,
                  "> Extracted any\n"));

    TAO_OutputCDR out2;

    out2 << the_out_any;

    out2.consolidate ();

    if (TAO_debug_level >= 2)
    {
      if (out2.length () > 512)
      {
        ACE_HEX_DUMP ((LM_INFO, out2.buffer (), 512, "Re-encoded union (1):"));
        ACE_HEX_DUMP ((LM_INFO, out2.buffer ()+512, out2.length ()-512, "Re-encoded union (2):"));
      }
      else
      {
        ACE_HEX_DUMP ((LM_INFO, out2.buffer (), out2.length (), "Re-encoded union:"));
      }
    }

// Unfortunately this test in unreliable due to the memory management optimizations of ACE, particularly
// concerning the non-initialization of allocated memory. When running with Valgrind all memory is forcibly
// initialized to 0 and than this test passes.
//
//    if (out.length () != out2.length () || ACE_OS::memcmp (out.buffer (), out2.buffer (), out.length ()) != 0)
//    {
//      ACE_ERROR ((LM_ERROR,
//                  "encoded representations of recursive typecodes differ\n"));
//
//      rc = false;
//    }

    TAO_InputCDR in2(out2);

    if (TAO_debug_level >= 1)
      ACE_DEBUG ((LM_INFO,
                  "> Re-extracting any\n"));

    in2 >> the_out_any2;

    if (TAO_debug_level >= 1)
      ACE_DEBUG ((LM_INFO,
                  "> Re-extracted any\n"));

    if (!the_out_any._tao_get_typecode ()->equal (the_out_any2._tao_get_typecode ()))
    {
      ACE_ERROR ((LM_ERROR,
                  "extracted recursive typecodes differ\n"));
      rc = false;
    }
  }
  // Any's typecodes out of scope
  if (TAO_debug_level >= 1)
    ACE_DEBUG ((LM_INFO,
                "> Anys destructed\n"));

  return rc;
}

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  try
    {
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv);

      recursive_union_test ();

      orb->destroy ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Exception caught:");
      return 1;
    }

  return 0;
}

#else

int
ACE_TMAIN(int , ACE_TCHAR *[])
{
  return 0;
}

#endif  /* TAO_HAS_MINIMUM_CORBA != 0 */
