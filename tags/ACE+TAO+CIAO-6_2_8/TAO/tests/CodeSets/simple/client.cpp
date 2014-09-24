// -*- C++ -*-

//=============================================================================
/**
 *  @file    client.cpp
 *
 *  $Id$
 *
 * A simple client to demonstrate the use of codeset translation
 *
 *
 *  @author   Phil Mesnier <mesnier_p@ociweb.com>
 */
//=============================================================================

// IDL generated headers
#include "simpleC.h"
#include "ace/ace_wchar.h"

// FUZZ: disable check_for_streams_include
#include "ace/streams.h"

#include "ace/OS_NS_string.h"
#include "ace/Log_Msg.h"
#include "ace/Get_Opt.h"

const ACE_TCHAR *ior = ACE_TEXT ("file://test.ior");

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
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-k <ior> "
                           "\n",
                           argv [0]),
                          -1);
      }
  // Indicates successful parsing of the command line
  return 0;
}

wchar_t *
make_wstring (const char *str)
{
  // Short circuit null pointer case
  if (str == 0)
    return 0;

  size_t len = ACE_OS::strlen (str) + 1;
  wchar_t *wstr = new wchar_t[len];
  ACE_DEBUG ((LM_DEBUG,
              "make_wstring: str = %C\n", str));
  for (size_t i = 0; i < len; i++)
    {
      char *t = const_cast<char *> (str);
      wstr[i] = static_cast<wchar_t> (*(t + i));
      ACE_DEBUG ((LM_DEBUG,
                  "wstr[%d] = %d\n", i, (short)wstr[i]));
    }
  return wstr;
}

// ------------------------------------------------------------
// Client
// ------------------------------------------------------------
int ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  int error_count = 0;

  try
    {
      // Init the orb
      CORBA::ORB_var orb= CORBA::ORB_init (argc, argv);

      // Get IOR from command line (or file)
      if (parse_args (argc, argv) != 0)
        return 1;

      // The first arg should be the IOR
      CORBA::Object_var object =
        orb->string_to_object (ior);

      // Get the server
      simple_var server = simple::_narrow (object.in ());

      const char *bare_string = "Hello World";

      const char *any_string = "Any World";
      CORBA::Any inarg;
      inarg <<= any_string;
      CORBA::Any_var outarg;

      // Invoke the call.
      CORBA::String_var reply =
        server->op1 (bare_string,
                     inarg,
                     outarg.out ());

      const char *any_reply;
      outarg >>= any_reply;

      ACE_DEBUG ((LM_DEBUG,
                 "Client sent %C, got %C\n", bare_string, reply.in ()) );

      if (ACE_OS::strcmp (bare_string, reply.in ()) != 0)
        {
          ++error_count;
        }

      ACE_DEBUG ((LM_DEBUG,
                 "Client sent %C, got %C\n", any_string, any_reply) );

      if (ACE_OS::strcmp (any_string, any_reply) != 0)
        {
          ++error_count;
        }
#if defined (ACE_HAS_WCHAR)
      wchar_t *wide_string = ACE_OS::strdup(ACE_TEXT_ALWAYS_WCHAR(ACE_TEXT ("Wide String")));
      wchar_t *wide_reply = server->op2 (wide_string);
      ACE_DEBUG ((LM_DEBUG,
                  "sent %W, got %W\n", wide_string, wide_reply));

      ACE_OS::free (wide_string);
      CORBA::wstring_free (wide_reply);
#endif /* ACE_HAS_WCHAR */

      server->shutdown ();

      orb->destroy ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Exception caught in client:");
      return 1;
    }

  return error_count;
}
