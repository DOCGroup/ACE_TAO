// $Id$

// ============================================================================
//
// = LIBRARY
//    cos
//
// = FILENAME
//   Endpoint_Strategy.cpp
//
// = AUTHOR
//    Sumedh Mungee <sumedh@cs.wustl.edu>
//    
//
// ============================================================================

#include "orbsvcs/AV/Endpoint_Strategy.h"

ACE_RCSID(AV, Endpoint_Strategy, "$Id$")

// ----------------------------------------------------------------------
// TAO_AV_Endpoint_Strategy
// ----------------------------------------------------------------------

// Constructor
TAO_AV_Endpoint_Strategy::TAO_AV_Endpoint_Strategy (void)
{
}

// The base class defines the "failure" case, so that unless the
// subclasses override this, the call will fail. This is done so that
// subclasses need only define the calls that they want to support,
// and the remaining calls will fail automagically
int
TAO_AV_Endpoint_Strategy::create_A (AVStreams::StreamEndPoint_A_ptr &stream_endpoint,
                                    AVStreams::VDev_ptr &vdev,
                                    CORBA::Environment &env)
{
  ACE_ERROR_RETURN ((LM_ERROR,
                     "(%P|%t) Error creating A endpoint\n"),
                    -1);
}

// The base class defines the "failure" case, so that unless the
// subclasses override this, the call will fail. This is done so that
// subclasses need only define the calls that they want to support,
// and the remaining calls will fail automagically
int
TAO_AV_Endpoint_Strategy::create_B (AVStreams::StreamEndPoint_B_ptr &stream_endpoint,
                                    AVStreams::VDev_ptr &vdev,
                                    CORBA::Environment &env)
{
  ACE_ERROR_RETURN ((LM_ERROR,
                     "(%P|%t) Error creating B endpoint\n"),
                    -1);
}


// ----------------------------------------------------------------------
// TAO_AV_Endpoint_Process_Strategy
// ----------------------------------------------------------------------

// Constructor
TAO_AV_Endpoint_Process_Strategy::TAO_AV_Endpoint_Process_Strategy (ACE_Process_Options *process_options)
  : process_options_ (process_options)
{
}

// Spawns the process, and waits for it to finish booting.
// Then uses bind_to_naming_service, get_stream_endpoint, and get_vdev
// to get the object references to the various objects created in the
// child 
int
TAO_AV_Endpoint_Process_Strategy::activate (void)
{
  ACE_Process process;

  // Create a new process to contain this endpoint
  pid_t pid = process.spawn (*this->process_options_);

  // Process creation failed
  if (pid == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "(%P|%t) ACE_Process:: spawn failed: %p\n",
                       "spawn"),
                      -1);

  // Create a unique semaphore name, using my hostname, and pid.
  char sem_str [BUFSIZ];
  char host [MAXHOSTNAMELEN];
  ACE_OS::hostname (host,
                    sizeof host);
  
  // create a unique semaphore name
  ::sprintf (sem_str,
             "%s:%s:%d",
             "TAO_AV_Process_Semaphore",
             host,
             pid);
  
  ACE_DEBUG ((LM_DEBUG,
              "(%P|%t) semaphore is %s\n",
              sem_str));
  // Create the semaphore
  ACE_Process_Semaphore semaphore (0, // 0 means that the
                                   // semaphore is locked initially
                                   sem_str);
  
  // wait until the child finishes booting
  while (1)
    {
      if (semaphore.acquire () == -1)
        {
          // See if my child process is still alive -- if not, return an error
          if (ACE_OS::kill (pid,
                            0) == -1)
            ACE_ERROR_RETURN ((LM_ERROR,
                               "(%P|%t) Process_Strategy: Process being waited on died unexpectedly.\n"),
                              -1);
          // if we were not interrupted due to a EINTR, break
          if (errno != EINTR)
            break;
        }
      else
        break;
    }

  // The job of the semaphore is done, remove it.
  if (semaphore.remove () == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "(%P|%t) semaphore remove failed: %p\n",
                       "remove"),
                      -1);
  
  TAO_TRY 
    {
      // Get ourselves a Naming service
      this->bind_to_naming_service (TAO_TRY_ENV);
      TAO_CHECK_ENV;

      // Get the stream endpoint created by the child from the naming service
      this->get_stream_endpoint (TAO_TRY_ENV);
      TAO_CHECK_ENV;

      // Get the Vdev created by the child from the naming service
      this->get_vdev (TAO_TRY_ENV);
      TAO_CHECK_ENV;
    }
  TAO_CATCHANY
    {
      TAO_TRY_ENV.print_exception ("TAO_Endpoint_Process_Strategy::activate");
      return -1;
    }      
  TAO_ENDTRY;

  return 0;
}

