
//=============================================================================
/**
 *  @file     client.cpp
 *
 *  $Id$
 *
 *   This is a client implementation.
 *
 *
 *  @author  Irfan Pyarali
 */
//=============================================================================


#include "ace/Get_Opt.h"
#include "testC.h"
#include "ace/OS_NS_string.h"

static const ACE_TCHAR *IOR = ACE_TEXT ("file://ior");
static int shutdown_server = 0;

static int
parse_args (int argc, ACE_TCHAR **argv)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("k:x"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'k':
        IOR = ACE_OS::strdup (get_opts.opt_arg ());
        break;

      case 'x':
        shutdown_server = 1;
        break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-k IOR "
                           "-x shutdown server "
                           "\n",
                           argv [0]),
                          -1);
      }

  if (IOR == 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Please specify the IOR\n"), -1);

  return 0;
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{

  try
    {
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv);

      int parse_args_result =
        parse_args (argc, argv);
      if (parse_args_result != 0)
        return parse_args_result;

      CORBA::Object_var object =
        orb->string_to_object (IOR);

      test_var test1 =
        test::_narrow (object.in ());

      test1->method ();

      test_var test2 =
        test1->create_POA ();

      test2->method ();

      test1->destroy_POA ();

      test_var test3 =
        test1->create_POA ();

      test2->method ();

      test3->method ();

      if (shutdown_server)
        {
          test1->shutdown ();
        }

      orb->destroy ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Error!");
      return -1;
    }

  return 0;
}
