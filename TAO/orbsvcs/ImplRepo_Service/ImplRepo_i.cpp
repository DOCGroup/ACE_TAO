/* -*- C++ -*- */
// $Id$

#include "ImplRepo_i.h"
#include "ace/Get_Opt.h"
#include "ace/Read_Buffer.h"
#include "ace/Process.h"

ACE_RCSID(ImplRepo, ImplRepo_i, "$Id$")

ImplRepo_i::ImplRepo_i (void)
  : ior_output_file_ (0),
    server_key_ (0),
    server_impl_ (0),
    activator_ (0)
{
}

void ImplRepo_i::start (const char *server)
{
  char *cl;

  int status = this->repository_.get_comm_line (server, cl);

  if (status == 0)
  {
    ACE_DEBUG ((LM_DEBUG, "Starting %s\n", server));

    ACE_Process_Options proc_opts;
      
    proc_opts.command_line (cl);

    ACE_Process proc;

    proc.spawn (proc_opts);

    ACE_OS::sleep (2);
    delete [] cl;
  } 
  else
  {
    ACE_DEBUG ((LM_DEBUG, "ERROR starting %s\n", server));
  }
}

void ImplRepo_i::server_is_running (const char *server,
                                    CORBA::Object_ptr &obj,
                                    const Implementation_Repository::INET_Addr &addr,
                                    CORBA::Environment &_env)
{
  this->server_impl_->forward_to (obj);

  if (TAO_debug_level > 0)
    ACE_DEBUG ((LM_DEBUG, "Server is running!\n"));

  IIOP_Object *iiop_obj = ACE_dynamic_cast (IIOP_Object *, obj->_stubobj ());

  if (TAO_debug_level > 0)
    ACE_DEBUG ((LM_DEBUG, "The old IOR is: <%s>\n", this->orb_manager_.orb ()->object_to_string (obj, _env)));

  ACE_INET_Addr my_addr = TAO_ORB_Core_instance ()->addr ();

  iiop_obj->profile.port = my_addr.get_port_number ();

  if (TAO_debug_level > 0)
    ACE_DEBUG ((LM_DEBUG, "The new IOR is: <%s>\n", this->orb_manager_.orb ()->object_to_string (obj, _env)));

}


// Reads the Server factory ior from a file

int
ImplRepo_i::read_ior (char *filename)
{
  // Open the file for reading.
  ACE_HANDLE f_handle_ = ACE_OS::open (filename, 0);

  if (f_handle_ == ACE_INVALID_HANDLE)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Unable to open %s for writing: %p\n",
                       filename),
                      -1);
  ACE_Read_Buffer ior_buffer (f_handle_);
  this->server_key_ = ior_buffer.read ();

  if (this->server_key_ == 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Unable to allocate memory to read ior: %p\n"),
                      -1);

  ACE_OS::close (f_handle_);
  return 0;
}

int
ImplRepo_i::parse_args (void)
{
  ACE_Get_Opt get_opts (this->argc_, this->argv_, "df:o:");
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'd':  // debug flag.
        TAO_debug_level++;
        break;
      case 'o':  // output the IOR to a file.
        this->ior_output_file_ = ACE_OS::fopen (get_opts.optarg, "w");
        if (this->ior_output_file_ == 0)
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Unable to open %s for writing: %p\n",
                             get_opts.optarg), -1);
        break;
      case 'f': // read the IOR from the file.
        this->server_input_file_ = ACE::strnew (get_opts.optarg);
        break;
      case '?':  // display help for use of the server.
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s"
                           " [-d]"
                           " [-f] <server_ior_file>"
                           " [-o] <ior_output_file>"
                           "\n",
                           argv_ [0]),
                          1);
      }

  // Indicates successful parsing of command line.
  return 0;
}

