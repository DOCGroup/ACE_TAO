// $Id$

#ifndef TAO_AV_ENDPOINT_STRATEGY_T_H
#define TAO_AV_ENDPOINT_STRATEGY_T_H

#include "Endpoint_Strategy.h"

template <class T_StreamEndpoint, class T_VDev , class T_MediaCtrl>
class TAO_ORBSVCS_Export TAO_AV_Endpoint_Reactive_Strategy
  : public TAO_AV_Endpoint_Strategy
// = DESCRIPTION
//    Reactive strategy base class
{
 protected:
  TAO_AV_Endpoint_Reactive_Strategy (TAO_ORB_Manager *orb_manager);
  // Constructor

  virtual ~TAO_AV_Endpoint_Reactive_Strategy (void);
  // Destructor.

  virtual int activate (void);
  // creates and activates the streamendpoint, vdev, and mediacontrol

  virtual int activate_stream_endpoint (CORBA::Environment &env) = 0;
  // activates the stream_endpoint with the POA

  virtual int activate_vdev (CORBA::Environment &env);
  // activates the vdev with the POA

  virtual int activate_mediactrl (CORBA::Environment &env);
  // activates the media controller with the POA

  virtual int make_vdev (T_VDev *&vdev);
  // Bridge method to create a vdev, a la Acceptor. Applications
  // can override this

  virtual int make_stream_endpoint (T_StreamEndpoint *&stream_endpoint);
  // Bridge method to create a stream_endpoint, a la Acceptor. Applications
  // can override this

  virtual int make_mediactrl (T_MediaCtrl *&media_ctrl);
  // Bridge method to create a media_ctrl, a la Acceptor. Applications
  // can override this

  TAO_ORB_Manager *orb_manager_;
  // ORB manager, used to activate the objects

};
// ----------------------------------------------------------------------

template <class T_StreamEndpoint, class T_VDev , class T_MediaCtrl>
class TAO_ORBSVCS_Export TAO_AV_Endpoint_Reactive_Strategy_A
  : public TAO_AV_Endpoint_Reactive_Strategy<T_StreamEndpoint, T_VDev , T_MediaCtrl>
// = DESCRIPTION
//    Reactive strategy
{
 public:
  TAO_AV_Endpoint_Reactive_Strategy_A (TAO_ORB_Manager *orb_manager);
  // Constructor

  virtual ~TAO_AV_Endpoint_Reactive_Strategy_A (void);
  // Destructor.

  virtual int activate_stream_endpoint (CORBA::Environment &env);
  // Overrides the base class stream_endpoint activator, to activate
  // an "A" type endpoint

  virtual int create_A (AVStreams::StreamEndPoint_A_ptr &stream_endpoint,
                        AVStreams::VDev_ptr &vdev,
                        CORBA::Environment &env);
  // Called by the MMDevice, when it needs to create an A type endpoint


};

// ----------------------------------------------------------------------
template <class T_StreamEndpoint, class T_Vdev , class T_MediaCtrl>
class TAO_ORBSVCS_Export TAO_AV_Endpoint_Reactive_Strategy_B
  : public TAO_AV_Endpoint_Reactive_Strategy <T_StreamEndpoint, T_Vdev, T_MediaCtrl>
// = DESCRIPTION
//    Reactive strategy
{
 public:
  TAO_AV_Endpoint_Reactive_Strategy_B (TAO_ORB_Manager *);
  // Constructor.

  virtual ~TAO_AV_Endpoint_Reactive_Strategy_B (void);
  // Destructor.

  virtual int activate_stream_endpoint (CORBA::Environment &env);
  // Overrides the base class stream_endpoint activator, to activate
  // a "B" type endpoint

  virtual int create_B (AVStreams::StreamEndPoint_B_ptr &stream_endpoint,
                        AVStreams::VDev_ptr &vdev,
                        CORBA::Environment &env);
  // Called by the MMDevice, when it needs to create a B type endpoint
};

// ----------------------------------------------------------------------

template <class T_StreamEndpoint, class T_VDev , class T_MediaCtrl>
class TAO_ORBSVCS_Export TAO_AV_Child_Process
// = DESCRIPTION
//    Helper class for the child process created in TAO_AV_Endpoint_Process_Strategy
{
public:
  TAO_AV_Child_Process ();
  // Constructor

  virtual ~TAO_AV_Child_Process ();
  // Destructor

  int init (int argc, char **argv);
  // Initializes the ORB, creates and activates the
  // T_StreamEndpoint, T_VDev, T_MediaCtrl in the POA

  int run (ACE_Time_Value *tv = 0);
  // runs the ORB event loop

 protected:
  int activate_objects (int argc,
                        char **argv,
                        CORBA::Environment &env);
  // Creates the objects and inserts them into the Naming
  // Service, so the parent can pick the IOR's and
  // return them to the client


  int bind_to_naming_service (CORBA::Environment &env);
  // Binds to the naming service

  int register_vdev (CORBA::Environment &env);
  // Registers vdev with the naming service

  int register_stream_endpoint (CORBA::Environment &env);
  // Registers stream_endpoint with the naming service

  int release_semaphore ();
  // Releases the semaphore on which the parent is waiting on

  virtual int make_vdev (T_VDev *&vdev);
  // Bridge method to create a vdev, a la Acceptor. Applications
  // can override this

  virtual int make_stream_endpoint (T_StreamEndpoint *&stream_endpoint);
  // Bridge method to create a stream_endpoint, a la Acceptor. Applications
  // can override this

  virtual int make_mediactrl (T_MediaCtrl *&media_ctrl);
  // Bridge method to create a media_ctrl, a la Acceptor. Applications
  // can override this

  TAO_ORB_Manager orb_manager_;
  // The ORB Manager

  CosNaming::NamingContext_var naming_context_;
  // The root Naming Context of the TAO naming service

  CosNaming::Name vdev_name_;
  // Name of the vdev

  CosNaming::Name stream_endpoint_name_;
  // Name of the stream_endpoint

  T_StreamEndpoint *stream_endpoint_;
  // The stream endpoint member

  T_VDev *vdev_;
  // The virtual device

  T_MediaCtrl *media_ctrl_;
  // Media controller

  pid_t pid_;
  // pid of this process

  char host_[MAXHOSTNAMELEN];
  // Name of the host.
};

// ----------------------------------------------------------------------

template <class T_StreamEndpoint, class T_VDev , class T_MediaCtrl>
class TAO_ORBSVCS_Export TAO_AV_Child_Process_A
  : public TAO_AV_Child_Process <T_StreamEndpoint, T_VDev, T_MediaCtrl>
// = DESCRIPTION
//    Helper class for the child process created in TAO_AV_Child_Process
{
public:
  TAO_AV_Child_Process_A (void);
  // Constructor.

  virtual ~TAO_AV_Child_Process_A (void);
  // Destructor.
};

// ----------------------------------------------------------------------

template <class T_StreamEndpoint, class T_VDev , class T_MediaCtrl>
class TAO_ORBSVCS_Export TAO_AV_Child_Process_B
  : public TAO_AV_Child_Process <T_StreamEndpoint, T_VDev, T_MediaCtrl>
// = DESCRIPTION
//    Helper class for the child process created in TAO_AV_Child_Process
{
public:
  TAO_AV_Child_Process_B (void);
  // Constructor.

  virtual ~TAO_AV_Child_Process_B (void);
  // Destructor.
};

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "Endpoint_Strategy_T.cpp"
#endif /*ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("Endpoint_Strategy_T.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#endif /* TAO_AV_ENDPOINT_STRATEGY_T_H */