// Get ourselves a Naming service reference
int
TAO_AV_Endpoint_Process_Strategy::bind_to_naming_service (CORBA::Environment &env)
{
  if (CORBA::is_nil (this->naming_context_.in ()) == CORBA::B_FALSE)
    return 0;

  CORBA::Object_var naming_obj =
    TAO_ORB_Core_instance ()->orb ()->resolve_initial_references ("NameService");

  if (CORBA::is_nil (naming_obj.in ()))
    ACE_ERROR_RETURN ((LM_ERROR,
                       " (%P|%t) Unable to resolve the Name Service.\n"),
                      -1);
  this->naming_context_ =
    CosNaming::NamingContext::_narrow (naming_obj.in (),
                                       env);
  TAO_CHECK_ENV_RETURN (env, -1);

  return 0;
}

// Get the VDev created in the child process from the namingservice
int
TAO_AV_Endpoint_Process_Strategy::get_vdev (CORBA::Environment &env)
{
  // Create the name
  CosNaming::Name VDev_Name (1);
  VDev_Name.length (1);
  VDev_Name [0].id = CORBA::string_dup
    ("VDev");
  
  // Get the CORBA::Object
  CORBA::Object_var vdev =
    this->naming_context_->resolve (VDev_Name,
                             env);
  TAO_CHECK_ENV_RETURN (env, -1);

  // Narrow it
  this->vdev_ =
    AVStreams::VDev::_narrow (vdev.in (),
                              env);
  TAO_CHECK_ENV_RETURN (env, -1);

  // Check if valid
  if (CORBA::is_nil (this->vdev_))
    ACE_ERROR_RETURN ((LM_ERROR,
                       " could not resolve Stream_Endpoint_B in Naming service <%s>\n"),
                      -1);
  return 0;
}

// ----------------------------------------------------------------------
// TAO_AV_Endpoint_Process_Strategy_A
// ----------------------------------------------------------------------

// Constructor
TAO_AV_Endpoint_Process_Strategy_A::TAO_AV_Endpoint_Process_Strategy_A (ACE_Process_Options *process_options)
  : TAO_AV_Endpoint_Process_Strategy (process_options)
{
}

// the "A" type endpoint creator
int
TAO_AV_Endpoint_Process_Strategy_A::create_A (AVStreams::StreamEndPoint_A_ptr &stream_endpoint,
                                           AVStreams::VDev_ptr &vdev,
                                           CORBA::Environment &env)
{
  // use the baseclass activate
  if (this->activate () == -1)
    ACE_ERROR_RETURN ((LM_ERROR, 
                       "(%P|%t) TAO_AV_Endpoint_Process_Strategy: Error in activate ()\n"),
                      -1);
  
  // return the object references
  stream_endpoint = this->stream_endpoint_a_;
  vdev = this->vdev_;
  return 0;
  
}

