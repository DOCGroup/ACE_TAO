// $Id$

#include "orbsvcs/IR_Helper.h"
#include "tao/IIOP_Profile.h"
#include "tao/IIOP_Acceptor.h"
#include "tao/Acceptor_Registry.h"
#include "ace/Get_Opt.h"
#include "ace/Read_Buffer.h"

class ServerObject_i: public POA_ImplementationRepository::ServerObject
  // = TITLE
  //    IR Server Object Implementation
  //
  // = DESCRIPTION
  //    Implementation Repository uses this to communicate with the IR
  //    registered server.
{
public:
  ServerObject_i (CORBA::ORB_ptr orb, int debug = 0) : orb_ (orb), debug_ (debug) {}

  virtual void ping (CORBA::Environment &/*ACE_TRY_ENV = TAO_default_environment ()*/)
    ACE_THROW_SPEC (())
    {
      if (this->debug_)
        ACE_DEBUG ((LM_DEBUG, "Pong!\n"));
    }

  virtual void shutdown (CORBA::Environment &/*ACE_TRY_ENV = TAO_default_environment ()*/)
    ACE_THROW_SPEC (())
    {
      if (this->debug_)
        ACE_DEBUG ((LM_DEBUG, "ServerObject_i::Shutdown\n"));
 
      this->orb_->shutdown ();
    }
private:
  CORBA::ORB_var orb_;

  int debug_;
};

IR_Helper::IR_Helper (char *server_name,
                      PortableServer::POA_ptr poa,
                      CORBA::ORB_ptr orb,
                      int debug)
  : name_ (ACE::strnew (server_name)),
    ir_addr_ (0),
    server_object_ (new ServerObject_i (orb, debug)),
    implrepo_ (0),
    orb_ (CORBA::ORB::_duplicate (orb)),
    poa_ (PortableServer::POA::_duplicate (poa)),
    debug_ (debug)
{
  const char *exception_message = "Null Message";

  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      CORBA::Object_var implrepo_object =
        orb_->resolve_initial_references ("ImplRepoService");

      if (CORBA::is_nil (implrepo_object.in ()))
        {
          ACE_ERROR ((LM_ERROR,
                      "Unable to resolve the Implementation Repository.\n"));
		  ACE_OS::exit (-1);
        }

      exception_message = "While narrowing ImplRepo";
      this->implrepo_ =
        ImplementationRepository::Administration::_narrow (implrepo_object.in(), ACE_TRY_ENV);
      ACE_TRY_CHECK;

      // Now register the Ping Object
      PortableServer::ObjectId_var server_object_id =
        PortableServer::string_to_ObjectId ("server_object");

      exception_message = "While activating server object";
      this->poa_->activate_object_with_id (server_object_id.in (),
                                           this->server_object_,
                                           ACE_TRY_ENV);
      ACE_TRY_CHECK;

      exception_message = "While creating reference to ping object";
      CORBA::Object_ptr svr_obj = 
        this->poa_->id_to_reference (server_object_id.in (),
                                     ACE_TRY_ENV);
      ACE_TRY_CHECK;
    
      this->server_object_ptr_ = 
        ImplementationRepository::ServerObject::_narrow (svr_obj,
                                                         ACE_TRY_ENV);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_ERROR ((LM_ERROR, "IR_Helper::IR_Helper - %s\n", exception_message));
      ACE_TRY_ENV.print_exception ("SYS_EX");
      return;
    }
  ACE_ENDTRY;
}

IR_Helper::~IR_Helper ()
{
  delete this->name_;

  // @@ Delete the two objects
}

int
IR_Helper::register_server (const char *comm_line,
                            const char * /*environment*/,
                            const char *working_dir,
                            CORBA_Environment &ACE_TRY_ENV)
{
  ACE_TRY
    {
      CORBA::Object_var implrepo_object =
        orb_->resolve_initial_references ("ImplRepoService");

      if (CORBA::is_nil (implrepo_object.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Unable to resolve the Implementation Repository.\n"),
                          -1);

      ImplementationRepository::Administration *ImplRepo =
        ImplementationRepository::Administration::_narrow (implrepo_object.in(), ACE_TRY_ENV);
      ACE_TRY_CHECK;

      if (CORBA::is_nil (implrepo_object.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           "IR_Helper::register_server - Could not narrow IR::Admin\n"),
                          -1);

      ImplementationRepository::StartupOptions startup_options;

      startup_options.command_line = CORBA::string_dup (comm_line);
      // @@ Implement Environment
      //      startup_options.environment = CORBA::string_dup (environment);
      startup_options.working_directory = CORBA::string_dup (working_dir);

      ImplRepo->reregister_server (this->name_, startup_options, ACE_TRY_ENV);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_TRY_ENV.print_exception ("IR_Helper::register_server");
      return -1;
    }
  ACE_ENDTRY;
  return 0;
}

void
IR_Helper::notify_startup (CORBA_Environment &ACE_TRY_ENV)
{
  // @@ Don't use the ORB_Core_instance() keep a pointer to the ORB
  //    and use the orb_core() accessor
  TAO_Acceptor_Registry* registry =
    TAO_ORB_Core_instance ()->acceptor_registry ();

  TAO_Acceptor *acceptor = 0;
  TAO_AcceptorSetItor end = registry->end ();
  for (TAO_AcceptorSetItor i = registry->begin (); i != end; ++i)
    {
      if ((*i)->tag () == TAO_IOP_TAG_INTERNET_IOP)
        {
          acceptor = (*i);
          break;
        }
    }
  if (acceptor == 0)
    ACE_THROW (CORBA::NO_IMPLEMENT());

  TAO_IIOP_Acceptor* iiop_acceptor =
    ACE_dynamic_cast (TAO_IIOP_Acceptor*,acceptor);

  // Get our host and port and convert it to something we can use.
  const ACE_INET_Addr& my_addr  = iiop_acceptor->address ();

  ImplementationRepository::Address my_ir_addr;
  my_ir_addr.port = my_addr.get_port_number ();
  my_ir_addr.host = CORBA::string_dup (my_addr.get_host_name ());

  ACE_TRY
    {
      delete this->ir_addr_;

      this->ir_addr_ = this->implrepo_->server_is_running (this->name_,
                                                           my_ir_addr,
                                                           ACE_dynamic_cast (ImplementationRepository::ServerObject *, this->server_object_ptr_),
                                                           ACE_TRY_ENV);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_RETHROW;
    }
  ACE_ENDTRY;
}


// Notify the IR that the server has been shut down.
void
IR_Helper::notify_shutdown (CORBA_Environment &ACE_TRY_ENV)
{
  ACE_TRY
    {
      this->implrepo_->server_is_shutting_down (this->name_, ACE_TRY_ENV);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_RETHROW;
    }
  ACE_ENDTRY;
}

void
IR_Helper::change_object (CORBA::Object_ptr obj, CORBA_Environment &ACE_TRY_ENV)
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
