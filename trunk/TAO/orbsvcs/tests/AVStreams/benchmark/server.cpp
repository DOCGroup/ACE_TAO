// $Id$


#include "server.h"

// AV_Server_Sig_Handler routines

AV_Server_Sig_Handler::AV_Server_Sig_Handler (void)
{
}

int
AV_Server_Sig_Handler::register_handler (void)
{
  // Assign the Sig_Handler a dummy I/O descriptor.  Note that even
  // though we open this file "Write Only" we still need to use the
  // ACE_Event_Handler::NULL_MASK when registering this with the
  // ACE_Reactor (see below).
  this->handle_ = ACE_OS::open (ACE_DEV_NULL, O_WRONLY);
  ACE_ASSERT (this->handle_ != -1);
  //  ACE_DEBUG ((LM_DEBUG,"(%P|%t) sig_handler == %d\n",this->handle_));

  // Register signal handler object.  Note that NULL_MASK is used to
  // keep the ACE_Reactor from calling us back on the "/dev/null"
  // descriptor.

   if (TAO_ORB_Core_instance ()->reactor ()->register_handler
       (this, ACE_Event_Handler::NULL_MASK) == -1)
     ACE_ERROR_RETURN ((LM_ERROR, 
                        "%p\n", 
                        "register_handler"),
                       -1);

  // handles these signals.
   //   this->sig_set.fill_set ();
   this->sig_set.sig_add (SIGCHLD);  
   this->sig_set.sig_add (SIGBUS); 
   this->sig_set.sig_add (SIGINT); 
   this->sig_set.sig_add (SIGTERM);
   
  // Register the signal handler object to catch the signals.  if
  if (TAO_ORB_Core_instance ()->reactor ()->register_handler 
      (this->sig_set, this) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, 
                       "%p\n", 
                       "register_handler"),
                      -1);
  return 0;
}

// Called by the ACE_Reactor to extract the fd.
ACE_HANDLE
AV_Server_Sig_Handler::get_handle (void) const
{
  return this->handle_;
}

int 
AV_Server_Sig_Handler::handle_input (ACE_HANDLE)
{
  ACE_DEBUG ((LM_DEBUG, "(%t) handling asynchonrous input...\n"));
  return 0;
}

int 
AV_Server_Sig_Handler::shutdown (ACE_HANDLE, ACE_Reactor_Mask)
{
  ACE_DEBUG ((LM_DEBUG, "(%t) closing down Sig_Handler...\n"));
  return 0;
}

// This method handles all the signals that are being caught by this
// object.  In our simple example, we are simply catching SIGALRM,
// SIGINT, and SIGQUIT.  Anything else is logged and ignored.
//
// There are several advantages to using this approach.  First, 
// the behavior triggered by the signal is handled in the main event
// loop, rather than in the signal handler.  Second, the ACE_Reactor's 
// signal handling mechanism eliminates the need to use global signal 
// handler functions and data. 

int
AV_Server_Sig_Handler::handle_signal (int signum, siginfo_t *, ucontext_t *)
{
  ACE_DEBUG ((LM_DEBUG, "(%P|%t) received signal %S\n", signum));

  // switch (signum)
//     {
//     case SIGCHLD:
//       // Handle the death of child signal.
//       this->clear_child (SIGCHLD);
//       break;
//     case SIGBUS:
//       // Handle the Bus error signal
//     case SIGINT:
//       // Handle the interrupt signal
//     case SIGTERM:
//       // Handle the process termination signal.
//       this->int_handler (signum);
//       break;
//     default:
//       //      ACE_DEBUG ((LM_DEBUG, "(%P|%t) %S: not handled, returning to program\n", signum));
//       break;
//     }
  return 0;
}

AV_Server_Sig_Handler::~AV_Server_Sig_Handler (void)
{
  TAO_ORB_Core_instance ()->reactor ()->remove_handler (this->sig_set);
}


//------------------------------------------------------------
Server::Server (void)
  //  :process_strategy_ (&process_options_)
  :reactive_strategy_(&orb_manager_)
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
                  //                  TAO_MMDevice (&this->process_strategy_),
                  TAO_MMDevice (&this->reactive_strategy_),
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
//   result = this->signal_handler_.register_handler ();

//   if (result < 0)
//     ACE_ERROR_RETURN ((LM_ERROR,
//                        "(%P|%t) Error registering signal handler"),
//                       -1);
  return 0;
}

// Runs the mpeg server
int
Server::run (CORBA::Environment& env)
{
  int result;
  // Run the ORB event loop
  while (1)
    {
      this->orb_manager_.run (env);
      if (errno== EINTR)
        continue;
      else
        break;
    }
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
