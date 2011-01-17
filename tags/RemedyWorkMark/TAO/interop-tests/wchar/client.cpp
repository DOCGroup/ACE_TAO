// -*- C++ -*-
// $Id$

// ============================================================================
//
// = LIBRARY
//    interop_test/wchar
//
// = FILENAME
//    client.cpp
//
// = DESCRIPTION
//    C++ client side for testing interoperability with wchar data.
//
// = AUTHOR
//    Phil Mesnier <mesnier_p@ociweb.com>
//
// ============================================================================

#include "interop_wcharC.h"
#include "wchar_reference.h"

#include "ace/Get_Opt.h"
#include "ace/Argv_Type_Converter.h"

const int WCHAR_TO_SERVER         = 0x00000001;
const int WSTRING_TO_SERVER       = 0x00000002;
const int WARRAY_TO_SERVER        = 0x00000004;
const int ANY_WCHAR_TO_SERVER     = 0x00000008;
const int WSTRING_EXCEPTION       = 0x00000010;
const int WCHAR_FROM_SERVER       = 0x00000020;
const int WSTRING_FROM_SERVER     = 0x00000040;
const int WARRAY_FROM_SERVER      = 0x00000080;
const int ANY_WCHAR_FROM_SERVER   = 0x00000100;
const int ANY_WSTRING_TO_SERVER   = 0x00000200;
const int ANY_WSTRING_FROM_SERVER = 0x00000400;
const int ANY_WSTRING_ECHO        = 0x00000800;
const int WSTRUCTSEQ_FROM_SERVER  = 0x00001000;
const int WSTRUCTSEQ_TO_SERVER    = 0x00002000;
const int NUL_WSTRING_TO_SERVER   = 0x00004000;
const int TBD_16 = 0x00008000; // update ALL_TESTS if this is defined
const int TBD_15 = 0x00010000; // update ALL_TESTS if this is defined
const int TBD_14 = 0x00020000; // update ALL_TESTS if this is defined
const int TBD_13 = 0x00040000; // update ALL_TESTS if this is defined
const int TBD_12 = 0x00080000; // update ALL_TESTS if this is defined
const int TBD_11 = 0x00100000; // update ALL_TESTS if this is defined
const int TBD_10 = 0x00200000; // update ALL_TESTS if this is defined
const int TBD_9  = 0x00400000; // update ALL_TESTS if this is defined
const int TBD_8  = 0x00800000; // update ALL_TESTS if this is defined
const int TBD_7  = 0x01000000; // update ALL_TESTS if this is defined
const int TBD_6  = 0x02000000; // update ALL_TESTS if this is defined
const int TBD_5  = 0x04000000; // update ALL_TESTS if this is defined
const int TBD_4  = 0x08000000; // update ALL_TESTS if this is defined
const int TBD_3  = 0x10000000; // update ALL_TESTS if this is defined
const int TBD_2  = 0x20000000; // update ALL_TESTS if this is defined
const int TBD_1  = 0x40000000; // update ALL_TESTS if this is defined
const int TBD_0  = 0x80000000; // update ALL_TESTS if this is defined

const int ALL_TESTS = 0x00007FFF;

// The length of this array determines which tests are run for "all tests"
const char * test_name[] =
  {
    "wchar_to_server",
    "wstring_to_server",
    "warray_to_server",
    "any(wchar)_to_server",
    "wstring_exception",
    "wchar_from_server",
    "wstring_from_server",
    "warray_from_server",
    "any(wchar)_from_server",
    "any(wstring)_to_server",
    "any(wstring)_from_server",
    "any(wstring)_echo",
    "wstructseq_from_server",
    "wstructseq_to_server",
    "nul_wstring_to_server"
  };

const int LAST_TEST = sizeof (test_name) / sizeof (test_name[0]);
const ACE_TCHAR *ior = ACE_TEXT("file://IOR");
int tests_to_run = 0;
int verbose = 0;
int kill_server = 0;
int data_set = 0;

