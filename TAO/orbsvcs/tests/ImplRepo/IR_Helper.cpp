// $Id$

#include "IR_Helper.h"
#include "ace/Get_Opt.h"
#include "ace/Read_Buffer.h"
#include "orbsvcs/PingS.h"
#include "tao/IIOP_Profile.h"

class Ping_i: public POA_Ping_Object
  // = TITLE
  //    Ping Object Implementation
  //
  // = DESCRIPTION
  //    Implementation Repository uses this to check to see if the server
  //    is still running.
{
public:
  Ping_i (int debug = 0) : debug_ (debug) {}

  virtual void ping (CORBA::Environment &env = CORBA_Environment::default_environment ())
    {
      if (this->debug_)
        ACE_DEBUG ((LM_DEBUG, "Pong!\n"));

      ACE_UNUSED_ARG (env);
      // Does nothing, just returns.
    }
private:
  int debug_;
};

IR_Helper::IR_Helper (char *server_name, 
                      PortableServer::POA_ptr poa, 
                      CORBA::ORB_ptr orb,
                      int debug)
  : name_ (ACE::strnew (server_name)),
    ir_key_ (0),
    ir_addr_ (0),
    ping_ (new Ping_i (debug)),
    implrepo_ (0),
    poa_ (poa),
    orb_ (orb),
    debug_ (debug)
{
  
  TAO_TRY
    {
      this->read_ir_ior (TAO_TRY_ENV);
      TAO_CHECK_ENV;

      // Resolve the IR.
      CORBA::Object_var implrepo_object =
        this->orb_->string_to_object (this->ir_key_, TAO_TRY_ENV);
      TAO_CHECK_ENV;

      if (CORBA::is_nil (implrepo_object.in ()))
        ACE_ERROR ((LM_ERROR,
                    "invalid implrepo key <%s>\n",
                    this->ir_key_));

      this->implrepo_ = 
        Implementation_Repository::_narrow (implrepo_object.in(), TAO_TRY_ENV);
      TAO_CHECK_ENV;

      // Now register the Ping Object
       PortableServer::ObjectId_var ping_id =
      PortableServer::string_to_ObjectId ("ping");

      this->poa_->activate_object_with_id (ping_id.in (),
                                           this->ping_,
                                           TAO_TRY_ENV);
      TAO_CHECK_ENV;

      this->ping_ptr_ = 
        this->poa_->id_to_reference (ping_id.in (),
                                     TAO_TRY_ENV);
      TAO_CHECK_ENV;
    }
  TAO_CATCHANY
    {
      TAO_TRY_ENV.print_exception ("IR_Helper::IR_Helper");
      return;
    }
  TAO_ENDTRY;
}

IR_Helper::~IR_Helper ()
{
  delete this->name_;
  delete this->ir_key_;

  // @@ Delete the two objects
}

int
IR_Helper::register_server (const char *comm_line, 
                            const char *environment, 
                            const char *working_dir, 
                            CORBA_Environment &TAO_IN_ENV)
{
  TAO_TRY 
    {
      CORBA::Object_var implrepo_object =
        this->orb_->string_to_object (this->ir_key_, TAO_TRY_ENV);
      TAO_CHECK_ENV;

      Implementation_Repository *ImplRepo = 
        Implementation_Repository::_narrow (implrepo_object.in(), TAO_TRY_ENV);
      TAO_CHECK_ENV;

      if (CORBA::is_nil (implrepo_object.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           "invalid implrepo key <%s>\n",
                           this->ir_key_),
                          -1);

      Implementation_Repository::Process_Options proc_opts;

      proc_opts.command_line_ = CORBA::string_dup (comm_line);
      proc_opts.environment_ = CORBA::string_dup (environment);
      proc_opts.working_directory_ = CORBA::string_dup (working_dir);

      ImplRepo->reregister_server (this->name_, proc_opts, TAO_TRY_ENV);
    }
  TAO_CATCHANY
    {
      TAO_TRY_ENV.print_exception ("IR_Helper::register_server");
      return -1;
    }
  TAO_ENDTRY;
  return 0;
}

int
IR_Helper::read_ir_ior (CORBA_Environment &TAO_IN_ENV)
{
  ACE_UNUSED_ARG (TAO_IN_ENV);
  
  // Open the file for reading.
  // @@ Hard-coded name is bad.  Need to fix.
  ACE_HANDLE f_handle_ = ACE_OS::open ("implrepo.ior", 0);

  if (f_handle_ == ACE_INVALID_HANDLE)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Unable to open %s for writing: implrepo.ior\n"),
                      -1);

  ACE_Read_Buffer ior_buffer (f_handle_);
  this->ir_key_ = ior_buffer.read ();

  if (this->ir_key_ == 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Unable to allocate memory to read ior: %p\n"),
                      -1);

  ACE_OS::close (f_handle_);
  return 0;
}


void
IR_Helper::notify_startup (CORBA_Environment &TAO_IN_ENV)
{
  // Get our host and port and convert it to something we can use.
  ACE_INET_Addr my_addr = TAO_ORB_Core_instance ()->orb_params ()->addr ();
  Implementation_Repository::INET_Addr my_ir_addr;
  my_ir_addr.port_ = my_addr.get_port_number ();
  my_ir_addr.host_ = CORBA::string_dup (my_addr.get_host_name ());

  TAO_TRY 
    {
      delete this->ir_addr_;
      this->ir_addr_ = this->implrepo_->server_is_running (this->name_, 
                                                           my_ir_addr,
                                                           this->ping_ptr_,
                                                           TAO_TRY_ENV);
      TAO_CHECK_ENV;
    }
  TAO_CATCHANY
    {
      TAO_RETHROW;
    }
  TAO_ENDTRY;
}


// Notify the IR that the server has been shut down.
void 
IR_Helper::notify_shutdown (CORBA_Environment &TAO_IN_ENV)
{
  TAO_TRY
    {
      this->implrepo_->server_is_shutting_down (this->name_, TAO_TRY_ENV);
      TAO_CHECK_ENV;
    }
  TAO_CATCHANY
    {
      TAO_RETHROW;
    }
  TAO_ENDTRY;
}

void
IR_Helper::change_object (CORBA::Object_ptr obj, CORBA_Environment &TAO_IN_ENV)
{
  if ( obj 
    && obj->_stubobj () 
    && obj->_stubobj ()->profile_in_use ()
    && this->implrepo_ 
    && this->implrepo_->_stubobj () 
    && this->implrepo_->_stubobj ()->profile_in_use ()  )
    {
      TAO_IIOP_Profile *implrepo_pfile =
          ACE_dynamic_cast (TAO_IIOP_Profile *, 
                            this->implrepo_->_stubobj ()->profile_in_use ());
      TAO_IIOP_Profile *iiop_pfile =
          ACE_dynamic_cast (TAO_IIOP_Profile *, 
                            obj->_stubobj ()->profile_in_use ());
    
      // @@ Only same host for now
      iiop_pfile->port (implrepo_pfile->port ());
    }
}
