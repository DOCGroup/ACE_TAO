// $Id$

#ifndef TAO_AV_ENDPOINT_STRATEGY_T_CPP
#define TAO_AV_ENDPOINT_STRATEGY_T_CPP

#include "orbsvcs/Log_Macros.h"
#include "orbsvcs/AV/Endpoint_Strategy_T.h"

#include "tao/debug.h"

#include "ace/Process_Semaphore.h"
#include "ace/OS_NS_unistd.h"


TAO_BEGIN_VERSIONED_NAMESPACE_DECL

// ----------------------------------------------------------------------
// TAO_AV_Endpoint_Reactive_Strategy
// ----------------------------------------------------------------------

template <class T_StreamEndpoint, class T_VDev , class T_MediaCtrl>
TAO_AV_Endpoint_Reactive_Strategy <T_StreamEndpoint, T_VDev, T_MediaCtrl>::TAO_AV_Endpoint_Reactive_Strategy (void)
   :  stream_endpoint_a_servant_(0), stream_endpoint_b_servant_(0), vdev_servant_(0),
      media_ctrl_servant_(0)
{
}

template <class T_StreamEndpoint, class T_VDev, class T_MediaCtrl>
TAO_AV_Endpoint_Reactive_Strategy <T_StreamEndpoint, T_VDev, T_MediaCtrl>::~TAO_AV_Endpoint_Reactive_Strategy (void)
{
  // Do not allow exceptions to escape from the destructor
  try
    {
      if(this->stream_endpoint_a_servant_ )
        {
          stream_endpoint_a_servant_->_remove_ref ();
        }

      if(this->stream_endpoint_b_servant_)
        {
          stream_endpoint_b_servant_->_remove_ref ();
        }

      if(this->vdev_servant_)
        {
          vdev_servant_->_remove_ref ();
        }

      if(this->media_ctrl_servant_)
        {
          media_ctrl_servant_->_remove_ref ();
        }
    }
  catch (const CORBA::Exception&)
    {
    }

}

// Create, activate the objects with the POA
template <class T_StreamEndpoint, class T_VDev , class T_MediaCtrl>
int
TAO_AV_Endpoint_Reactive_Strategy <T_StreamEndpoint, T_VDev, T_MediaCtrl>::activate (void)
{
  try
    {
      this->activate_stream_endpoint ();
      if (TAO_debug_level > 0) ORBSVCS_DEBUG ((LM_DEBUG,"(%P|%t)TAO_AV_Endpoint_Reactive_Strategy::activated stream_endpoint\n"));

      this->activate_vdev ();
      if (TAO_debug_level > 0) ORBSVCS_DEBUG ((LM_DEBUG,"(%P|%t)TAO_AV_Endpoint_Reactive_Strategy::activated vdev\n"));

      this->activate_mediactrl ();
      if (TAO_debug_level > 0) ORBSVCS_DEBUG ((LM_DEBUG,"(%P|%t)TAO_AV_Endpoint_Reactive_Strategy::activated mediactrl\n"));
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception (
        "TAO_Endpoint_Reactive_Strategy::activate");
      return -1;
    }
  return 0;
}

template <class T_StreamEndpoint, class T_VDev , class T_MediaCtrl>
char *
TAO_AV_Endpoint_Reactive_Strategy <T_StreamEndpoint, T_VDev, T_MediaCtrl>::activate_with_poa (PortableServer::Servant servant)
{

  PortableServer::ObjectId_var id =
    this->poa_->activate_object (servant);

  CORBA::Object_var obj =
    this->poa_->id_to_reference (id.in ());

  CORBA::String_var str =
    this->orb_->object_to_string (obj.in ());

  return str._retn ();
}


