#include "Cubit_Server.h"
#include "tao/TAO_Internal.h"
#include "tao/debug.h"
#include "ace/OS_NS_stdio.h"

int
Cubit_Server::parse_args ()
{
  ACE_Get_Opt get_opts (argc_, argv_, ACE_TEXT("df:"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'd':  // debug flag.
        TAO_debug_level++;
        break;

      case 'f': // output the IOR to a file.
        this->ior_output_file_.reset (ACE_OS::fopen (get_opts.opt_arg (), "w"));
        if (!this->ior_output_file_)
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Unable to open <%s> for writing: %p\n",
                             get_opts.opt_arg (),
                             ""),
                            -1);
        break;
      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s"
                           " [-d]"
                           " [-f] <ior_output_file>"
                           "\n",
                           argv_ [0]),
                          -1);
      }

  // Indicates successful parsing of command line.
  return 0;
}

int
Cubit_Server::init (int argc,
                    ACE_TCHAR** argv)
{
  // Call the init of <TAO_ORB_Manager> to initialize the ORB and
  // create a child POA under the root POA.
  if (this->orb_manager_.init_child_poa (argc,
                                         argv,
                                         "child_poa") == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "%p\n",
                       "init_child_poa"),
                      -1);
  this->argc_ = argc;
  this->argv_ = argv;

  if (this->parse_args () == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "%p\n",
                       "parse_args"),
                      -1);
  // Get the orb
  CORBA::ORB_var orb = this->orb_manager_.orb ();

  // Now create the implementations
  this->factory_impl_ = std::make_unique<Cubit_Factory_i> (orb.in ());

  PortableServer::POA_var root_poa = this->orb_manager_.root_poa ();
  this->factory_impl_->set_default_poa (root_poa.in ());

  this->factory_id_ =
    this->orb_manager_.activate_under_child_poa ("factory",
                                                 this->factory_impl_.get ());

  ACE_DEBUG ((LM_DEBUG,
              "The IOR is: <%C>\n",
              this->factory_id_.in ()));

  if (this->ior_output_file_)
    {
      ACE_OS::fprintf (this->ior_output_file_.get (),
                       "%s",
                       this->factory_id_.in ());
      this->ior_output_file_.reset ();
    }

  return 0;
}

int
Cubit_Server::run ()
{
  if (this->orb_manager_.run () == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Cubit_Server::run"),
                      -1);
  return 0;
}