int
ImplRepo_i::init (int argc, char **argv, CORBA::Environment &_env)
{
  TAO_TRY
  {
    // Call the init of <TAO_ORB_Manager> to initialize the ORB and
    // create a child POA under the root POA.
    if (this->orb_manager_.init_child_poa (argc, argv, "ir_poa", TAO_TRY_ENV) == -1)
      ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "init_child_poa"), -1);
    TAO_CHECK_ENV;

    this->argc_ = argc;
    this->argv_ = argv;

    int retval = this->parse_args ();

    if (retval != 0)
      return retval;

    ACE_NEW_RETURN (this->server_impl_, 
                    IR_Simple_i (this->orb_manager_.orb (),
                                 this->orb_manager_.child_poa (),
                                 this),
                    -1);

    CORBA::String_var str  =
      this->orb_manager_.activate (this->server_impl_);
    if (TAO_debug_level > 0)
      ACE_DEBUG ((LM_DEBUG, "The server IOR is: <%s>\n", str.in ()));

    if (this->ior_output_file_)
      {
        ACE_OS::fprintf (this->ior_output_file_, "%s", str.in ());
        ACE_OS::fclose (this->ior_output_file_);
      }

    CORBA::String_var ir_var  =
      this->orb_manager_.activate_under_child_poa ("implrepo",
                                                   this,
                                                   TAO_TRY_ENV);
    TAO_CHECK_ENV;

    if (TAO_debug_level > 0)
      ACE_DEBUG ((LM_DEBUG, "The IR IOR is: <%s>\n", ir_var.in ()));
  
    FILE *ir_file = ACE_OS::fopen ("implrepo.ior", "w");
    ACE_OS::fprintf (ir_file, "%s", ir_var.in ());
    ACE_OS::fclose (ir_file);

    ACE_NEW_RETURN (this->activator_, 
                    IR_Adapter_Activator(this->server_impl_),
                    -1);

    PortableServer::AdapterActivator_var activator =
      this->activator_->_this (TAO_TRY_ENV);
    TAO_CHECK_ENV;

    // Register the TAO_Adapter_Activator reference to be the RootPOA's
    // Adapter Activator.

    this->orb_manager_.root_poa ()->the_activator (activator.in (), TAO_TRY_ENV);
    TAO_CHECK_ENV;

  
  }
  TAO_CATCHANY
  {
    TAO_TRY_ENV.print_exception ("Server_i::init");
    TAO_RETHROW_RETURN (-1);
  }
  TAO_ENDTRY;
  return 0;
}

int
ImplRepo_i::run (CORBA::Environment& env)
{
  /*
  ACE_CString name = "firstPOA";
  PortableServer::POA_var first_poa =
    this->orb_manager_.root_poa()->find_POA (name.c_str (),
                        1,
                        env);
  if (env.exception () != 0)
    {
      env.print_exception ("PortableServer::POA::find_POA");
      return -1;
    }

  // Use the TAO_POA name_separator (which is '/') to find a childPOA
  // of firstPOA named secondPOA.

  name += TAO_POA::name_separator ();
  name += "secondPOA";
  PortableServer::POA_var second_poa =
    this->orb_manager_.root_poa()->find_POA (name.c_str (),
                        1,
                        env);
  if (env.exception () != 0)
    {
      env.print_exception ("PortableServer::POA::find_POA");
      return -1;
    }

  // Create a hierarchical string of POA names
  // eg. thirdPOA/forthPOA/fifthPOA thirdPOA being the root of the
  // hierarchy with forthPOA as its child and fifthPOA as its
  // grandchild.

  name = "thirdPOA";
  name += TAO_POA::name_separator ();
  name += "forthPOA";
  name += TAO_POA::name_separator ();
  name += "fifthPOA";

  // Try to find the fifth_poa by passing the hierarchy of POA names
  // resulting in the creation of third and forth POAs as well as the
  // fifth POA.

  PortableServer::POA_var fifth_poa =
    this->orb_manager_.root_poa()->find_POA (name.c_str (),
                        1,
                        env);
  if (env.exception () != 0)
    {
      env.print_exception ("PortableServer::POA::find_POA");
      return -1;
    }

  // Get the names of all the POAs

  CORBA::String_var root_poa_name =
    this->orb_manager_.root_poa()->the_name (env);
  if (env.exception () != 0)
    {
      env.print_exception ("PortableServer::POA::_narrow");
      return -1;
    }

  CORBA::String_var first_poa_name =
    first_poa->the_name (env);
  if (env.exception () != 0)
    {
      env.print_exception ("PortableServer::POA::_narrow");
      return -1;
    }

  CORBA::String_var second_poa_name =
    second_poa->the_name (env);
  if (env.exception () != 0)
    {
      env.print_exception ("PortableServer::POA::_narrow");
      return -1;
    }

  CORBA::String_var fifth_poa_name =
    fifth_poa->the_name (env);
  if (env.exception () != 0)
    {
      env.print_exception ("PortableServer::POA::_narrow");
      return -1;
    }


  ACE_DEBUG((LM_DEBUG,"%s\n%s\n%s\n%s\n",
       root_poa_name.in (),
       first_poa_name.in (),
       second_poa_name.in (),
       fifth_poa_name.in ()));

*/
  if (this->orb_manager_.run (env) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "IR_Server_i::run"), -1);
  return 0;
}