// Gets the stream endpoint object reference from the naming service
int
TAO_AV_Endpoint_Process_Strategy_A::get_stream_endpoint (CORBA::Environment &env)
{
  // Create the name
  CosNaming::Name Stream_Endpoint_A_Name (1);
  
  Stream_Endpoint_A_Name.length (1);
  Stream_Endpoint_A_Name [0].id = CORBA::string_dup
    ("Stream_Endpoint_A");

  // Get the CORBA::Object
  CORBA::Object_var stream_endpoint_a =
    this->naming_context_->resolve (Stream_Endpoint_A_Name,
                                    env);
  TAO_CHECK_ENV_RETURN (env, -1);
  
  // Narrow the reference
  this->stream_endpoint_a_ =
    AVStreams::StreamEndPoint_A::_narrow (stream_endpoint_a.in (),
                                          env);
  TAO_CHECK_ENV_RETURN (env, -1);
  
  // Check for validity
  if (CORBA::is_nil (this->stream_endpoint_a_))
    ACE_ERROR_RETURN ((LM_ERROR,
                       " could not resolve Stream_Endpoint_A in Naming service <%s>\n"),
                      -1);
  return 0;
}

// ----------------------------------------------------------------------
// TAO_AV_Endpoint_Process_Strategy_B
// ----------------------------------------------------------------------

// Constructor
TAO_AV_Endpoint_Process_Strategy_B::TAO_AV_Endpoint_Process_Strategy_B (ACE_Process_Options *process_options)
  : TAO_AV_Endpoint_Process_Strategy (process_options)
{
}

// Creates and returns a "B" type endpoint
int
TAO_AV_Endpoint_Process_Strategy_B::create_B (AVStreams::StreamEndPoint_B_ptr &stream_endpoint,
                                           AVStreams::VDev_ptr &vdev,
                                           CORBA::Environment &env)
{
  if (this->activate () == -1)
    ACE_ERROR_RETURN ((LM_ERROR, 
                       "(%P|%t) TAO_AV_Endpoint_Process_Strategy: Error in activate ()\n"),
                      -1);

  stream_endpoint = this->stream_endpoint_b_;
  vdev = this->vdev_;
  return 0;
  
}

// Gets the B type stream_endpoint from the Naming service
int
TAO_AV_Endpoint_Process_Strategy_B::get_stream_endpoint (CORBA::Environment &env)
{
  // Create the name
  CosNaming::Name Stream_Endpoint_B_Name (1);
  
  Stream_Endpoint_B_Name.length (1);
  Stream_Endpoint_B_Name [0].id = CORBA::string_dup
    ("Stream_Endpoint_B");
  
  // Get the CORBA::Object reference
  CORBA::Object_var stream_endpoint_b =
    this->naming_context_->resolve (Stream_Endpoint_B_Name,
                                    env);
  TAO_CHECK_ENV_RETURN (env, -1);

  // Narrow the reference
  this->stream_endpoint_b_ =
    AVStreams::StreamEndPoint_B::_narrow (stream_endpoint_b.in (),
                                          env);
  TAO_CHECK_ENV_RETURN (env, -1);

  // Check for validity
  if (CORBA::is_nil (this->stream_endpoint_b_))
    ACE_ERROR_RETURN ((LM_ERROR,
                       " could not resolve Stream_Endpoint_B in Naming service <%s>\n"),
                      -1);
  return 0;
}
// ----------------------------------------------------------------------
// TAO_AV_Endpoint_Reactive_Strategy
// ----------------------------------------------------------------------

// Constructor
template <class T_StreamEndpoint, class T_VDev , class T_MediaCtrl>
TAO_AV_Endpoint_Reactive_Strategy <T_StreamEndpoint, T_VDev, T_MediaCtrl>::TAO_AV_Endpoint_Reactive_Strategy (TAO_ORB_Manager *orb_manager)
  : orb_manager_ (orb_manager)
{
}

// Create, activate the objects with the POA
template <class T_StreamEndpoint, class T_VDev , class T_MediaCtrl>
int
TAO_AV_Endpoint_Reactive_Strategy <T_StreamEndpoint, T_VDev, T_MediaCtrl>::activate (void)
{
  TAO_TRY
    {
      this->activate_stream_endpoint (TAO_TRY_ENV);
      TAO_CHECK_ENV;
      ACE_DEBUG ((LM_DEBUG,"TAO_AV_Endpoint_Reactive_Strategy::activated stream_endpoint\n"));
      
      this->activate_vdev (TAO_TRY_ENV);
      TAO_CHECK_ENV;
      ACE_DEBUG ((LM_DEBUG,"TAO_AV_Endpoint_Reactive_Strategy::activated vdev\n"));      

      this->activate_mediactrl (TAO_TRY_ENV);
      TAO_CHECK_ENV;
      ACE_DEBUG ((LM_DEBUG,"TAO_AV_Endpoint_Reactive_Strategy::activated mediactrl\n"));
    }
  TAO_CATCHANY
    {
      TAO_TRY_ENV.print_exception ("TAO_Endpoint_Reactive_Strategy::activate");
      return -1;
    }
  TAO_ENDTRY;
  return 0;
}

