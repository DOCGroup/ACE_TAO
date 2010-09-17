// $Id$

#include "TestC.h"
#include "ace/Get_Opt.h"

#if defined (ACE_OPENVMS)
// need this to circumvent link error on OpenVMS
// has to do with interference in template instantiations
// because multiple executables are build in one directory
// reuising eachothers object files.
ACE_Time_Value dum = ACE_Time_Value::zero;
#endif

const ACE_TCHAR *ior = ACE_TEXT("file://test.ior");

int
parse_args (int argc, ACE_TCHAR *argv[]);

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  ACE_DEBUG ((LM_DEBUG,
             "Starting shutdown\n"));
  try
    {
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv);

      if (parse_args (argc, argv) != 0)
        return 1;

      CORBA::Object_var tmp =
        orb->string_to_object(ior);

      Test::Echo_Caller_var server =
        Test::Echo_Caller::_narrow(tmp.in ());

      if (CORBA::is_nil (server.in ()))
        {
          ACE_ERROR_RETURN ((LM_DEBUG,
                             "Nil Test::Echo_Caller reference <%s>\n",
                             ior),
                            1);
        }

      server->shutdown ();

      orb->destroy ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Exception caught:");
      return 1;
    }

  return 0;
}

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