#if defined (ACE_HAS_WCHAR) || defined (ACE_HAS_XPG4_MULTIBYTE_CHAR)

wchar_reference ref;

void
assign_wstruct (short key, interop::wstruct & ws)
{
    ws.st_char = ref.get_wchar(key);
    ws.st_string = CORBA::wstring_dup(ref.get_wstring(key));
    ref.assign_warray(key, ws.st_array);
    ws.st_any <<= CORBA::wstring_dup(ref.get_wstring(key));
}


CORBA::Boolean
run_one_test (interop::WChar_Passer_ptr server,
              int test_num)
{
  switch (test_num)
    {
    case WCHAR_TO_SERVER:
      return server->wchar_to_server (ref.get_wchar(data_set), data_set);
    case WSTRING_TO_SERVER:
      return server->wstring_to_server (ref.get_wstring(data_set), data_set);
    case WARRAY_TO_SERVER:
      return server->warray_to_server (ref.get_warray(data_set), data_set);
    case ANY_WCHAR_TO_SERVER:
      {
        CORBA::Any a;
        a <<= CORBA::Any::from_wchar(ref.get_wchar(data_set));
        return server->any_to_server (a,data_set);
      }
    case WSTRING_EXCEPTION:
      {
        try
          {
            server->exception_test(data_set);
          }
        catch (const interop::WChar_Passer::WStringException& e)
          {
            return ref.match_except (data_set,e.why.in());
          }
        break;
      }
    case WCHAR_FROM_SERVER:
      {
        CORBA::WChar wc = server->wchar_from_server (data_set);
        return ref.match_wchar (data_set,wc);
      }
    case WSTRING_FROM_SERVER:
      {
        CORBA::WString_var ws =
          server->wstring_from_server (data_set);
        return ref.match_wstring (data_set,ws.in());
      }
    case WARRAY_FROM_SERVER:
      {
        interop::warray_var wa =
          server->warray_from_server (data_set);
        return ref.match_warray (data_set,wa.in());
      }
    case ANY_WCHAR_FROM_SERVER:
      {
        CORBA::WChar wc;
        CORBA::Any_var test = server->any_from_server (data_set,
                                                       interop::is_wchar);
        if (test >>= CORBA::Any::to_wchar(wc))
          {
            return ref.match_wchar (data_set,wc);
          }
        return 0;
      }
    case ANY_WSTRING_TO_SERVER:
      {
        CORBA::Any a;
        a <<= ref.get_wstring(data_set);
        return server->any_to_server (a,data_set);
      }
    case ANY_WSTRING_FROM_SERVER:
      {
        const CORBA::WChar *ws;
        CORBA::Any_var test = server->any_from_server (data_set,
                                                       interop::is_wstring);
        if (test >>= ws)
          {
            return ref.match_wstring (data_set,ws);
          }
        return 0;
      }
    case ANY_WSTRING_ECHO:
      {
        CORBA::Any a;
        a <<= ref.get_wstring(data_set);
        const CORBA::WChar *ws;
        CORBA::Any_var test = server->any_echo (a);
        if (test >>= ws)
          {
            return ref.match_wstring (data_set,ws);
          }
        return 0;
      }
    case WSTRUCTSEQ_FROM_SERVER:
      {
        interop::wstructseq_var wsList =
          server->wstructseq_from_server(data_set);
        int result = 1;

        for (CORBA::ULong i = 0; i < wsList->length(); i++)
          result &= ref.match_wstring (data_set,
                                       wsList[i].st_string.in());

        return result;
      }
    case WSTRUCTSEQ_TO_SERVER:
      {
        interop::wstructseq_var wsList = new interop::wstructseq;
        wsList->length(5);
        for (CORBA::ULong i = 0; i < wsList->length(); ++i)
        {
          assign_wstruct(data_set, (*wsList)[i]);
        }
        return server->wstructseq_to_server(wsList.in(), data_set);
      }
    case NUL_WSTRING_TO_SERVER:
      return server->wstring_to_server (L"", -1);
    default:
      break;
    }
  return 0;
}