// Activate VDev into the POA
template <class T_StreamEndpoint, class T_VDev , class T_MediaCtrl>
int
TAO_AV_Endpoint_Reactive_Strategy <T_StreamEndpoint, T_VDev, T_MediaCtrl>::activate_vdev (CORBA::Environment &env)
{
  // Bridge pattern. Allow subclasses to override this behavior
  T_VDev *vdev = 0;
  if (this->make_vdev (vdev) == -1)
    return -1;

  // Put the object in the POA
  this->orb_manager_->activate (vdev,
                                env);
  TAO_CHECK_ENV_RETURN (env, -1);

  // Save the object reference, so that create_A can return it
  this->vdev_ = vdev->_this (env);
  TAO_CHECK_ENV_RETURN (env, -1);


  return 0;
}

// Activate the media_controller
template <class T_StreamEndpoint, class T_VDev , class T_MediaCtrl>
int
TAO_AV_Endpoint_Reactive_Strategy <T_StreamEndpoint, T_VDev, T_MediaCtrl>::activate_mediactrl (CORBA::Environment &env)
{
  // Bridge pattern. Subclasses can override this
  T_MediaCtrl *media_ctrl = 0;
  if (this->make_mediactrl (media_ctrl) == -1)
    return -1;

  // Put the object in the POA
  this->orb_manager_->activate (media_ctrl,
                                env);
  TAO_CHECK_ENV_RETURN (env, -1);
  
  // Associate the media controller object reference with the vdev, as per the OMG spec
  CORBA::Any anyval;
  anyval <<= this->orb_manager_->orb ()->object_to_string (media_ctrl->_this (env),
                                                           env);
  TAO_CHECK_ENV_RETURN (env, -1);
  
  this->vdev_->define_property ("Related_MediaCtrl",
                                anyval,
                                env);

  TAO_CHECK_ENV_RETURN (env, -1);

  return 0;
}

// Bridge method
template <class T_StreamEndpoint, class T_VDev , class T_MediaCtrl>
int
TAO_AV_Endpoint_Reactive_Strategy<T_StreamEndpoint, T_VDev, T_MediaCtrl>::make_stream_endpoint (T_StreamEndpoint *&stream_endpoint)
{
  ACE_NEW_RETURN (stream_endpoint, 
                  T_StreamEndpoint,
                  -1);
  return 0;
}

// Bridge method
template <class T_StreamEndpoint, class T_VDev , class T_MediaCtrl>
int
TAO_AV_Endpoint_Reactive_Strategy<T_StreamEndpoint, T_VDev, T_MediaCtrl>::make_vdev (T_VDev *&vdev)
{
  ACE_NEW_RETURN (vdev, 
                  T_VDev,
                  -1);
  return 0;
}

// Bridge method
template <class T_StreamEndpoint, class T_VDev , class T_MediaCtrl>
int
TAO_AV_Endpoint_Reactive_Strategy<T_StreamEndpoint, T_VDev, T_MediaCtrl>::make_mediactrl (T_MediaCtrl *&media_ctrl)
{
  ACE_NEW_RETURN (media_ctrl, 
                  T_MediaCtrl,
                  -1);
  return 0;
}


// ----------------------------------------------------------------------

