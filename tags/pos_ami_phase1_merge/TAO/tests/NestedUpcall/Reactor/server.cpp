// $Id$

#include "server.h"

ACE_RCSID(Reactor, server, "$Id$")

NestedUpCalls_Server::NestedUpCalls_Server (void)
  : ior_output_file_ (0)
{
}

int
NestedUpCalls_Server::parse_args (void)
{
  ACE_Get_Opt get_opts (argc_, argv_, "dn:o:");
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'd':  // debug flag.
        TAO_debug_level++;
        break;

      case 'o': // output the IOR to a file.
        this->ior_output_file_ = ACE_OS::fopen (get_opts.optarg, "w");
        if (this->ior_output_file_ == 0)
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Unable to open %s for writing: %p\n",
                             get_opts.optarg), -1);
        break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s"
                           " [-d]"
                           " [-o] <ior_output_file>"
                           " [-s]"
                           "\n",
                           argv_ [0]),
                          1);
      }

  // Indicates successful parsing of command line.
  return 0;
}

int
NestedUpCalls_Server::init (int argc,
                            char* argv[],
                            CORBA::Environment& ACE_TRY_ENV)
{
  // Call the init of TAO_ORB_Manager to create a child POA
  // under the root POA.
  this->orb_manager_.init_child_poa (argc,
                                     argv,
                                     "child_poa",
                                     ACE_TRY_ENV);

  ACE_CHECK_RETURN (-1);

  this->argc_ = argc;
  this->argv_ = argv;

  this->parse_args ();
  // ~~ check for the return value here

  CORBA::String_var str  =
    this->orb_manager_.activate_under_child_poa ("reactor",
                                                 &this->reactor_impl_,
                                                 ACE_TRY_ENV);
  ACE_CHECK_RETURN (-1);

  ACE_DEBUG ((LM_DEBUG,
              "The IOR is: <%s>\n",
              str.in ()));

  if (this->ior_output_file_)
    {
      ACE_OS::fprintf (this->ior_output_file_,
                       "%s",
                       str.in ());
      ACE_OS::fclose (this->ior_output_file_);
    }

  return 0;
}

int
NestedUpCalls_Server::run (CORBA::Environment& ACE_TRY_ENV)
{
  if (this->orb_manager_.run (ACE_TRY_ENV) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "NestedUpCalls_Server::run"),
                      -1);
  return 0;
}

NestedUpCalls_Server::~NestedUpCalls_Server (void)
{
}

int
main (int argc, char *argv[])
{
  NestedUpCalls_Server nested_up_calls_server;

  ACE_DEBUG ((LM_DEBUG,
              "\n \t NestedUpCalls:SERVER \n \n"));

  ACE_TRY_NEW_ENV
    {
      if (nested_up_calls_server.init (argc,argv,
                                       ACE_TRY_ENV) == -1)
        return 1;

      ACE_TRY_CHECK;

      nested_up_calls_server.run (ACE_TRY_ENV);
      ACE_TRY_CHECK;
    }
  ACE_CATCH (CORBA::SystemException, sysex)
    {
      ACE_PRINT_EXCEPTION (sysex, "System Exception");
      return 1;
    }
  ACE_CATCH (CORBA::UserException, userex)
    {
      ACE_PRINT_EXCEPTION (userex, "User Exception");
      return 1;
    }
  ACE_ENDTRY;

  return 0;
}
