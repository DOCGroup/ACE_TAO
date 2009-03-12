// $Id$

#include "WorkerC.h"
#include "ace/Get_Opt.h"

const ACE_TCHAR *first_ior = ACE_TEXT ("file://first.ior");

int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("k:"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'k':
        first_ior = get_opts.opt_arg ();
        break;
      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s"
                           "-k <ior> "
                           "\n",
                           argv [0]),
                          -1);
      }
  // Indicates sucessful parsing of the command line
  return 0;
}

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  try
    {
      CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);

      if (parse_args (argc, argv) != 0)
        return 1;

      CORBA::Object_var tmp = orb->string_to_object(first_ior);

      DeCoRAM::Worker_var worker = DeCoRAM::Worker::_narrow(tmp.in ());

      if (CORBA::is_nil (worker.in ()))
        {
          ACE_ERROR_RETURN ((LM_DEBUG,
                             "Nil Worker reference <%s>\n",
                             first_ior),
                            1);
        }

      worker->stop ();

      orb->destroy ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Exception caught:");
      return 1;
    }

  return 0;
}