short
run_tests (interop::WChar_Passer_ptr server)
{
  short successes = 0;
  short numtests = 0;
  ref.set_verbose (verbose);
  int t = 1;
  for (int i = 0; i < LAST_TEST; i++, t <<= 1)
    if ((tests_to_run & t) == t)
      {
        CORBA::Boolean result = run_one_test (server,t);
        ++numtests;
        if (result) ++successes;
        if (verbose)
          ACE_DEBUG ((LM_DEBUG, "%s[%d] %s\n",
                      test_name[i],data_set,
                      (result ? "passed" : "failed")));
      }
  if (verbose)
    ACE_DEBUG ((LM_DEBUG, "Total of %d successes out of %d tests\n",
                successes, numtests));
  return successes == numtests;
}
#endif // ACE_HAS_WCHAR || ACE_HAS_XPG4_MULTIBYTE_CHAR

int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("k:t:vx"));
  int c;
  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'k':
        ior = get_opts.opt_arg ();
        break;
      case 't':
        {
          int tnum = ACE_OS::atoi(get_opts.opt_arg());
          if (tnum >= 0 && tnum < LAST_TEST)
            tests_to_run |= (1 << tnum);
          else
            ACE_ERROR_RETURN ((LM_ERROR,"test %d is out of range\n",tnum),-1);
          break;
        }
      case 'v':
        verbose = 1;
        break;
      case 'x':
        kill_server = 1;
        break;
      case '?':
      default:
        {
          ACE_ERROR ((LM_ERROR,
                      "usage:  %s "
                      "-k <ior> "
                      "[-t <0 <= test_num < %d>][ -t ...] "
                      "[-v] "
                      "[-x] "
                      "\n",
                      argv [0], LAST_TEST));
          ACE_ERROR ((LM_ERROR, "Available tests:\n"));
          for (int i = 0; i < LAST_TEST; i++)
            ACE_ERROR ((LM_ERROR, " %d - %s\n",i, test_name[i]));
          return -1;
        }
      }

  if (tests_to_run == 0)
    tests_to_run = ALL_TESTS;
  // Indicates successful parsing of the command line
  return 0;
}

int
ACE_TMAIN( int argc, ACE_TCHAR *argv[] )
{
#if (!defined ACE_HAS_WCHAR) && (!defined ACE_HAS_XPG4_MULTIBYTE_CHAR)
  ACE_UNUSED_ARG (argc);
  ACE_UNUSED_ARG (argv);
  ACE_ERROR_RETURN ((LM_ERROR,ACE_TEXT ("This test requires wchar support\n")),0);
#else
  try
  {
    // Initialize orb
    CORBA::ORB_var orb = CORBA::ORB_init(argc, argv);
    if (parse_args(argc, argv) == -1)
      return 0;

    // Destringify ior
    CORBA::Object_var obj = orb->string_to_object(ior);
    if( CORBA::is_nil( obj.in() ) )
      ACE_ERROR_RETURN ((LM_ERROR,
                         "arg is not a valid ior sting"),
                        -1);

    // Narrow
    interop::WChar_Passer_var server =
      interop::WChar_Passer::_narrow( obj.in());

    if( CORBA::is_nil( server.in() ))
      ACE_ERROR_RETURN ((LM_ERROR,
                         "arg is not a interop::WChar_Passer reference\n"),
                        -1);

    short result = run_tests (server.in());
    CORBA::String_var server_orb =
      server->orb_name();
    ACE_ERROR ((LM_ERROR,
                "wchar_interop test (TAO client, %C server) %C\n",
                server_orb.in(),
                (result ? "passed" : "failed")));
    if (kill_server)
    {
      server->shutdown();
    }
  }
  catch (const CORBA::Exception& ex)
  {
    ex._tao_print_exception ("Uncaught CORBA exception: ");
    return 1;
  }
  return 0;
#endif
}