// ----------------------------------------------------------------------
// TAO_AV_Endpoint_Reactive_Strategy_A
// ----------------------------------------------------------------------
// Constructor
template <class T_StreamEndpoint, class T_VDev , class T_MediaCtrl>
TAO_AV_Endpoint_Reactive_Strategy_A <T_StreamEndpoint, T_VDev, T_MediaCtrl>::TAO_AV_Endpoint_Reactive_Strategy_A (TAO_ORB_Manager *orb_manager)
  : TAO_AV_Endpoint_Reactive_Strategy <T_StreamEndpoint, T_VDev, T_MediaCtrl> (orb_manager)
{
}

// Creates an "A" type streamendpoint, and a vdev and returns the
// object references
template <class T_StreamEndpoint, class T_VDev , class T_MediaCtrl>
int
TAO_AV_Endpoint_Reactive_Strategy_A<T_StreamEndpoint, T_VDev, T_MediaCtrl>::create_A (AVStreams::StreamEndPoint_A_ptr &stream_endpoint,
                                                                                      AVStreams::VDev_ptr &vdev,
                                                                                      CORBA::Environment &env)
{
  if (this->activate () == -1)
    ACE_ERROR_RETURN ((LM_ERROR, 
                       "(%P|%t) TAO_AV_Endpoint_Reactive_Strategy_A: Error in activate ()\n"),
                      -1);
  
  stream_endpoint = this->stream_endpoint_a_;
  vdev = this->vdev_;
  return 0;
  
}

// Put the stream_endpoint into the POA
template <class T_StreamEndpoint, class T_VDev , class T_MediaCtrl>
int
TAO_AV_Endpoint_Reactive_Strategy_A <T_StreamEndpoint, T_VDev, T_MediaCtrl>::activate_stream_endpoint (CORBA::Environment &env)
{
  T_StreamEndpoint *stream_endpoint_a = 0;

  // Use the bridge method
  if (this->make_stream_endpoint (stream_endpoint_a) == -1)
    return -1;
  
  this->orb_manager_->activate (stream_endpoint_a,
                                env);
  TAO_CHECK_ENV_RETURN (env, -1);

  // Save the object references, so that create_a can return them
  this->stream_endpoint_a_ = stream_endpoint_a->_this (env);
  TAO_CHECK_ENV_RETURN (env, -1);

  return 0;
}

// ----------------------------------------------------------------------
// TAO_AV_Endpoint_Reactive_Strategy_B
// ----------------------------------------------------------------------

// Constructor
template <class T_StreamEndpoint, class T_VDev , class T_MediaCtrl>
TAO_AV_Endpoint_Reactive_Strategy_B <T_StreamEndpoint, T_VDev, T_MediaCtrl>::TAO_AV_Endpoint_Reactive_Strategy_B (TAO_ORB_Manager *orb_manager)
  : TAO_AV_Endpoint_Reactive_Strategy <T_StreamEndpoint, T_VDev, T_MediaCtrl> (orb_manager)
{
}

// Activate stream_endpoint
template <class T_StreamEndpoint, class T_VDev , class T_MediaCtrl>
int
TAO_AV_Endpoint_Reactive_Strategy_B <T_StreamEndpoint, T_VDev, T_MediaCtrl>::activate_stream_endpoint (CORBA::Environment &env)
{
  T_StreamEndpoint *stream_endpoint_b = 0;

  if (this->make_stream_endpoint (stream_endpoint_b) == -1)
    return -1;
  this->orb_manager_->activate (stream_endpoint_b,
                                 env);
  TAO_CHECK_ENV_RETURN (env, -1);

  this->stream_endpoint_b_ = stream_endpoint_b->_this (env);
  TAO_CHECK_ENV_RETURN (env, -1);

  return 0;
}

// Returns a "B" type stream_endpoint and a vdev
template <class T_StreamEndpoint, class T_VDev , class T_MediaCtrl>
int
TAO_AV_Endpoint_Reactive_Strategy_B<T_StreamEndpoint, T_VDev, T_MediaCtrl>::create_B (AVStreams::StreamEndPoint_B_ptr &stream_endpoint,
                                                                                      AVStreams::VDev_ptr &vdev,
                                                                                      CORBA::Environment &env)
{
  if (this->activate () == -1)
    ACE_ERROR_RETURN ((LM_ERROR, 
                       "(%P|%t) TAO_AV_Endpoint_Reactive_Strategy_B: Error in activate ()\n"),
                      -1);
  
  stream_endpoint = this->stream_endpoint_b_;
  vdev = this->vdev_;
  return 0;
  
}