// Activate VDev into the POA
template <class T_StreamEndpoint, class T_VDev , class T_MediaCtrl>
int
TAO_AV_Endpoint_Reactive_Strategy <T_StreamEndpoint, T_VDev, T_MediaCtrl>::activate_vdev (void)
{
  try
    {
      // Bridge pattern. Allow subclasses to override this behavior
      T_VDev *vdev = 0;
      if (this->make_vdev (vdev) == -1)
        return -1;

      // Activate the object under the root poa.
//      CORBA::String_var vdev_ior = this->activate_with_poa (vdev,
//);
//      if (TAO_debug_level > 0) ORBSVCS_DEBUG ((LM_DEBUG,"(%P|%t)TAO_AV_Endpoint_Reactive_Strategy::activate_vdev, vdev ior is:%s\n",
//                  vdev_ior. in ()));

      // Save the object reference, so that create_A can return it
      this->vdev_ = vdev->_this ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception (
        "TAO_AV_Endpoint_Reactive_Strategy::activate_vdev");
      return -1;
    }
  return 0;
}


// Activate the media_controller
template <class T_StreamEndpoint, class T_VDev , class T_MediaCtrl>
int
TAO_AV_Endpoint_Reactive_Strategy <T_StreamEndpoint, T_VDev, T_MediaCtrl>::activate_mediactrl (void)
{
  try
    {
      // Bridge pattern. Subclasses can override this
      if (this->make_mediactrl ( media_ctrl_servant_ ) == -1)
        return -1;

      // Associate the media controller object reference with the vdev, as per the OMG spec
      CORBA::Any anyval;
      media_ctrl_obj_
        = media_ctrl_servant_->_this ();

      anyval <<= media_ctrl_obj_.in();


     this->vdev_->define_property ("Related_MediaCtrl",
                                   anyval);

    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception (
        "TAO_AV_Endpoint_Reactive_Strategy::activate_mediactrl");
      return -1;
    }
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

//Constructor
template <class T_StreamEndpoint, class T_VDev , class T_MediaCtrl>
TAO_AV_Endpoint_Reactive_Strategy_A <T_StreamEndpoint, T_VDev, T_MediaCtrl>::TAO_AV_Endpoint_Reactive_Strategy_A (void)
{
}


// Destructor
template <class T_StreamEndpoint, class T_VDev , class T_MediaCtrl>
TAO_AV_Endpoint_Reactive_Strategy_A <T_StreamEndpoint, T_VDev, T_MediaCtrl>::~TAO_AV_Endpoint_Reactive_Strategy_A (void)
{
}


template <class T_StreamEndpoint, class T_VDev , class T_MediaCtrl>
int
TAO_AV_Endpoint_Reactive_Strategy_A <T_StreamEndpoint, T_VDev, T_MediaCtrl>::init (CORBA::ORB_ptr orb, PortableServer::POA_ptr poa)
{
  this->orb_ = CORBA::ORB::_duplicate (orb);
  this->poa_ = PortableServer::POA::_duplicate (poa);
  return 0;
}

// Creates an "A" type streamendpoint, and a vdev and returns the
// object references
template <class T_StreamEndpoint, class T_VDev , class T_MediaCtrl>
int
TAO_AV_Endpoint_Reactive_Strategy_A<T_StreamEndpoint, T_VDev, T_MediaCtrl>::create_A (AVStreams::StreamEndPoint_A_ptr &stream_endpoint,
                                                                                      AVStreams::VDev_ptr &vdev)
{
  if (this->activate () == -1)
    ORBSVCS_ERROR_RETURN ((LM_ERROR,
                       "(%P|%t) TAO_AV_Endpoint_Reactive_Strategy_A: Error in activate ()\n"),
                      -1);

  stream_endpoint = AVStreams::StreamEndPoint_A::_duplicate( this->stream_endpoint_a_.in() );
  vdev = AVStreams::VDev::_duplicate( this->vdev_.in() );
  return 0;

}

// Put the stream_endpoint into the POA
template <class T_StreamEndpoint, class T_VDev , class T_MediaCtrl>
int
TAO_AV_Endpoint_Reactive_Strategy_A <T_StreamEndpoint, T_VDev, T_MediaCtrl>::activate_stream_endpoint (void)
{
  try
    {

      // Use the bridge method
      if (this->make_stream_endpoint (this->stream_endpoint_a_servant_) == -1)
        return -1;

      // Save the object references, so that create_a can return them
      this->stream_endpoint_a_ = this->stream_endpoint_a_servant_->_this ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception (
        "TAO_AV_Endpoint_Reactive_Strategy_A::activate_stream_endpoint");
      return -1;
    }
  return 0;
}

// ----------------------------------------------------------------------
// TAO_AV_Endpoint_Reactive_Strategy_B
// ----------------------------------------------------------------------


// Constructor
template <class T_StreamEndpoint, class T_VDev , class T_MediaCtrl>
TAO_AV_Endpoint_Reactive_Strategy_B <T_StreamEndpoint, T_VDev, T_MediaCtrl>::TAO_AV_Endpoint_Reactive_Strategy_B (void)
{
}

// Destructor
template <class T_StreamEndpoint, class T_VDev , class T_MediaCtrl>
TAO_AV_Endpoint_Reactive_Strategy_B <T_StreamEndpoint, T_VDev, T_MediaCtrl>::~TAO_AV_Endpoint_Reactive_Strategy_B (void)
{
}


template <class T_StreamEndpoint, class T_VDev , class T_MediaCtrl>
int
TAO_AV_Endpoint_Reactive_Strategy_B <T_StreamEndpoint, T_VDev, T_MediaCtrl>::init (CORBA::ORB_ptr orb, PortableServer::POA_ptr poa)
{
  this->orb_ = CORBA::ORB::_duplicate (orb);
  this->poa_ = PortableServer::POA::_duplicate (poa);
  return 0;
}

// Activate stream_endpoint
template <class T_StreamEndpoint, class T_VDev , class T_MediaCtrl>
int
TAO_AV_Endpoint_Reactive_Strategy_B <T_StreamEndpoint, T_VDev, T_MediaCtrl>::activate_stream_endpoint (void)
{
  try
    {
      if (this->make_stream_endpoint ( this->stream_endpoint_b_servant_ ) == -1)
        return -1;

      this->stream_endpoint_b_ = this->stream_endpoint_b_servant_->_this ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception (
        "TAO_AV_Endpoint_Reactive_Strategy_B::activate_stream_endpoint");
      return -1;
    }
  return 0;
}

// Returns a "B" type stream_endpoint and a vdev
template <class T_StreamEndpoint, class T_VDev , class T_MediaCtrl>
int
TAO_AV_Endpoint_Reactive_Strategy_B<T_StreamEndpoint, T_VDev, T_MediaCtrl>::create_B (AVStreams::StreamEndPoint_B_ptr &stream_endpoint,
                                                                                      AVStreams::VDev_ptr &vdev)
{
  if (this->activate () == -1)
    ORBSVCS_ERROR_RETURN ((LM_ERROR,
                       "(%P|%t) TAO_AV_Endpoint_Reactive_Strategy_B: Error in activate ()\n"),
                      -1);

  stream_endpoint = AVStreams::StreamEndPoint_B::_duplicate( this->stream_endpoint_b_.in() );
  vdev = AVStreams::VDev::_duplicate( this->vdev_.in() );

  return 0;
}

// ----------------------------------------------------------------------
// TAO_AV_Child_Process
// ----------------------------------------------------------------------

// Constructor
template <class T_StreamEndpoint_B, class T_VDev , class T_MediaCtrl>
TAO_AV_Child_Process <T_StreamEndpoint_B, T_VDev, T_MediaCtrl>::TAO_AV_Child_Process ()
  : stream_endpoint_name_ (0),
    pid_ (-1),
    stream_endpoint_(0),
    vdev_ (0),
    media_ctrl_ (0)
{
  this->pid_ = ACE_OS::getpid ();
  if (this->pid_ == 0)
    ORBSVCS_ERROR ((LM_ERROR,"getpid () failed\n"));
  ACE_OS::hostname (this->host_,
                    sizeof this->host_);
}

// Initializes the ORB, activates the objects, and release the semaphore
template <class T_StreamEndpoint_B, class T_VDev , class T_MediaCtrl>
int
TAO_AV_Child_Process  <T_StreamEndpoint_B, T_VDev, T_MediaCtrl>::init (int argc,
                                                                       ACE_TCHAR **argv,
                                                                       CORBA::ORB_ptr orb,
                                                                       PortableServer::POA_ptr poa)
{
  try
    {
      this->orb_ = orb;

      this->poa_ = poa;

      // create the objects and activate them in the poa
      this->activate_objects (argc,
                              argv);

      // Get ourselves a naming_service object reference
      this->bind_to_naming_service ();

      // Register the vdev with the naming service
      this->register_vdev ();

      // register the stream_endpoing with the naming_service
      this->register_stream_endpoint ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("TAO_Child_Process");
      return -1;
    }

  // release the semaphore the parent is waiting on
  if (this->release_semaphore () == -1)
    ORBSVCS_ERROR_RETURN ((LM_ERROR,
                       "(%P|%t) Error releasing semaphores\n"),
                      -1);

  return 0;
}

template <class T_StreamEndpoint, class T_VDev , class T_MediaCtrl>
char *
TAO_AV_Child_Process <T_StreamEndpoint, T_VDev, T_MediaCtrl>::activate_with_poa (PortableServer::Servant servant)
{

  PortableServer::ObjectId_var id =
    this->poa_->activate_object (servant);

  CORBA::Object_var obj =
    this->poa_->id_to_reference (id.in ());

  CORBA::String_var str =
    this->orb_->object_to_string (obj.in ());

  return str._retn ();
}

// initializes the orb, and activates the objects
template <class T_StreamEndpoint, class T_VDev , class T_MediaCtrl>
int
TAO_AV_Child_Process  <T_StreamEndpoint, T_VDev, T_MediaCtrl>::activate_objects (int /*argc*/,
                                                                                 ACE_TCHAR ** /*argv*/)
{
  try
    {
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
      CORBA::String_var stream_endpoint_ior = this->activate_with_poa (this->stream_endpoint_);
      if (TAO_debug_level > 0) ORBSVCS_DEBUG ((LM_DEBUG,
                                           "(%P|%t)TAO_AV_Child_Process::activate_objects,stream_endpoint_ior :%s\n",
                                           stream_endpoint_ior.in ()));

      // activate the vdev
      CORBA::String_var vdev_ior = this->activate_with_poa (this->vdev_);
      if (TAO_debug_level > 0) ORBSVCS_DEBUG ((LM_DEBUG,
                                           "(%P|%t)TAO_AV_Child_Process::activate_objects, vdev ior is :%s\n",
                                           vdev_ior.in ()));

      // activate the media controller
      CORBA::String_var media_ctrl_ior = this->activate_with_poa (this->media_ctrl_);

      if (TAO_debug_level > 0) ORBSVCS_DEBUG ((LM_DEBUG,"(%P|%t)TAO_AV_Child_Process::activate_objects,media_ctrl_ior is: %s\n",media_ctrl_ior.in ()));
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("TAO_AV_Child_Process::init ");
      return -1;
    }
  return 0;
}

// Bind to the namingservice
template <class T_StreamEndpoint, class T_VDev , class T_MediaCtrl>
int
TAO_AV_Child_Process  <T_StreamEndpoint, T_VDev, T_MediaCtrl>::bind_to_naming_service (void)
{
  try
    {
      CORBA::Object_var naming_obj =
        this->orb_->resolve_initial_references ("NameService");
      if (CORBA::is_nil (naming_obj.in ()))
        ORBSVCS_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Unable to resolve the Name Service.\n"),
                          -1);
      //  if (TAO_debug_level > 0) ORBSVCS_DEBUG ((LM_DEBUG, "(%P|%t) %s:%d\n", __FILE__, __LINE__));
      this->naming_context_ =
        CosNaming::NamingContext::_narrow (naming_obj.in ());
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception (
        "TAO_AV_Child_Process::bind_to_naming_service");
      return -1;
    }
  return 0;
}

