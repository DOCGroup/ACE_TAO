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

// Destructor.
TAO_AV_Endpoint_Strategy::~TAO_AV_Endpoint_Strategy (void)
{
  if (CORBA::is_nil (this->stream_endpoint_a_) == 0)
    CORBA::release (this->stream_endpoint_a_);

  if (CORBA::is_nil (this->stream_endpoint_b_) == 0)
    CORBA::release (this->stream_endpoint_b_);

  if (CORBA::is_nil (this->stream_endpoint_b_) == 0)
    CORBA::release (this->vdev_);

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

// Destructor.
TAO_AV_Endpoint_Process_Strategy::~TAO_AV_Endpoint_Process_Strategy (void)
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
  ACE_OS::sprintf (sem_str,
                   "%s:%s:%ld",
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
  if (CORBA::is_nil (this->naming_context_.in ()) == 0)
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

// Destructor
TAO_AV_Endpoint_Process_Strategy_A::~TAO_AV_Endpoint_Process_Strategy_A (void)
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

// Destructor
TAO_AV_Endpoint_Process_Strategy_B::~TAO_AV_Endpoint_Process_Strategy_B (void)
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
