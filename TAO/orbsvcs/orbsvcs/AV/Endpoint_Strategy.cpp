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

// ----------------------------------------------------------------------
// TAO_AV_Endpoint_Strategy
// ----------------------------------------------------------------------

TAO_AV_Endpoint_Strategy::TAO_AV_Endpoint_Strategy (void)
{
}

int
TAO_AV_Endpoint_Strategy::create_A (AVStreams::StreamEndPoint_A_ptr &stream_endpoint,
                                 AVStreams::VDev_ptr &vdev,
                                 CORBA::Environment &env)
{
  ACE_ERROR_RETURN ((LM_ERROR,
                     "(%P|%t) Error creating A endpoint\n"),
                    -1);
}

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

TAO_AV_Endpoint_Process_Strategy::TAO_AV_Endpoint_Process_Strategy (ACE_Process_Options *process_options)
  : process_options_ (process_options)
{
}

int
TAO_AV_Endpoint_Process_Strategy::activate (void)
{
  ACE_Process process;
  
  pid_t pid = process.spawn (*this->process_options_);
  if (pid == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "(%P|%t) ACE_Process:: spawn failed: %p\n",
                       "spawn"),
                      -1);

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
  
  // %% wait until the child finishes booting
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
  
  if (semaphore.remove () == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "(%P|%t) semaphore remove failed: %p\n",
                       "remove"),
                      -1);
  
  TAO_TRY 
    {
      this->bind_to_naming_service (TAO_TRY_ENV);
      TAO_CHECK_ENV;

      this->get_stream_endpoint (TAO_TRY_ENV);
      TAO_CHECK_ENV;

      this->get_vdev (TAO_TRY_ENV);
      TAO_CHECK_ENV;
    }
  TAO_CATCHANY
    {
      TAO_TRY_ENV.print_exception ("TAO_Endpoint_Process_Strategy::activate");
      return -1;
    }      
  TAO_ENDTRY;
}