// register the vdev with the naming service
template <class T_StreamEndpoint, class T_VDev , class T_MediaCtrl>
int
TAO_AV_Child_Process  <T_StreamEndpoint, T_VDev, T_MediaCtrl>::register_vdev (void)
{
  try
    {
      char vdev_name [BUFSIZ];
      ACE_OS::sprintf (vdev_name,
                       "%s:%s:%ld",
                       "VDev",
                       this->host_,
                       static_cast<long> (this->pid_));

      if (TAO_debug_level > 0) ORBSVCS_DEBUG ((LM_DEBUG,"(%P|%t)%s\n",vdev_name));
      // create the name
      this->vdev_name_.length (1);
      this->vdev_name_ [0].id = CORBA::string_dup (vdev_name);

      // make the media controller a property of the vdev
      CORBA::Any media_ctrl_property;
      media_ctrl_obj_ =
        this->media_ctrl_->_this ();

      this->vdev_->define_property ("Related_MediaCtrl",
                                    media_ctrl_obj_.in());

      vdev_obj_ = this->vdev_->_this ();

      try
        {
          // Register the vdev with the naming server.
          this->naming_context_->bind (this->vdev_name_,
                                       vdev_obj_.in());
        }
      catch (const CosNaming::NamingContext::AlreadyBound& )
        {
          // If the object was already there, replace the older reference
          // with this one
          this->naming_context_->rebind (this->vdev_name_,
                                         vdev_obj_.in());
        }
      catch (const CORBA::Exception& ex)
        {
          ex._tao_print_exception (
            "TAO_AV_Child_Process::register_vdev");
          return -1;
        }
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception (
        "TAO_AV_Child_Process::register_vdev");
      return -1;
    }
  return 0;
}

