/* -*- C++ -*- */


//=============================================================================
/**
 *  @file   Endpoint_Strategy_T.h
 *
 *  $Id$
 *
 *  @author Sumedh Mungee <sumedh@cs.wustl.edu>
 *  @author Nagarajan Surendran <naga@cs.wustl.edu>
 */
//=============================================================================



#ifndef TAO_AV_ENDPOINT_STRATEGY_T_H
#define TAO_AV_ENDPOINT_STRATEGY_T_H
#include /**/ "ace/pre.h"

#include "Endpoint_Strategy.h"

/**
 * @class TAO_AV_Endpoint_Reactive_Strategy
 * @brief  Reactive strategy base class
 */
template <class T_StreamEndpoint, class T_VDev , class T_MediaCtrl>
class TAO_AV_Export TAO_AV_Endpoint_Reactive_Strategy
: public TAO_AV_Endpoint_Strategy
{

protected:
  // Constructor

  /// Constructor
  TAO_AV_Endpoint_Reactive_Strategy (void);

  /// Destructor.
  virtual ~TAO_AV_Endpoint_Reactive_Strategy (void);

  /// creates and activates the streamendpoint, vdev, and mediacontrol
  virtual int activate (void);

  /// activates the stream_endpoint with the POA
  virtual int activate_stream_endpoint (ACE_ENV_SINGLE_ARG_DECL) = 0;

  /// activates the vdev with the POA
  virtual int activate_vdev (ACE_ENV_SINGLE_ARG_DECL);



  /// activates the media controller with the POA
  virtual int activate_mediactrl (ACE_ENV_SINGLE_ARG_DECL);

  /// Bridge method to create a vdev, a la Acceptor. Applications
  /// can override this
  virtual int make_vdev (T_VDev *&vdev);

  /// Bridge method to create a stream_endpoint, a la Acceptor. Applications
  /// can override this
  virtual int make_stream_endpoint (T_StreamEndpoint *&stream_endpoint);

  /// Bridge method to create a media_ctrl, a la Acceptor. Applications
  /// can override this
  virtual int make_mediactrl (T_MediaCtrl *&media_ctrl);


  char* activate_with_poa (PortableServer::Servant servant ACE_ENV_ARG_DECL);

  CORBA::ORB_var orb_;

  PortableServer::POA_var poa_;

  CORBA::Object_var media_ctrl_obj_;

  T_StreamEndpoint *stream_endpoint_a_servant_;

  T_StreamEndpoint *stream_endpoint_b_servant_;

  T_VDev *vdev_servant_;

  T_MediaCtrl *media_ctrl_servant_;
};
// ----------------------------------------------------------------------

/**
 * @class TAO_AV_Endpoint_Reactive_Strategy_A
 * @brief Reactive strategy
 */
template <class T_StreamEndpoint, class T_VDev , class T_MediaCtrl>
class TAO_AV_Export TAO_AV_Endpoint_Reactive_Strategy_A
  : public TAO_AV_Endpoint_Reactive_Strategy<T_StreamEndpoint, T_VDev , T_MediaCtrl>
{

public:

  TAO_AV_Endpoint_Reactive_Strategy_A (void);

  /// Constructor
  int init (CORBA::ORB_ptr orb, PortableServer::POA_ptr poa);

  /// Destructor.
  virtual ~TAO_AV_Endpoint_Reactive_Strategy_A (void);

  /// Overrides the base class stream_endpoint activator, to activate
  /// an "A" type endpoint
  virtual int activate_stream_endpoint (ACE_ENV_SINGLE_ARG_DECL);

  /// Called by the MMDevice, when it needs to create an A type endpoint
  virtual int create_A (AVStreams::StreamEndPoint_A_ptr &stream_endpoint,
                        AVStreams::VDev_ptr &vdev
                        ACE_ENV_ARG_DECL);

};

// ----------------------------------------------------------------------
/**
 * @class TAO_AV_Endpoint_Reactive_Strategy_B
 * @brief Reactive strategy
 */
template <class T_StreamEndpoint, class T_Vdev , class T_MediaCtrl>
class TAO_AV_Export TAO_AV_Endpoint_Reactive_Strategy_B
  : public TAO_AV_Endpoint_Reactive_Strategy <T_StreamEndpoint, T_Vdev, T_MediaCtrl>
{

public:

  /// Constructor
  TAO_AV_Endpoint_Reactive_Strategy_B (void);

  /// Destructor.
  virtual ~TAO_AV_Endpoint_Reactive_Strategy_B (void);

  int init (CORBA::ORB_ptr orb, PortableServer::POA_ptr poa);

  /// Overrides the base class stream_endpoint activator, to activate
  /// a "B" type endpoint
  virtual int activate_stream_endpoint (ACE_ENV_SINGLE_ARG_DECL);

  /// Called by the MMDevice, when it needs to create a B type endpoint
  virtual int create_B (AVStreams::StreamEndPoint_B_ptr &stream_endpoint,
                        AVStreams::VDev_ptr &vdev
                        ACE_ENV_ARG_DECL);
};