// ----------------------------------------------------------------------
// TAO_AV_Child_Process
// ----------------------------------------------------------------------

// Constructor
template <class T_StreamEndpoint_B, class T_VDev , class T_MediaCtrl>
TAO_AV_Child_Process <T_StreamEndpoint_B, T_VDev, T_MediaCtrl>::TAO_AV_Child_Process ()
  : stream_endpoint_name_ (0)
{
}

// Initializes the ORB, activates the objects, and release the semaphore
template <class T_StreamEndpoint_B, class T_VDev , class T_MediaCtrl>
int
TAO_AV_Child_Process  <T_StreamEndpoint_B, T_VDev, T_MediaCtrl>::init (int argc, char **argv)
{
  TAO_TRY
    {
      // create the objects and activate them in the poa
      this->activate_objects (argc,
                              argv,
                              TAO_TRY_ENV);
      TAO_CHECK_ENV;

      // Get ourselves a naming_service object reference
      this->bind_to_naming_service (TAO_TRY_ENV);
      TAO_CHECK_ENV;

      // Register the vdev with the naming service
      this->register_vdev (TAO_TRY_ENV);
      TAO_CHECK_ENV;

      // register the stream_endpoing with the naming_service
      this->register_stream_endpoint (TAO_TRY_ENV);
      TAO_CHECK_ENV;
    }
  TAO_CATCHANY
    {
      TAO_TRY_ENV.print_exception ("TAO_Child_Process");
      return -1;
    }
  TAO_ENDTRY;

  // release the semaphore the parent is waiting on
  if (this->release_semaphore () == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "(%P|%t) Error releasing semaphores\n"),
                      -1);
  
  return 0;
}

// initializes the orb, and activates the objects
template <class T_StreamEndpoint, class T_VDev , class T_MediaCtrl>
int
TAO_AV_Child_Process  <T_StreamEndpoint, T_VDev, T_MediaCtrl>::activate_objects (int argc, 
                                                                                char **argv,
                                                                                CORBA::Environment &env)
{
  // initialize the orb
  this->orb_manager_.init (argc,
                           argv,
                           env);
  TAO_CHECK_ENV_RETURN (env, -1);

  // bridge method to make a new stream endpoint
  if (this->make_stream_endpoint (this->stream_endpoint_) == -1)
    return -1;
  
  // bridge method to make a new vdev
  if (this->make_vdev (this->vdev_) == -1)
    return -1;

  // bridge method to make a new media controller
  if (this->make_mediactrl (this->media_ctrl_) == -1)
    return -1;

  // activate the stream_endpoint
  this->orb_manager_.activate (this->stream_endpoint_,
                               env);
  TAO_CHECK_ENV_RETURN (env, -1);


  // activate the vdev
  this->orb_manager_.activate (this->vdev_,
                               env);
  TAO_CHECK_ENV_RETURN (env, -1);

  // activate the media controller
  this->orb_manager_.activate (this->media_ctrl_,
                               env);

  TAO_CHECK_ENV_RETURN (env, -1);

  return 0;

}

// Bind to the namingservice
template <class T_StreamEndpoint, class T_VDev , class T_MediaCtrl>
int
TAO_AV_Child_Process  <T_StreamEndpoint, T_VDev, T_MediaCtrl>::bind_to_naming_service (CORBA::Environment &env)
{
  CORBA::Object_var naming_obj =
    this->orb_manager_.orb ()->resolve_initial_references ("NameService");
  if (CORBA::is_nil (naming_obj.in ()))
    ACE_ERROR_RETURN ((LM_ERROR,
                       " (%P|%t) Unable to resolve the Name Service.\n"),
                      -1);
  //  ACE_DEBUG ((LM_DEBUG, "(%P|%t) %s:%d\n", __FILE__, __LINE__));
  this->naming_context_ =
    CosNaming::NamingContext::_narrow (naming_obj.in (),
                                       env);
  TAO_CHECK_ENV_RETURN (env,
                        -1);

  return 0;
}