// run the orb event look
template <class T_StreamEndpoint_B, class T_VDev , class T_MediaCtrl>
int
TAO_AV_Child_Process  <T_StreamEndpoint_B, T_VDev, T_MediaCtrl>::run (ACE_Time_Value *tv)
{
  try
    {
      this->orb_->run (tv);
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("orb.run ()");
      return -1;
    }
  return 0;
}

// release the semaphore the parent is waiting on
template <class T_StreamEndpoint_B, class T_VDev , class T_MediaCtrl>
int
TAO_AV_Child_Process  <T_StreamEndpoint_B, T_VDev, T_MediaCtrl>::release_semaphore ()
{
  char sem_str [BUFSIZ];

  long pid = this->pid_;
  ACE_OS::sprintf (sem_str,
                   "%s:%s:%ld",
                   "TAO_AV_Process_Semaphore",
                   this->host_,
                   pid);

  if (TAO_debug_level > 0) ORBSVCS_DEBUG ((LM_DEBUG,
              "(%P|%t) semaphore is %s\n",
              sem_str));

  // Release the lock on which the server is waiting
  ACE_Process_Semaphore semaphore (0, // 0 means that the semaphore is
                                   // initially locked
                                   sem_str);

  if (semaphore.release () == -1)
    ORBSVCS_ERROR_RETURN ((LM_ERROR,
                       "(%P|%t) Error releasing semaphore %s: %p\n",
                       sem_str,
                       "semaphore.release ()"),
                      -1);

  return 0;
}

