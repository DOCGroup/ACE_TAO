// $Id$


#include "server.h"

Server::Server (void)
  :process_strategy_ (&process_options_)
{
  this->process_options_.command_line ("./child -ORBport 0 -ORBobjrefstyle url");
}

        
// Initializes the mpeg server
int
Server::init (int argc,
              char **argv,
              CORBA::Environment& env)
{
  int result;
  
  // Initialize the orb_manager
  this->orb_manager_.init_child_poa (argc,
                                     argv,
                                     "child_poa",
                                     env);
  TAO_CHECK_ENV_RETURN (env,
                        -1);
  
  CORBA::ORB_var orb = 
    this->orb_manager_.orb ();

  PortableServer::POA_var child_poa = 
    this->orb_manager_.child_poa ();

  // Resolve the Naming service reference. 

  CORBA::Object_var naming_obj = orb->resolve_initial_references ("NameService");
  if (CORBA::is_nil (naming_obj.in ()))
    ACE_ERROR_RETURN ((LM_ERROR,
                       " (%P|%t) Unable to resolve the Name Service.\n"),
                      -1);

  this->naming_context_ =
    CosNaming::NamingContext::_narrow (naming_obj.in (),
                                       env);
  TAO_CHECK_ENV_RETURN (env,-1);

  // Register the video mmdevice object with the ORB
  ACE_NEW_RETURN (this->mmdevice_,
                  TAO_MMDevice (&this->process_strategy_),
                  -1);

  // create the video server mmdevice with the naming service pointer.
  this->orb_manager_.activate_under_child_poa ("Bench_Server_MMDevice",
                                               this->mmdevice_,
                                               env);
  TAO_CHECK_ENV_RETURN (env,-1);

  // Register the mmdevice with the naming service.
  CosNaming::Name server_mmdevice_name (1);
  server_mmdevice_name.length (1);
  server_mmdevice_name [0].id = CORBA::string_dup ("Bench_Server_MMDevice");
  
  // Register the video control object with the naming server.
  this->naming_context_->bind (server_mmdevice_name,
                               this->mmdevice_->_this (env),
                               env);

  if (env.exception () != 0)
    {
      env.clear ();
      this->naming_context_->rebind (server_mmdevice_name,
                              this->mmdevice_->_this (env),
                              env);
      TAO_CHECK_ENV_RETURN (env,-1);
    }
  return 0;
}

// Runs the mpeg server
int
Server::run (CORBA::Environment& env)
{
  int result;
  // Run the ORB event loop
  this->orb_manager_.run (env);

  ACE_DEBUG ((LM_DEBUG,
              "(%P|%t) Server::run () "
              "came out of the (ORB) "
              "event loop %p\n",
              "run_event_loop"));
  return 0;
  
}

Server::~Server (void)
{
  ACE_DEBUG ((LM_DEBUG,
              "(%P|%t) Server: Removing handlers from the Reactor\n"));

  if (this->mmdevice_ != 0)
    delete this->mmdevice_;
  
}


int
main (int argc, char **argv)
{
  Server server;
  TAO_TRY
    {
      if (server.init (argc, argv, TAO_TRY_ENV) == -1)
        return 1;
      TAO_CHECK_ENV;

      server.run (TAO_TRY_ENV);
      TAO_CHECK_ENV;
    }
  TAO_CATCHANY
    {
      TAO_TRY_ENV.print_exception ("Bench_Server::Exception");
      return -1;
    }
  TAO_ENDTRY;
  
  return 0;
}