// register the vdev with the naming service
template <class T_StreamEndpoint, class T_VDev , class T_MediaCtrl>
int
TAO_AV_Child_Process  <T_StreamEndpoint, T_VDev, T_MediaCtrl>::register_vdev (CORBA::Environment &env)
{  
  // create the name
  this->vdev_name_.length (1);
  this->vdev_name_ [0].id = CORBA::string_dup ("VDev");

  // make the media controller a property of the vdev
  CORBA::Any media_ctrl_property;
  media_ctrl_property <<= this->orb_manager_.orb ()->object_to_string (this->media_ctrl_->_this (env),
                                                                       env);
  this->vdev_->define_property ("Related_MediaCtrl",
                                media_ctrl_property,
                                env);
  TAO_CHECK_ENV_RETURN (env,-1);
  
  // Register the vdev with the naming server.
  this->naming_context_->bind (this->vdev_name_,
                               this->vdev_->_this (env),
                               env);
  
  // If the object was already there, replace the older reference
  // with this one
  if (env.exception () != 0)
    {
      env.clear ();
      this->naming_context_->rebind (this->vdev_name_,
                                     this->vdev_->_this (env),
                                     env);
      TAO_CHECK_ENV_RETURN (env, -1);
    }
  
  return 0;
}

// run the orb event look
template <class T_StreamEndpoint_B, class T_VDev , class T_MediaCtrl>
int
TAO_AV_Child_Process  <T_StreamEndpoint_B, T_VDev, T_MediaCtrl>::run (ACE_Time_Value *tv)
{
  int result;
  TAO_TRY
    {
      result = this->orb_manager_.run (TAO_TRY_ENV,tv);
      TAO_CHECK_ENV;
    }
  TAO_CATCHANY
    {
      TAO_TRY_ENV.print_exception ("orb_manager_.run ()");
      return -1;
    }
  TAO_ENDTRY;
  return result;
}

// release the semaphore the parent is waiting on
template <class T_StreamEndpoint_B, class T_VDev , class T_MediaCtrl>
int
TAO_AV_Child_Process  <T_StreamEndpoint_B, T_VDev, T_MediaCtrl>::release_semaphore ()
{
  char sem_str [BUFSIZ];
  char host [MAXHOSTNAMELEN];
  pid_t pid;

  // make the semaphore name
  if ((pid = ACE_OS::getpid ()) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "(%P|%t) Unable to get my pid!\n"),
                      -1);
  
  ACE_OS::hostname (host,
                    sizeof host);
  
  sprintf (sem_str,
           "%s:%s:%d",
           "TAO_AV_Process_Semaphore",
           host,
           pid);

  ACE_DEBUG ((LM_DEBUG,
              "(%P|%t) semaphore is %s\n",
              sem_str));

  // Release the lock on which the server is waiting
  ACE_Process_Semaphore semaphore (0, // 0 means that the semaphore is
                                   // initially locked
                                   sem_str);
  
  if (semaphore.release () == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "(%P|%t) Error releasing semaphore %s: %p\n",
                       sem_str,
                       "semaphore.release ()"),
                      -1);
  
  return 0;
}

// register the stream_endpoint with the naming service
template <class T_StreamEndpoint, class T_VDev , class T_MediaCtrl>
int
TAO_AV_Child_Process  <T_StreamEndpoint, T_VDev, T_MediaCtrl>::register_stream_endpoint (CORBA::Environment &env)
{  
  //  ACE_DEBUG ((LM_DEBUG, "(%P|%t) %s:%d\n", __FILE__, __LINE__));
  // Create a name for the video control object
  // subclasses can define their own name for the streamendpoint
  // Register the stream endpoint object with the naming server.
  this->naming_context_->bind (this->stream_endpoint_name_,
                               this->stream_endpoint_->_this (env),
                               env);
  
  // if the name was already there, replace the reference with the new one
  if (env.exception () != 0)
    {
      env.clear ();
      this->naming_context_->rebind (this->stream_endpoint_name_,
                                     this->stream_endpoint_->_this (env),
                                     env);
      TAO_CHECK_ENV_RETURN (env, -1);
    }

  return 0;
}