// register the stream_endpoint with the naming service
template <class T_StreamEndpoint, class T_VDev , class T_MediaCtrl>
int
TAO_AV_Child_Process  <T_StreamEndpoint, T_VDev, T_MediaCtrl>::register_stream_endpoint (void)
{
  CORBA::Object_ptr stream_endpoint_obj = CORBA::Object::_nil ();
  try
    {
      stream_endpoint_obj = this->stream_endpoint_->_this ();

      // Create a name for the video control object
      // subclasses can define their own name for the streamendpoint
      // Register the stream endpoint object with the naming server.
      this->naming_context_->bind (this->stream_endpoint_name_,
                                   stream_endpoint_obj);
    }
  catch (const CosNaming::NamingContext::AlreadyBound& )
    {
      // if the name was already there, replace the reference with the new one
      this->naming_context_->rebind (this->stream_endpoint_name_,
                                     stream_endpoint_obj);
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception (
        "TAO_Endpoint_Reactive_Strategy::activate");
      return -1;
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
  if (TAO_debug_level > 0) ORBSVCS_DEBUG ((LM_DEBUG,"(%P|%t) TAO_AV_Child_Process::make_mediactrl ()\n"));
  ACE_NEW_RETURN (media_ctrl,
                  T_MediaCtrl,
                  -1);
  return 0;
}


template <class T_StreamEndpoint, class T_VDev , class T_MediaCtrl>
int
TAO_AV_Child_Process<T_StreamEndpoint, T_VDev, T_MediaCtrl>::unbind_names (void)
{
  // Remove the names from the naming service
  try
    {
      if (CORBA::is_nil (this->naming_context_.in ()) == 0)
        return 0;
      this->naming_context_->unbind (this->stream_endpoint_name_);

      this->naming_context_->unbind (this->vdev_name_);
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception (
        "TAO_Endpoint_Process_Strategy::activate");
      return -1;
    }
  return 0;
}

// %% its not clear whether we should be deleting the objects, since
// if the application overrides the make_mediactrl methods etc.,
// then, we may not own these objects.
// For now, we dont delete the objects, since they exist for the
// lifetime of the process anyway
template <class T_StreamEndpoint, class T_VDev , class T_MediaCtrl>
TAO_AV_Child_Process<T_StreamEndpoint, T_VDev, T_MediaCtrl>::~TAO_AV_Child_Process ()
{
  this->unbind_names ();
}

// ----------------------------------------------------------------------
// TAO_AV_Child_Process_A
// ----------------------------------------------------------------------

// Define the name of the stream_endpoint, as used to register with
// the  naming service
template <class T_StreamEndpoint, class T_VDev , class T_MediaCtrl>
TAO_AV_Child_Process_A<T_StreamEndpoint, T_VDev, T_MediaCtrl>::TAO_AV_Child_Process_A ()
{
  char stream_endpoint_name[BUFSIZ];
  ACE_OS::sprintf (stream_endpoint_name,
                   "%s:%s:%ld",
                   "Stream_Endpoint_A",
                   this->host_,
                   static_cast<long> (this->pid_));

  if (TAO_debug_level > 0) ORBSVCS_DEBUG ((LM_DEBUG,"(%P|%t)%s\n",stream_endpoint_name));
  this->stream_endpoint_name_.length (1);
  this->stream_endpoint_name_ [0].id = CORBA::string_dup (stream_endpoint_name);
}

template <class T_StreamEndpoint, class T_VDev , class T_MediaCtrl>
TAO_AV_Child_Process_A  <T_StreamEndpoint, T_VDev, T_MediaCtrl>::~TAO_AV_Child_Process_A ()
{
}

// ----------------------------------------------------------------------
// TAO_AV_Child_Process_B
// ----------------------------------------------------------------------

// Define the name of the stream_endpoint, as used to register with
// the  naming service
template <class T_StreamEndpoint, class T_VDev , class T_MediaCtrl>
TAO_AV_Child_Process_B<T_StreamEndpoint, T_VDev, T_MediaCtrl>::TAO_AV_Child_Process_B ()
{
  char stream_endpoint_name[BUFSIZ];
  ACE_OS::sprintf (stream_endpoint_name,
                   "%s:%s:%ld",
                   "Stream_Endpoint_B",
                   this->host_,
                   static_cast<long> (this->pid_));

  if (TAO_debug_level > 0) ORBSVCS_DEBUG ((LM_DEBUG,"(%P|%t)%s\n",stream_endpoint_name));
  this->stream_endpoint_name_.length (1);
  this->stream_endpoint_name_ [0].id = CORBA::string_dup (stream_endpoint_name);
}

template <class T_StreamEndpoint, class T_VDev , class T_MediaCtrl>
TAO_AV_Child_Process_B<T_StreamEndpoint, T_VDev, T_MediaCtrl>::~TAO_AV_Child_Process_B ()
{
}

TAO_END_VERSIONED_NAMESPACE_DECL

#endif /* TAO_AV_ENDPOINT_STRATEGY_T_CPP */
