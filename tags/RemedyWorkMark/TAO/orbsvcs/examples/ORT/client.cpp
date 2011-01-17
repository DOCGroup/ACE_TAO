// -*- C++ -*-
// $Id$

#include "sum_serverC.h"
#include "ace/Get_Opt.h"

const ACE_TCHAR *ior = ACE_TEXT("file://test.ior");

int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("k:"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'k':
        ior = get_opts.optarg;
        break;
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Usage: %s "
                           "-k IOR "
                           "\n",
                           argv[0]),
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
        CORBA::ORB_init (argc, argv, "client_sum_orb");

      if (parse_args (argc, argv) != 0)
        return 1;

      CORBA::Object_var obj =
        orb->string_to_object (ior);

      ORT::sum_server_var server =
        ORT::sum_server::_narrow (obj.in ());

      if (CORBA::is_nil (server.in ()))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Object reference <%s> is nil.\n",
                             ior),
                            1);
        }

      CORBA::ULong a = 5;
      CORBA::ULong b = 3;

      CORBA::ULong result = server->add_variables (a,
                                                   b);

      if (result != 8)
        ACE_DEBUG ((LM_DEBUG,
                    "Error: Add Variables did not return the right value\n"));
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("ORT example on client side :");
      return -1;
    }

  return 0;
}