// ----------------------------------------------------------------------

/**
 * @class TAO_AV_Child_Process
 *
 * @brief Helper class for the child process created in
 *        TAO_AV_Endpoint_Process_Strategy
 */
template <class T_StreamEndpoint, class T_VDev , class T_MediaCtrl>
class TAO_AV_Export TAO_AV_Child_Process
{

public:
  /// Constructor
  TAO_AV_Child_Process ();

  /// Destructor
  virtual ~TAO_AV_Child_Process ();

  /// Initializes the ORB, creates and activates the
  /// T_StreamEndpoint, T_VDev, T_MediaCtrl in the POA
  int init (int argc, char **argv, CORBA::ORB_ptr orb, PortableServer::POA_ptr poa);

  /// runs the ORB event loop
  int run (ACE_Time_Value *tv = 0);

protected:
  /**
   * Creates the objects and inserts them into the Naming
   * Service, so the parent can pick the IOR's and
   * return them to the client
   */
  int activate_objects (int argc,
                        char **argv
                        ACE_ENV_ARG_DECL);

  ///activate the servant with the poa
  char* activate_with_poa (PortableServer::Servant servant ACE_ENV_ARG_DECL);

  /// Removes the vdev and streamendpoint names from the naming service.
  int unbind_names (void);

  /// Binds to the naming service
  int bind_to_naming_service (ACE_ENV_SINGLE_ARG_DECL);

  /// Registers vdev with the naming service
  int register_vdev (ACE_ENV_SINGLE_ARG_DECL);

  /// Registers stream_endpoint with the naming service
  int register_stream_endpoint (ACE_ENV_SINGLE_ARG_DECL);

  /// Releases the semaphore on which the parent is waiting on
  int release_semaphore ();

  /// Bridge method to create a vdev, a la Acceptor. Applications
  /// can override this
  virtual int make_vdev (T_VDev *&vdev);

  /// Bridge method to create a stream_endpoint, a la Acceptor. Applications
  /// can override this
  virtual int make_stream_endpoint (T_StreamEndpoint *&stream_endpoint);

  /// Bridge method to create a media_ctrl, a la Acceptor. Applications
  /// can override this
  virtual int make_mediactrl (T_MediaCtrl *&media_ctrl);

  /// The root Naming Context of the TAO naming service
  CosNaming::NamingContext_var naming_context_;

  /// Name of the vdev
  CosNaming::Name vdev_name_;

  /// Name of the stream_endpoint
  CosNaming::Name stream_endpoint_name_;

  /// The stream endpoint member
  T_StreamEndpoint *stream_endpoint_;

  /// The virtual device
  T_VDev *vdev_;

  /// Virtual device CORBA object reference
  CORBA::Object_var vdev_obj_;

  /// Media controller
  T_MediaCtrl *media_ctrl_;

  // Media controller CORBA object reference
  CORBA::Object_var media_ctrl_obj_;

  /// pid of this process
  pid_t pid_;

  /// Name of the host.
  char host_[MAXHOSTNAMELEN];

  CORBA::ORB_ptr orb_;

  PortableServer::POA_ptr poa_;

};

// ----------------------------------------------------------------------
/**
 * @class TAO_AV_Child_Process_A
 * @brief Helper class for the child process created in TAO_AV_Child_Process
 */
template <class T_StreamEndpoint, class T_VDev , class T_MediaCtrl>
class TAO_AV_Export TAO_AV_Child_Process_A
  : public TAO_AV_Child_Process <T_StreamEndpoint, T_VDev, T_MediaCtrl>
{

public:
  /// Constructor.
  TAO_AV_Child_Process_A (void);

  /// Destructor.
  virtual ~TAO_AV_Child_Process_A (void);
};

// ----------------------------------------------------------------------

/**
 * @class TAO_AV_Child_Process_B
 * @brief Helper class for the child process created in TAO_AV_Child_Process
 */
template <class T_StreamEndpoint, class T_VDev , class T_MediaCtrl>
class TAO_AV_Export TAO_AV_Child_Process_B
  : public TAO_AV_Child_Process <T_StreamEndpoint, T_VDev, T_MediaCtrl>
{

public:
  /// Constructor.
  TAO_AV_Child_Process_B (void);

  /// Destructor.
  virtual ~TAO_AV_Child_Process_B (void);
};

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "Endpoint_Strategy_T.cpp"
#endif /*ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("Endpoint_Strategy_T.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#include /**/ "ace/post.h"
#endif /* TAO_AV_ENDPOINT_STRATEGY_T_H */
