// $Id$

#include "ace/SString.h"
#include "ace/Get_Opt.h"
#include "ace/OS_NS_unistd.h"
#include "testC.h"

const ACE_TCHAR *ior_input_file = 0;

int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("o:"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'o':
        ior_input_file = get_opts.opt_arg ();
        break;
      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-o <iorfile>"
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

      if (parse_args (argc, argv) == -1)
        return -1;

      CORBA::Object_var objref =
        orb->string_to_object (ior_input_file);

      if (objref.in () == 0)
        {
          //FUZZ: disable check_for_NULL
          ACE_ERROR_RETURN  ((LM_ERROR,
                              "The received objref is NULL\n"),
                             -1);
          //FUZZ: enable check_for_NULL
        }

      Simple_Server_var server =
        Simple_Server::_narrow (objref.in ());

      if (CORBA::is_nil (server.in ()))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Object reference  is nil\n"),
                            -1);
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
          ACE_DEBUG ((LM_DEBUG,
                      "About to make remote call\n"));
          ACE_OS::sleep (2);

          // Make a remote call
          server->remote_call ();

          /*ACE_DEBUG ((LM_DEBUG,
                      "Kill  the primary . . . "));
          ACE_OS::sleep (25);
          ACE_DEBUG ((LM_DEBUG, " hope you did\n")); */
          ACE_DEBUG ((LM_DEBUG,
                      "I am going to shutdown\n"));
          server->shutdown ();
          ACE_OS::sleep (23);
        }
      catch (const CORBA::UserException& x)
        {
          x._tao_print_exception ("Caught an ");
        }
      catch (const CORBA::TRANSIENT& t)
        {
          t._tao_print_exception ("Caught Exception");
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