ImplRepo_i::~ImplRepo_i (void)
{
  if (this->server_impl_ != 0)
    delete this->server_impl_;
  if (this->activator_ != 0)
    delete this->activator_;
}

void 
ImplRepo_i::register_server (const char *server,
                             const Implementation_Repository::Process_Options &options,
                             CORBA::Environment &env)
{
  Repository::Record rec;
  rec.comm_line = options.command_line_;
  rec.env = options.environment_;
  rec.wdir = options.working_directory_;
  rec.ior = "";
  rec.ping_ior = "";

  int status = this->repository_.add (server, rec);

  if (status == -1)
  {
    ACE_DEBUG ((LM_DEBUG, "Server %s Already Registered\n", server));
    TAO_THROW_ENV (Implementation_Repository::Already_Registered (), env);
  }
  else
    ACE_DEBUG ((LM_DEBUG, "Server %s Successfully Registered\n", server));
  this->repository_.dump ();
}


IR_Adapter_Activator::IR_Adapter_Activator (IR_Simple_i_ptr servant)
: servant_ (servant)
{
  // Nothing
}


CORBA::Boolean 
IR_Adapter_Activator::unknown_adapter (PortableServer::POA_ptr parent,
                                       const char *name,
                                       CORBA_Environment &env)
{
  ACE_DEBUG ((LM_DEBUG, "Unknown Adapter called\n"));
    
  CORBA::PolicyList policies (4);
  policies.length (4); 

  // ID Assignment Policy
  policies[0] =
    parent->create_id_assignment_policy (PortableServer::USER_ID, env);
  if (env.exception () != 0)
    {
      env.print_exception ("PortableServer::POA::create_id_assignment_policy");
      return -1;
    }

  // Lifespan Policy
  policies[1] =
    parent->create_lifespan_policy (PortableServer::PERSISTENT, env);
  if (env.exception () != 0)
    {
      env.print_exception ("PortableServer::POA::create_lifespan_policy");
      return -1;
    }
  
  // Request Processing Policy
  policies[2] =
    parent->create_request_processing_policy (PortableServer::USE_DEFAULT_SERVANT, env);
  if (env.exception () != 0)
    {
      env.print_exception ("PortableServer::POA::create_request_processing_policy");
      return -1;
    }
  
  // Id Uniqueness Policy
  policies[3] =
    parent->create_id_uniqueness_policy (PortableServer::MULTIPLE_ID, env);
  if (env.exception () != 0)
    {
      env.print_exception ("PortableServer::POA::create_id_uniqueness_policy");
      return -1;
    }

  PortableServer::POA_var child = parent->create_POA (name,
                                                      PortableServer::POAManager::_nil (),
                                                      policies,
                                                      env);  
  if (env.exception () != 0)
    return 0;

  for (CORBA::ULong i = 0;
       i < policies.length () && env.exception () == 0;
       ++i)
    {
      CORBA::Policy_ptr policy = policies[i];
      policy->destroy (env);
    }  

  if (env.exception () != 0)
    {
      env.print_exception ("unknown_adapter::policy->destroy");
      return -1;
    }

    PortableServer::AdapterActivator_var activator = this->_this (env);
    if (env.exception () != 0)
      {
        child->destroy (0, 0, env);
        return 0;
      }

    child->the_activator (activator.in (), env);

    if (env.exception () != 0)
      {
        env.print_exception ("unknown_adapter, the_activator");
        child->destroy (0, 0, env);
        return 0;
      }

    child->set_servant (this->servant_, env);
    
    if (env.exception () != 0)
      {
        env.print_exception ("unknown_adapter, set_servant");
        child->destroy (0, 0, env);
        return 0;
      }

    // Finally everything is fine
    return 1;
}




