// $Id$

#include "PP_Test_Server.h"
#include "tao/TAO_Internal.h"

ACE_RCSID(IDL_Cubit, Cubit_Server, "$Id$")

PP_Test_Server::PP_Test_Server (void)
  : ior_output_file_ (0)
{
}

int
PP_Test_Server::parse_args (void)
{
  ACE_Get_Opt get_opts (argc_, argv_, "do:");
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
                           "\n",
                           argv_ [0]),
                          1);
      }

  // Indicates successful parsing of command line.
  return 0;
}

int
PP_Test_Server::init (int argc,
                      char** argv,
                      CORBA::Environment& env)
{
  // Call the init of <TAO_ORB_Manager> to initialize the ORB and
  // create a child POA under the root POA.
  if (this->orb_manager_.init_child_poa (argc,
                                         argv,
                                         "child_poa",
                                         env) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "%p\n",
                       "init_child_poa"),
                      -1);
  TAO_CHECK_ENV_RETURN (env,-1);
  this->argc_ = argc;
  this->argv_ = argv;

  this->parse_args ();
  // @@ Check for the return value here.

  // Get the orb
  CORBA::ORB_var orb = this->orb_manager_.orb ();

  // Now create the implementations
  this->factory_impl_ = new Pluggable_Test_Factory_i (orb.in ());

  CORBA::String_var str  =
    this->orb_manager_.activate_under_child_poa ("factory",
                                                 this->factory_impl_,
                                                 env);
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
PP_Test_Server::run (CORBA::Environment& env)
{
  if (this->orb_manager_.run (env) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "PP_Test_Server::run"),
                      -1);
  return 0;
}

PP_Test_Server::~PP_Test_Server (void)
{
  delete this->factory_impl_;
}