// Bridge method to make a new stream_endpoint
template <class T_StreamEndpoint, class T_VDev , class T_MediaCtrl>
int
TAO_AV_Child_Process<T_StreamEndpoint, T_VDev, T_MediaCtrl>::make_stream_endpoint (T_StreamEndpoint *&stream_endpoint)
{
  ACE_NEW_RETURN (stream_endpoint, 
                  T_StreamEndpoint,
                  -1);
  return 0;
}

// Bridge method to make a new vdev
template <class T_StreamEndpoint, class T_VDev , class T_MediaCtrl>
int
TAO_AV_Child_Process<T_StreamEndpoint, T_VDev, T_MediaCtrl>::make_vdev (T_VDev *&vdev)
{
  ACE_NEW_RETURN (vdev, 
                  T_VDev,
                  -1);
  return 0;
}

// Bridge method to make a new media controller
template <class T_StreamEndpoint, class T_VDev , class T_MediaCtrl>
int
TAO_AV_Child_Process<T_StreamEndpoint, T_VDev, T_MediaCtrl>::make_mediactrl (T_MediaCtrl *&media_ctrl)
{
  ACE_DEBUG ((LM_DEBUG,"(%P|%t) TAO_AV_Child_Process::make_mediactrl ()\n"));
  ACE_NEW_RETURN (media_ctrl, 
                  T_MediaCtrl,
                  -1);
  return 0;
}

// %% its not clear whether we should be deleting the objects, since
// if the application overrides the make_mediactrl methods etc.,
// then, we may not own these objects.
// For now, we dont delete the objects, since they exist for the
// lifetime of the process anyway
template <class T_StreamEndpoint, class T_VDev , class T_MediaCtrl>
TAO_AV_Child_Process  <T_StreamEndpoint, T_VDev, T_MediaCtrl>::~TAO_AV_Child_Process ()
{
  // Remove the names from the naming service
  if (this->naming_context_ == 0)
    return;
  TAO_TRY
    {
      this->naming_context_->unbind (this->stream_endpoint_name_,
                                     TAO_TRY_ENV);
      TAO_CHECK_ENV;

      this->naming_context_->unbind (this->vdev_name_,
                                     TAO_TRY_ENV);
      TAO_CHECK_ENV;
    }
  TAO_CATCHANY
    {
      TAO_TRY_ENV.print_exception ("TAO_Endpoint_Process_Strategy::activate");
    }      
  TAO_ENDTRY;
    
//  if (this->stream_endpoint_ != 0)
//    delete this->stream_endpoint_;
//  if (this->vdev_ != 0)      
//    delete this->vdev_;      
//  if (this->media_ctrl_ != 0)
//    delete this->media_ctrl_;
}

// ----------------------------------------------------------------------
// TAO_AV_Child_Process_A
// ----------------------------------------------------------------------

// Define the name of the stream_endpoint, as used to register with
// the  naming service
template <class T_StreamEndpoint, class T_VDev , class T_MediaCtrl>
TAO_AV_Child_Process_A  <T_StreamEndpoint, T_VDev, T_MediaCtrl>::TAO_AV_Child_Process_A ()
{  
  this->stream_endpoint_name_.length (1);
  this->stream_endpoint_name_ [0].id = CORBA::string_dup ("Stream_Endpoint_A");
}

// ----------------------------------------------------------------------
// TAO_AV_Child_Process_B
// ----------------------------------------------------------------------

// Define the name of the stream_endpoint, as used to register with
// the  naming service
template <class T_StreamEndpoint, class T_VDev , class T_MediaCtrl>
TAO_AV_Child_Process_B  <T_StreamEndpoint, T_VDev, T_MediaCtrl>::TAO_AV_Child_Process_B ()
{  
  this->stream_endpoint_name_.length (1);
  this->stream_endpoint_name_ [0].id = CORBA::string_dup ("Stream_Endpoint_B");
}
