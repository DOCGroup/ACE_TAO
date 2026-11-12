// -*- C++ -*-

//=============================================================================
/**
 *  @file    client.cpp
 *
 * A simple client to demonstrate the use of codeset translation
 *
 *  @author   Phil Mesnier <mesnier_p@ociweb.com>
 */
//=============================================================================

// IDL generated headers
#include "simpleC.h"
#include "ace/ace_wchar.h"

// FUZZ: disable check_for_streams_include
#include "ace/streams.h"

#include "ace/Log_Msg.h"
#include "ace/Get_Opt.h"

#include <climits>
#include <cstring>
#include <cwchar>

struct Options
{
  ACE_TCHAR const *ior {ACE_TEXT ("file://test.ior")};
  bool test_supplementary_character {};
};

static int
parse_args (int argc, ACE_TCHAR *argv[], Options &options)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("k:w"));
  int c {};

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'k':
        options.ior = get_opts.opt_arg ();
        break;

      case 'w':
        options.test_supplementary_character = true;
        break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-k <ior> "
                           "[-w] "
                           "\n",
                           argv [0]),
                          -1);
      }
  // Indicates successful parsing of the command line
  return 0;
}

// ------------------------------------------------------------
// Client
// ------------------------------------------------------------
int ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  int error_count {};

  try
    {
      // Init the orb
      CORBA::ORB_var const orb = CORBA::ORB_init (argc, argv);

      // Get IOR from command line (or file)
      Options options {};
      if (parse_args (argc, argv, options) != 0)
        return 1;

      // The first arg should be the IOR
      CORBA::Object_var const object =
        orb->string_to_object (options.ior);

      // Get the server
      simple_var const server = simple::_narrow (object.in ());

      char const * const bare_string = "Hello World";

      char const * const any_string = "Any World";
      CORBA::Any inarg;
      inarg <<= any_string;
      CORBA::Any_var outarg;

      // Invoke the call.
      CORBA::String_var const reply =
        server->op1 (bare_string,
                     inarg,
                     outarg.out ());

      char const *any_reply {};
      outarg >>= any_reply;

      ACE_DEBUG ((LM_DEBUG,
                 "Client sent %C, got %C\n", bare_string, reply.in ()));

      if (std::strcmp (bare_string, reply.in ()) != 0)
        {
          ++error_count;
        }

      ACE_DEBUG ((LM_DEBUG,
                 "Client sent %C, got %C\n", any_string, any_reply));

      if (std::strcmp (any_string, any_reply) != 0)
        {
          ++error_count;
        }

      CORBA::Char const high_byte = static_cast<CORBA::Char> (0x80);
      CORBA::Char const high_byte_reply = server->op3 (high_byte);
      if (static_cast<unsigned char> (high_byte) !=
          static_cast<unsigned char> (high_byte_reply))
        {
          ACE_ERROR ((LM_ERROR, "High byte char round trip failed\n"));
          ++error_count;
        }

#if defined (ACE_HAS_WCHAR)
      ACE_CDR::WChar const * const wide_string =
        L"Wide String";
      CORBA::WString_var const wide_reply = server->op2 (wide_string);
      ACE_DEBUG ((LM_DEBUG,
                  "sent %W, got %W\n", wide_string, wide_reply.in ()));
      if (wide_reply.in () == nullptr)
        {
          ACE_ERROR ((LM_ERROR, "Wide string round trip returned null\n"));
          ++error_count;
        }
      else if (std::wcscmp (wide_string, wide_reply.in ()) != 0)
        {
          ACE_ERROR ((LM_ERROR,
                      "Wide string round trip mismatch: input first unit %d, reply first unit %d\n",
                      static_cast<int> (wide_string[0]),
                      static_cast<int> (wide_reply.in ()[0])));
          ++error_count;
        }

#if WCHAR_MAX > 0xFFFF
      if (options.test_supplementary_character)
        {
          ACE_CDR::WChar const * const supplementary_string = L"\U0001F600";
          CORBA::WString_var const supplementary_reply =
            server->op2 (supplementary_string);
          if (supplementary_reply.in () == nullptr ||
              std::wcscmp (supplementary_string, supplementary_reply.in ()) != 0)
            {
              ACE_ERROR ((LM_ERROR,
                          "Supplementary wide character round trip failed\n"));
              ++error_count;
            }
        }
#endif /* WCHAR_MAX > 0xFFFF */

#endif /* ACE_HAS_WCHAR */

      server->shutdown ();

      orb->destroy ();
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("Exception caught in client:");
      return 1;
    }

  return error_count;
}
