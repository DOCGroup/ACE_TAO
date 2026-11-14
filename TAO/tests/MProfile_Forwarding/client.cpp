#include "ace/SString.h"
#include "ace/Get_Opt.h"
#include "ace/OS_NS_unistd.h"
#include "testC.h"

struct Client_Options
{
  ACE_TString ior_input_file;
};

int
parse_args (int argc, ACE_TCHAR *argv[], Client_Options &options)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT ("o:"));
  int c {};

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'o':
        options.ior_input_file = get_opts.opt_arg ();
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
  // Indicates successful parsing of the command line
  return 0;
}

void
run_test (Simple_Server_ptr const server);

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try
    {
      Client_Options options;
      CORBA::ORB_var const orb = CORBA::ORB_init (argc, argv);

      if (parse_args (argc, argv, options) == -1)
        return -1;

      CORBA::Object_var const objref =
        orb->string_to_object (options.ior_input_file.c_str ());

      if (objref.in () == nullptr)
        {
          ACE_ERROR_RETURN  ((LM_ERROR,
                              "The received objref is nullptr\n"),
                             -1);
        }

      Simple_Server_var const server =
        Simple_Server::_narrow (objref.in ());

      if (CORBA::is_nil (server.in ()))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Object reference  is nil\n"),
                            -1);
        }

      run_test (server.in ());
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("Caught an exception\n");
      return -1;
    }
  return 0;
}

void
run_test (Simple_Server_ptr const server)
{
  for (int loop {}; loop < 10; ++loop)
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
      catch (CORBA::UserException const &x)
        {
          x._tao_print_exception ("Caught an ");
        }
      catch (CORBA::TRANSIENT const &t)
        {
          t._tao_print_exception ("Caught Exception");
        }
      catch (CORBA::COMM_FAILURE const &f)
        {
          f._tao_print_exception ("A (sort of) expected COMM_FAILURE");
          ACE_DEBUG ((LM_DEBUG,
                      "Automagically re-issuing request on COMM_FAILURE\n"));
        }
      catch (CORBA::Exception const &ex)
        {
          ex._tao_print_exception ("Unexpected exception");
          throw;
        }
    }
}
