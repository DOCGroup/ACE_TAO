// -*- C++ -*-
// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/tests/CodeSets/simple
//
// = FILENAME
//    client.cpp
//
// = DESCRIPTION
//   A simple client to demonstrate the use of codeset translation
//
// = AUTHORS
//      Phil Mesnier <mesnier_p@ociweb.com>
//
// ============================================================================
// IDL generated headers
#include "simpleC.h"
#include "ace/ace_wchar.h"
#include "ace/streams.h"

wchar_t *
make_wstring (const char *str)
{
  // Short circuit null pointer case
  if (str == 0)
    return 0;

  int len = strlen (str) + 1;
  wchar_t *wstr = new wchar_t[len];
  cout << "make_wstring: str = " << str << endl;
  for (int i = 0; i < len; i++)
    {
      char *t = ACE_const_cast (char *, str);
      wstr[i] = ACE_static_cast (wchar_t, *(t + i));
      cout << "wstr[" << i << "] = " << (short)wstr[i] << endl;
    }
  return wstr;
}

// ------------------------------------------------------------
// Client
// ------------------------------------------------------------
int main (int argc, char *argv[])
{
  char buf[1000];
  int error_count = 0;

  ACE_TRY_NEW_ENV
    {
      // Init the orb
      CORBA::ORB_var orb= CORBA::ORB_init (argc,
                                           argv,
                                           ""
                                           ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Get IOR from command line (or file)
      if (argc != 2)
        {
          ifstream fstr;
          fstr.open ("server.ior");

          if (fstr.bad ())
            {
              cout << "Cannot open server.ior and no IOR argument!" << endl;
              exit (1);
            }
          else
            {
              fstr >> buf;
            }
        }
      else
        {
          ACE_OS::strcpy (buf, argv[1]);
        }

      // The first arg should be the IOR
      CORBA::Object_var object =
        orb->string_to_object (buf
                               ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Get the server
      simple_var server = simple::_narrow (object.in ()
                                           ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      const char *bare_string = "Hello World";

      const char *any_string = "Any World";
      CORBA::Any inarg;
      inarg <<= CORBA::string_dup (any_string);
      CORBA::Any_var outarg;

      // Invoke the call.
      CORBA::String_var reply =
        server->op1 (bare_string,
                     inarg,
                     outarg.out ()
                     ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      const char *any_reply;
      outarg >>= any_reply;

      cout << "Client sent " << bare_string
           << ", got " << reply.in () << endl;

      if (ACE_OS::strcmp (bare_string, reply.in ()) != 0)
        {
          ++error_count;
        }

      cout << "Client sent " << any_string
           << ", got " << any_reply << endl;

      if (ACE_OS::strcmp (any_string, any_reply) != 0)
        {
          ++error_count;
        }
#if defined (ACE_HAS_WCHAR)
      wchar_t *wide_string = ACE_OS::strdup(ACE_TEXT_ALWAYS_WCHAR ("Wide String"));
      wchar_t *wide_reply = server->op2 (wide_string);
      cout << "sent " << wide_string << " got " << wide_reply << endl;
#endif /* ACE_HAS_WCHAR */
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Exception caught:");
      return 1;
    }
  ACE_ENDTRY;

  return error_count;
}
