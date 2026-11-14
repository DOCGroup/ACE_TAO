#include "PP_Test_Server.h"
#include "tao/TAO_Internal.h"
#include "tao/debug.h"
#include "ace/OS_NS_stdio.h"

int
PP_Test_Server::parse_args (int argc, ACE_TCHAR **argv)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("do:"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'd':  // debug flag.
        ++TAO_debug_level;
        break;
      case 'o': // output the IOR to a file.
        this->ior_output_file_name_ = get_opts.opt_arg ();
        break;
      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s"
                           " [-d]"
                           " [-o] <ior_output_file>"
                           "\n",
                           argv [0]),
                          -1);
      }

  return 0;
}

int
PP_Test_Server::init (int argc, ACE_TCHAR **argv)
{
  if (this->orb_manager_.init_child_poa (argc, argv, "child_poa") == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "%p\n",
                       "init_child_poa"),
                      -1);

  if (this->parse_args (argc, argv) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "%p\n",
                       "parse_args"),
                      -1);

  CORBA::ORB_var orb = this->orb_manager_.orb ();
  this->factory_impl_ = std::make_unique<Pluggable_Test_Factory_i> (orb.in ());

  this->factory_id_ =
    this->orb_manager_.activate_under_child_poa ("factory",
                                                 this->factory_impl_.get ());

  ACE_DEBUG ((LM_DEBUG,
              "The IOR is: <%s>\n",
              this->factory_id_.in ()));

  if (!this->ior_output_file_name_.empty ())
    {
      std::unique_ptr<FILE, int (*) (FILE *)> output_file (
        ACE_OS::fopen (this->ior_output_file_name_.c_str (), "w"), ACE_OS::fclose);
      if (!output_file)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Unable to open %s for writing: %p\n",
                           this->ior_output_file_name_.c_str ()),
                          -1);

      ACE_OS::fprintf (output_file.get (), "%s", this->factory_id_.in ());
    }

  return 0;
}

int
PP_Test_Server::run ()
{
  const int result = this->orb_manager_.run ();
  if (result == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "PP_Test_Server::run"),
                      -1);
  return 0;
}

PP_Test_Server::~PP_Test_Server ()
{
  if (this->factory_id_.in ())
    this->orb_manager_.deactivate_under_child_poa (this->factory_id_.in ());
}
