
//=============================================================================
/**
 *  @file     client.cpp
 *
 *  $Id$
 *
 *   This is the client program that tests TAO's Smart Proxy extension.
 *
 *
 *  @author  Kirthika Parameswaran <kirthika@cs.wustl.edu>
 */
//=============================================================================


#define ACE_BUILD_SVC_DLL
#include "ace/Get_Opt.h"
#include "testC.h"
#include "ace/OS_NS_string.h"

const ACE_TCHAR *ior = ACE_TEXT("file://test.ior");

int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("i:"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'i':
        ior = ACE_OS::strdup (get_opts.opt_arg ());
      break;
      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-i "
                           "\n",
                           argv [0]),
                          -1);
      }
  return 0;
}


int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  try
    {
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc,
                         argv);

      if (parse_args (argc, argv) != 0)
        return 1;

       CORBA::Object_var object =
        orb->string_to_object (ior);

      // To use the smart proxy just enter it as a svc.conf
      // entry.

      Test_var server =
        Test::_narrow (object.in ());

      if (CORBA::is_nil (server.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Object reference <%s> is nil.\n",
                           ior),
                          1);

      server->method (0);

      server->shutdown ();

    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Client-side exception:");
      return 1;
    }

  return 0;
}


