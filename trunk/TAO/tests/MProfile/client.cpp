// $Id$

#include "tao/IORManipulation/IORManip_Loader.h"
#include "ace/SString.h"
#include "ace/Get_Opt.h"
#include "ace/OS_NS_unistd.h"
#include "testC.h"

const ACE_TCHAR *ior = ACE_TEXT("file://test.ior");
int do_shutdown = 0;

const ACE_TCHAR *name = ACE_TEXT("file://amba.ior");

int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("xk:m:"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'x':
        do_shutdown = 1;
        break;

      case 'k':
        ior = get_opts.opt_arg ();
        break;

      case 'm':
        name = get_opts.opt_arg ();
        break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-k <ior> "
                           "-i <niterations> "
                           "\n",
                           argv [0]),
                          -1);
      }
  // Indicates sucessful parsing of the command line
  return 0;
}

void
run_test (Simple_Server_ptr server);

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{

  try
    {
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv);

      if (parse_args (argc, argv) != 0)
        return 1;

      // Primary server
      CORBA::Object_var object_primary =
        orb->string_to_object (ior);

      //Secondary server
      CORBA::Object_var object_secondary =
        orb->string_to_object (name);

      // Get an object reference for the ORBs IORManipultion object!
      CORBA::Object_ptr IORM =
        orb->resolve_initial_references (TAO_OBJID_IORMANIPULATION,
                                         0);

      TAO_IOP::TAO_IOR_Manipulation_ptr iorm =
        TAO_IOP::TAO_IOR_Manipulation::_narrow (IORM);

      TAO_IOP::TAO_IOR_Manipulation::IORList iors (2);
      iors.length(2);
      iors [0] = object_primary;
      iors [1] = object_secondary;

      CORBA::Object_var merged = iorm->merge_iors (iors);

      // Combined IOR stuff
      Simple_Server_var server =
        Simple_Server::_narrow (merged.in ());

      if (CORBA::is_nil (server.in ()))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Object reference <%s> is nil.\n",
                             ior),
                            1);
        }

      run_test (server.in ());
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Caught an exception\n");
      return -1;
    }
  return 0;
}

void run_test (Simple_Server_ptr server)
{
  for (int loop = 0; loop < 10; loop++)
    {
      try
        {
          // Make a remote call
          CORBA::Long ret =
            server->remote_call ();

          ACE_UNUSED_ARG (ret);

          ACE_OS::sleep (25);
        }
      catch (const CORBA::TRANSIENT& t)
        {
          if (t.completed () != CORBA::COMPLETED_NO)
            {
              t._tao_print_exception ("Unexpected kind of TRANSIENT");
            }
          else
            {
              ACE_DEBUG ((LM_DEBUG,
                          "The completed status %d\n", t.completed ()));
              ACE_DEBUG ((LM_DEBUG,
                          "Automagically re-issuing request on TRANSIENT\n"));
              ACE_OS::sleep (1);
            }
        }
      catch (const CORBA::COMM_FAILURE& f)
        {
          f._tao_print_exception ("A (sort of) expected COMM_FAILURE");
          ACE_DEBUG ((LM_DEBUG,
                      "Automagically re-issuing request on COMM_FAILURE\n"));
        }
      catch (const CORBA::Exception& ex)
        {
          ex._tao_print_exception ("Unexpected exception");
          throw;
        }
    }
}
