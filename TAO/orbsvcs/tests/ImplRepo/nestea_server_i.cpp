// $Id$

#include "nestea_server_i.h"
#include "ace/Get_Opt.h"
#include "ace/Read_Buffer.h"

ACE_RCSID(ImplRepo, nestea_server_i, "$Id$")

const char *NESTEA_DATA_FILENAME = "nestea.dat";

Nestea_Server_i::Nestea_Server_i (const char *filename)
  : server_impl_ (0),
    ior_output_file_ (0),
    ir_helper_ (0),
    register_with_ir_ (0),
    use_ir_ (0)
{
  // Nothing
}

Nestea_Server_i::~Nestea_Server_i (void)
{
  delete this->ir_helper_;
  delete this->server_impl_;
}

int
Nestea_Server_i::parse_args (void)
{
  ACE_Get_Opt get_opts (this->argc_, this->argv_, "do:ir");
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'd':  // debug flag.
        TAO_debug_level++;
        break;
      case 'r':  // Register restart information with the IR.
        this->register_with_ir_ = 1;
        break;
      case 'i':  // Use the IR
        this->use_ir_ = 1;
        break;
      case 'o':  // output the IOR to a file.
        this->ior_output_file_ = ACE_OS::fopen (get_opts.optarg, "w");
        if (this->ior_output_file_ == 0)
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Unable to open %s for writing: %p\n",
                             get_opts.optarg), -1);
        break;
      case '?':  // display help for use of the server.
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s"
                           " [-d]"
                           " [-i]"
                           " [-r]"
                           " [-o] <ior_output_file>"
                           "\n",
                           argv_ [0]),
                          1);
      }

  // Indicates successful parsing of command line.
  return 0;
}

int
Nestea_Server_i::init (int argc, char** argv, CORBA::Environment &TAO_IN_ENV)
{
  char poa_name[] = "nestea_server";

  TAO_TRY 
    {
      // Call the init of <TAO_ORB_Manager> to initialize the ORB and
      // create a child POA under the root POA.
      if (this->orb_manager_.init_child_poa (argc, argv, poa_name, TAO_TRY_ENV) == -1)
        ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "init_child_poa"), -1);

      TAO_CHECK_ENV;
    
      this->argc_ = argc;
      this->argv_ = argv;

      int retval = this->parse_args ();

      if (retval != 0)
        return retval;

      ACE_NEW_RETURN (this->server_impl_, Nestea_i (NESTEA_DATA_FILENAME, this->use_ir_), -1);

      CORBA::String_var server_str  =
        this->orb_manager_.activate_under_child_poa ("server",
                                                     this->server_impl_,
                                                     TAO_TRY_ENV);
      TAO_CHECK_ENV;

      if (this->use_ir_ == 1)
        {
          ACE_NEW_RETURN (this->ir_helper_, IR_Helper (poa_name,
                                                       this->orb_manager_.child_poa (),
                                                       this->orb_manager_.orb (),
                                                       TAO_debug_level),
                          -1);

          if (this->register_with_ir_ == 1)
            this->ir_helper_->register_server ("nestea_server -i");
        }

      PortableServer::ObjectId_var id =
        PortableServer::string_to_ObjectId ("server");

      CORBA::Object_var server_obj =
        this->orb_manager_.child_poa ()->id_to_reference (id.in (),
                                                          TAO_TRY_ENV);
      TAO_CHECK_ENV;

      if (this->use_ir_ == 1)
        {
          this->ir_helper_->change_object (server_obj, TAO_TRY_ENV);
          TAO_CHECK_ENV;
        }
       
      server_str =
        this->orb_manager_.orb ()->object_to_string (server_obj.in (),
                                                     TAO_TRY_ENV);
      TAO_CHECK_ENV;

      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG, "The IOR is: <%s>\n", server_str.in ()));

      if (this->ior_output_file_)
        {
          ACE_OS::fprintf (this->ior_output_file_, "%s", server_str.in ());
          ACE_OS::fclose (this->ior_output_file_);
        }
    }
  TAO_CATCHANY
    {
      TAO_TRY_ENV.print_exception ("Nestea_i::init");
      TAO_RETHROW_RETURN (-1);
    }
  TAO_ENDTRY;

  return 0;
}

int
Nestea_Server_i::run (CORBA::Environment& env)
{
  TAO_TRY
    {
      if (this->use_ir_ == 1)
        {
          this->ir_helper_->notify_startup (TAO_TRY_ENV);
          TAO_CHECK_ENV;
        }

      this->orb_manager_.run (TAO_TRY_ENV);
      TAO_CHECK_ENV;

      if (this->use_ir_ == 1)
        {
          this->ir_helper_->notify_shutdown (TAO_TRY_ENV);
          TAO_CHECK_ENV;
        }
    }
  TAO_CATCHANY
    {
      TAO_TRY_ENV.print_exception ("Nestea_Server_i::run");
      return -1;
    }
  TAO_ENDTRY;

  return 0;
}