int
TAO_AV_Endpoint_Process_Strategy::bind_to_naming_service (CORBA::Environment &env)
{
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

int
TAO_AV_Endpoint_Process_Strategy::get_vdev (CORBA::Environment &env)
{
  CosNaming::Name VDev_Name (1);
  VDev_Name.length (1);
  VDev_Name [0].id = CORBA::string_dup
    ("VDev");
  
  CORBA::Object_var vdev =
    this->naming_context_->resolve (VDev_Name,
                             env);
  TAO_CHECK_ENV_RETURN (env, -1);
  
  this->vdev_ =
    AVStreams::VDev::_narrow (vdev.in (),
                              env);
  TAO_CHECK_ENV_RETURN (env, -1);
  
      if (CORBA::is_nil (this->vdev_))
        ACE_ERROR_RETURN ((LM_ERROR,
                           " could not resolve Stream_Endpoint_B in Naming service <%s>\n"),
                          -1);
  return 0;
}

// ----------------------------------------------------------------------
// TAO_AV_Endpoint_Process_Strategy_A
// ----------------------------------------------------------------------

TAO_AV_Endpoint_Process_Strategy_A::TAO_AV_Endpoint_Process_Strategy_A (ACE_Process_Options *process_options)
  : TAO_AV_Endpoint_Process_Strategy (process_options)
{
}

int
TAO_AV_Endpoint_Process_Strategy_A::create_A (AVStreams::StreamEndPoint_A_ptr &stream_endpoint,
                                           AVStreams::VDev_ptr &vdev,
                                           CORBA::Environment &env)
{
  if (this->activate () == -1)
    ACE_ERROR_RETURN ((LM_ERROR, 
                       "(%P|%t) TAO_AV_Endpoint_Process_Strategy: Error in activate ()\n"),
                      -1);
  
  stream_endpoint = this->stream_endpoint_a_;
  vdev = this->vdev_;
  return 0;
  
}

int
TAO_AV_Endpoint_Process_Strategy_A::get_stream_endpoint (CORBA::Environment &env)
{
  CosNaming::Name Stream_Endpoint_A_Name (1);
  
  Stream_Endpoint_A_Name.length (1);
  Stream_Endpoint_A_Name [0].id = CORBA::string_dup
    ("Stream_Endpoint_A");
  
  CORBA::Object_var stream_endpoint_a =
    this->naming_context_->resolve (Stream_Endpoint_A_Name,
                                    env);
  TAO_CHECK_ENV_RETURN (env, -1);
  
  this->stream_endpoint_a_ =
    AVStreams::StreamEndPoint_A::_narrow (stream_endpoint_a.in (),
                                          env);
  TAO_CHECK_ENV_RETURN (env, -1);
  
  if (CORBA::is_nil (this->stream_endpoint_a_))
    ACE_ERROR_RETURN ((LM_ERROR,
                       " could not resolve Stream_Endpoint_A in Naming service <%s>\n"),
                      -1);
  return 0;
}

// ----------------------------------------------------------------------
// TAO_AV_Endpoint_Process_Strategy_B
// ----------------------------------------------------------------------

TAO_AV_Endpoint_Process_Strategy_B::TAO_AV_Endpoint_Process_Strategy_B (ACE_Process_Options *process_options)
  : TAO_AV_Endpoint_Process_Strategy (process_options)
{
}

int
TAO_AV_Endpoint_Process_Strategy_B::create_B (AVStreams::StreamEndPoint_B_ptr &stream_endpoint,
                                           AVStreams::VDev_ptr &vdev,
                                           CORBA::Environment &env)
{
  if (this->activate () == -1)
    ACE_ERROR_RETURN ((LM_ERROR, 
                       "(%P|%t) TAO_AV_Endpoint_Strategy: Error in activate ()\n"),
                      -1);

  stream_endpoint = this->stream_endpoint_b_;
  vdev = this->vdev_;
  return 0;
  
}

int
TAO_AV_Endpoint_Process_Strategy_B::get_stream_endpoint (CORBA::Environment &env)
{
  CosNaming::Name Stream_Endpoint_B_Name (1);
  
  Stream_Endpoint_B_Name.length (1);
  Stream_Endpoint_B_Name [0].id = CORBA::string_dup
    ("Stream_Endpoint_B");
  
  CORBA::Object_var stream_endpoint_b =
    this->naming_context_->resolve (Stream_Endpoint_B_Name,
                                    env);
  TAO_CHECK_ENV_RETURN (env, -1);
  
  this->stream_endpoint_b_ =
    AVStreams::StreamEndPoint_B::_narrow (stream_endpoint_b.in (),
                                          env);
  TAO_CHECK_ENV_RETURN (env, -1);
  
  if (CORBA::is_nil (this->stream_endpoint_b_))
    ACE_ERROR_RETURN ((LM_ERROR,
                       " could not resolve Stream_Endpoint_B in Naming service <%s>\n"),
                      -1);
  return 0;
}
// ----------------------------------------------------------------------
// TAO_AV_Endpoint_Reactive_Strategy
// ----------------------------------------------------------------------
template <class T_StreamEndpoint, class T_VDev , class T_MediaCtrl>
TAO_AV_Endpoint_Reactive_Strategy <T_StreamEndpoint, T_VDev, T_MediaCtrl>::TAO_AV_Endpoint_Reactive_Strategy (TAO_ORB_Manager *orb_manager)
  : orb_manager_ (orb_manager)
{
}

template <class T_StreamEndpoint, class T_VDev , class T_MediaCtrl>
int
TAO_AV_Endpoint_Reactive_Strategy <T_StreamEndpoint, T_VDev, T_MediaCtrl>::activate (void)
{
  TAO_TRY
    {
      this->activate_stream_endpoint (TAO_TRY_ENV);
      TAO_CHECK_ENV;
      
      this->activate_vdev (TAO_TRY_ENV);
      TAO_CHECK_ENV;
      
      this->activate_mediactrl (TAO_TRY_ENV);
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

template <class T_StreamEndpoint, class T_VDev , class T_MediaCtrl>
int
TAO_AV_Endpoint_Reactive_Strategy <T_StreamEndpoint, T_VDev, T_MediaCtrl>::activate_vdev (CORBA::Environment &env)
{
  // Bridge pattern. Allow subclasses to override this behavior
  this->vdev_ = 0;
  if (this->make_vdev (this->vdev_) == -1)
    return -1;

  this->orb_manager_->activate_under_child_poa ("VDev",
                                                this->vdev_,
                                                env);
  TAO_CHECK_ENV_RETURN (env, -1);
  return 0;
}


template <class T_StreamEndpoint, class T_VDev , class T_MediaCtrl>
int
TAO_AV_Endpoint_Reactive_Strategy <T_StreamEndpoint, T_VDev, T_MediaCtrl>::activate_mediactrl (CORBA::Environment &env)
{
  T_MediaCtrl *media_ctrl = 0;
  if (this->make_mediactrl (media_ctrl) == -1)
    return -1;

  this->orb_manager_->activate_under_child_poa ("MediaCtrl",
                                                media_ctrl,
                                                env);
  TAO_CHECK_ENV_RETURN (env, -1);
  
  CORBA::Any anyval;
  anyval <<= this->orb_manager_->orb ()->object_to_string (media_ctrl->_this (env));
  TAO_CHECK_ENV_RETURN (env, -1);
  
  this->vdev_->define_property ("Related_MediaCtrl",
                                anyval,
                                env);

  TAO_CHECK_ENV_RETURN (env, -1);

  return 0;
}

template <class T_StreamEndpoint, class T_VDev , class T_MediaCtrl>
int
TAO_AV_Endpoint_Reactive_Strategy<T_StreamEndpoint, T_VDev, T_MediaCtrl>::make_stream_endpoint (T_StreamEndpoint *&stream_endpoint)
{
  ACE_NEW_RETURN (stream_endpoint, 
                  T_StreamEndpoint,
                  -1);
  return 0;
}

template <class T_StreamEndpoint, class T_VDev , class T_MediaCtrl>
int
TAO_AV_Endpoint_Reactive_Strategy<T_StreamEndpoint, T_VDev, T_MediaCtrl>::make_vdev (T_VDev *&vdev)
{
  ACE_NEW_RETURN (vdev, 
                  T_VDev,
                  -1);
  return 0;
}

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
template <class T_StreamEndpoint, class T_VDev , class T_MediaCtrl>
TAO_AV_Endpoint_Reactive_Strategy_A <T_StreamEndpoint, T_VDev, T_MediaCtrl>::TAO_AV_Endpoint_Reactive_Strategy_A (TAO_ORB_Manager *orb_manager)
  : TAO_AV_Endpoint_Reactive_Strategy <T_StreamEndpoint, T_VDev,
    T_MediaCtrl> (orb_manager)
{
}

template <class T_StreamEndpoint, class T_VDev , class T_MediaCtrl>
int
TAO_AV_Endpoint_Reactive_Strategy_A<T_StreamEndpoint, T_VDev, T_MediaCtrl>::create_A (AVStreams::StreamEndPoint_A_ptr &stream_endpoint,
                                                                                      AVStreams::VDev_ptr &vdev,
                                                                                      CORBA::Environment &env)
{
  if (this->activate () == -1)
    ACE_ERROR_RETURN ((LM_ERROR, 
                       "(%P|%t) TAO_AV_Endpoint_Process_Strategy_A: Error in activate ()\n"),
                      -1);
  
  stream_endpoint = this->stream_endpoint_a_;
  vdev = this->vdev_;
  return 0;
  
}

template <class T_StreamEndpoint, class T_VDev , class T_MediaCtrl>
int
TAO_AV_Endpoint_Reactive_Strategy_A <T_StreamEndpoint, T_VDev, T_MediaCtrl>::activate_stream_endpoint (CORBA::Environment &env)
{
  this->stream_endpoint_a_ = 0;
  ACE_NEW_RETURN (this->stream_endpoint_a_,
                  T_StreamEndpoint,
                  -1);
  this->orb_manager_->activate_under_child_poa ("Stream_Endpoint_A",
                                                this->stream_endpoint_a_,
                                                env);
  TAO_CHECK_ENV_RETURN (env, -1);
  return 0;
}

// ----------------------------------------------------------------------
// TAO_AV_Endpoint_Reactive_Strategy_B
// ----------------------------------------------------------------------
template <class T_StreamEndpoint, class T_VDev , class T_MediaCtrl>
TAO_AV_Endpoint_Reactive_Strategy_B <T_StreamEndpoint, T_VDev, T_MediaCtrl>::TAO_AV_Endpoint_Reactive_Strategy_B (TAO_ORB_Manager *orb_manager)
  : TAO_AV_Endpoint_Reactive_Strategy <T_StreamEndpoint, T_VDev,
    T_MediaCtrl> (orb_manager)
{
}

template <class T_StreamEndpoint, class T_VDev , class T_MediaCtrl>
int
TAO_AV_Endpoint_Reactive_Strategy_B <T_StreamEndpoint, T_VDev, T_MediaCtrl>::activate_stream_endpoint (CORBA::Environment &env)
{
  this->stream_endpoint_b_ = 0;
  ACE_NEW_RETURN (this->stream_endpoint_b_,
                  T_StreamEndpoint,
                  -1);
  this->orb_manager_->activate_under_child_poa ("Stream_Endpoint_B",
                                                this->stream_endpoint_b_,
                                                env);
  TAO_CHECK_ENV_RETURN (env, -1);
  return 0;
}

template <class T_StreamEndpoint, class T_VDev , class T_MediaCtrl>
int
TAO_AV_Endpoint_Reactive_Strategy_B<T_StreamEndpoint, T_VDev, T_MediaCtrl>::create_B (AVStreams::StreamEndPoint_B_ptr &stream_endpoint,
                                                                                     AVStreams::VDev_ptr &vdev,
                                                                                     CORBA::Environment &env)
{
  if (this->activate () == -1)
    ACE_ERROR_RETURN ((LM_ERROR, 
                       "(%P|%t) TAO_AV_Endpoint_Process_Strategy_B: Error in activate ()\n"),
                      -1);
  
  stream_endpoint = this->stream_endpoint_b_;
  vdev = this->vdev_;
  return 0;
  
}

// ----------------------------------------------------------------------
// TAO_AV_Child_Process
// ----------------------------------------------------------------------

template <class T_StreamEndpoint_B, class T_VDev , class T_MediaCtrl>
TAO_AV_Child_Process <T_StreamEndpoint_B, T_VDev, T_MediaCtrl>::TAO_AV_Child_Process ()
  :stream_endpoint_name_ (0)
{
}

template <class T_StreamEndpoint_B, class T_VDev , class T_MediaCtrl>
int
TAO_AV_Child_Process  <T_StreamEndpoint_B, T_VDev, T_MediaCtrl>::init (int argc, char **argv)
{
  TAO_TRY
    {
      this->activate_objects (argc,
                              argv,
                              TAO_TRY_ENV);
      TAO_CHECK_ENV;

      this->bind_to_naming_service (TAO_TRY_ENV);
      TAO_CHECK_ENV;

      this->register_vdev (TAO_TRY_ENV);
      TAO_CHECK_ENV;

      this->register_stream_endpoint (TAO_TRY_ENV);
      TAO_CHECK_ENV;
    }
  TAO_CATCHANY
    {
      TAO_TRY_ENV.print_exception ("TAO_Child_Process");
      return -1;
    }
  TAO_ENDTRY;

  if (this->release_semaphore () == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "(%P|%t) Error releasing semaphores\n"),
                      -1);
  
  return 0;
}

template <class T_StreamEndpoint, class T_VDev , class T_MediaCtrl>
int
TAO_AV_Child_Process  <T_StreamEndpoint, T_VDev, T_MediaCtrl>::activate_objects (int argc, 
                                                                                char **argv,
                                                                                CORBA::Environment &env)
{
  this->orb_manager_.init_child_poa (argc,
                                     argv,
                                     "child_poa",
                                     env);
  TAO_CHECK_ENV_RETURN (env, -1);

  this->orb_manager_.activate_under_child_poa ("Stream_Endpoint",
                                               &this->stream_endpoint_,
                                               env);
  TAO_CHECK_ENV_RETURN (env, -1);


  this->orb_manager_.activate_under_child_poa ("VDev",
                                               &this->vdev_,
                                               env);
  TAO_CHECK_ENV_RETURN (env, -1);

  this->orb_manager_.activate_under_child_poa ("MediaCtrl",
                                               &this->media_control_,
                                               env);
  TAO_CHECK_ENV_RETURN (env, -1);
  return 0;

}

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

}

template <class T_StreamEndpoint, class T_VDev , class T_MediaCtrl>
int
TAO_AV_Child_Process  <T_StreamEndpoint, T_VDev, T_MediaCtrl>::register_vdev (CORBA::Environment &env)
{  
  CosNaming::Name vdev_name (1);
  vdev_name.length (1);
  vdev_name [0].id = CORBA::string_dup ("VDev");

  CORBA::Any media_ctrl_property;
  media_ctrl_property <<= this->orb_manager_.orb ()->object_to_string (this->media_ctrl_->_this (env));
  this->vdev_->define_property ("Related_MediaCtrl",
                                media_ctrl_property,
                                env);
  TAO_CHECK_ENV_RETURN (env,-1);
  
  // Register the vdev with the naming server.
  this->naming_context_->bind (vdev_name,
                               this->vdev_._this (env),
                               env);
  
  if (env.exception () != 0)
    {
      env.clear ();
      this->naming_context_->rebind (vdev_name,
                                     this->vdev_._this (env),
                                     env);
      TAO_CHECK_ENV_RETURN (env, -1);
    }
  
  return 0;
}

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

template <class T_StreamEndpoint_B, class T_VDev , class T_MediaCtrl>
int
TAO_AV_Child_Process  <T_StreamEndpoint_B, T_VDev, T_MediaCtrl>::release_semaphore ()
{
  char sem_str [BUFSIZ];
  char host [MAXHOSTNAMELEN];
  pid_t pid;
  
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

// ----------------------------------------------------------------------
// TAO_AV_Child_Process_A
// ----------------------------------------------------------------------

template <class T_StreamEndpoint, class T_VDev , class T_MediaCtrl>
int
TAO_AV_Child_Process  <T_StreamEndpoint, T_VDev, T_MediaCtrl>::register_stream_endpoint (CORBA::Environment &env)
{  
  //  ACE_DEBUG ((LM_DEBUG, "(%P|%t) %s:%d\n", __FILE__, __LINE__));
  // Create a name for the video control object
  CosNaming::Name Stream_Endpoint_Name (1);
  
  Stream_Endpoint_Name.length (1);
  Stream_Endpoint_Name [0].id = CORBA::string_dup (this->stream_endpoint_name_);

  
  // Register the stream endpoint object with the naming server.
  this->naming_context_->bind (Stream_Endpoint_Name,
                               this->stream_endpoint_._this (env),
                               env);

  if (env.exception () != 0)
    {
      env.clear ();
      this->naming_context_->rebind (Stream_Endpoint_Name,
                                     this->stream_endpoint_._this (env),
                                     env);
      TAO_CHECK_ENV_RETURN (env, -1);
    }

}

// ----------------------------------------------------------------------
// TAO_AV_Child_Process_A
// ----------------------------------------------------------------------

template <class T_StreamEndpoint, class T_VDev , class T_MediaCtrl>
TAO_AV_Child_Process_A  <T_StreamEndpoint, T_VDev, T_MediaCtrl>::TAO_AV_Child_Process_A ()
{  
  this->stream_endpoint_name_ = "Stream_Endpoint_A";
}

// ----------------------------------------------------------------------
// TAO_AV_Child_Process_A
// ----------------------------------------------------------------------

template <class T_StreamEndpoint, class T_VDev , class T_MediaCtrl>
TAO_AV_Child_Process_B  <T_StreamEndpoint, T_VDev, T_MediaCtrl>::TAO_AV_Child_Process_B ()
{  
  this->stream_endpoint_name_ = "Stream_Endpoint_B";
}