// Constructor
IR_Simple_i::IR_Simple_i (CORBA::ORB_ptr orb_ptr,
                                PortableServer::POA_ptr poa_ptr,
                                ImplRepo_i *ir_impl)
  : ir_impl_ (ir_impl),
    orb_var_ (CORBA::ORB::_duplicate (orb_ptr)),
    poa_var_ (PortableServer::POA::_duplicate (poa_ptr)),
    forward_to_ptr_ (CORBA::Object::_nil ())
{
}

// Destructor

IR_Simple_i::~IR_Simple_i (void)
{
}

// Cube a long

CORBA::Long
IR_Simple_i::simple_method (CORBA::Long l, CORBA::Environment &env)
{
  return this->forward (env);
}

// Shutdown.

void IR_Simple_i::shutdown (CORBA::Environment &env)
{
  this->forward (env);
}

int IR_Simple_i::forward (CORBA::Environment &env)
{
  this->ir_impl_->start ("Simple_Server");

  ACE_DEBUG ((LM_DEBUG, "Forwarding to %s\n", 
    this->orb_var_->object_to_string (this->forward_to_ptr_)));

  if (!CORBA::is_nil (this->forward_to_ptr_))
    {
      env.exception (new PortableServer::ForwardRequest (this->forward_to_ptr_));
/*
      ACE_DEBUG ((LM_DEBUG, "Forwarding...\n"));
    
      PortableServer::ObjectId_var oid =
        this->poa_var_->servant_to_id (this, env);
      
      if (env.exception () != 0)
        return -1;

      PortableServer::Servant servant = this->poa_var_->_servant ();
      if (servant == 0)
        {
//          CORBA::Exception *exception = new Foo::Cannot_Forward;
//          env.exception (exception);
          return -1;
        }
      
      void *ptr = servant->_downcast ("IDL:PortableServer/POA:1.0");
      POA_PortableServer::POA *poa = (POA_PortableServer::POA *) ptr;
      TAO_POA *tao_poa = ACE_dynamic_cast (TAO_POA *, poa);
      
      tao_poa->forward_object (oid.in (),
                               this->forward_to_var_.in (),
                               env);*/
      return 0;
    }
  else
    {
      ACE_DEBUG ((LM_DEBUG, "Forward_to reference is nil.\n"));
//      CORBA::Exception *exception = new Foo::Cannot_Forward;
//      env.exception (exception);
      return -1;
    }
}

void IR_Simple_i::forward_to (CORBA::Object_ptr forward_to_ptr)
{
  IIOP_Object *iiop_obj = ACE_dynamic_cast (IIOP_Object *, forward_to_ptr->_stubobj ());
  IIOP::Profile *new_profile = new IIOP::Profile;
  *new_profile = iiop_obj->profile;
  IIOP_Object *new_iiop_obj = new IIOP_Object (iiop_obj->type_id, *new_profile);

  this->forward_to_ptr_ = 
    new CORBA_Object (new_iiop_obj, forward_to_ptr->_servant ());

}


