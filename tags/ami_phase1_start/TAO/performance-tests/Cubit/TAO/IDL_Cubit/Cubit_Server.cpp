// $Id$

#include "Cubit_Server.h"
#include "tao/TAO_Internal.h"

ACE_RCSID(IDL_Cubit, Cubit_Server, "$Id$")

Cubit_Server::Cubit_Server (void)
  : ior_output_file_ (0)
{
}

int
Cubit_Server::parse_args (void)
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
Cubit_Server::init (int argc,
                    char** argv,
                    CORBA::Environment& ACE_TRY_ENV)
{
  // Call the init of <TAO_ORB_Manager> to initialize the ORB and
  // create a child POA under the root POA.
  if (this->orb_manager_.init_child_poa (argc,
                                         argv,
                                         "child_poa",
                                         ACE_TRY_ENV) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "%p\n",
                       "init_child_poa"),
                      -1);
  ACE_CHECK_RETURN (-1);
  this->argc_ = argc;
  this->argv_ = argv;

  this->parse_args ();
  // @@ Check for the return value here.

  // Get the orb
  CORBA::ORB_var orb = this->orb_manager_.orb ();

  // Now create the implementations
  this->factory_impl_ = new Cubit_Factory_i (orb.in ());

  this->factory_id_ =
    this->orb_manager_.activate_under_child_poa ("factory",
                                                 this->factory_impl_,
                                                 ACE_TRY_ENV);
  ACE_CHECK_RETURN (-1);

  ACE_DEBUG ((LM_DEBUG,
              "The IOR is: <%s>\n",
              this->factory_id_.in ()));

  if (this->ior_output_file_)
    {
      ACE_OS::fprintf (this->ior_output_file_,
                       "%s",
                       this->factory_id_.in ());
      ACE_OS::fclose (this->ior_output_file_);
    }

  return 0;
}

int
Cubit_Server::run (CORBA::Environment& env)
{
  if (this->orb_manager_.run (env) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Cubit_Server::run"),
                      -1);
  return 0;
}

Cubit_Server::~Cubit_Server (void)
{
  if (this->factory_id_.in ())
    this->orb_manager_.deactivate_under_child_poa (this->factory_id_.in ());

  delete this->factory_impl_;
}
