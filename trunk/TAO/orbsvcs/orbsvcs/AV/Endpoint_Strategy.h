/* -*- C++ -*- */

// $Id$

// ============================================================================
//
// = LIBRARY
//    cos
// 
// = FILENAME
//   Endpoint_Strategy.h
//
// = AUTHOR
//    Sumedh Mungee <sumedh@cs.wustl.edu>
//    
// 
// ============================================================================

#if !defined (TAO_AV_ENDPOINT_STRATEGY_H)
#define	TAO_AV_ENDPOINT_STRATEGY_H

#include "orbsvcs/AV/AVStreams_i.h"
#include "tao/TAO.h"

class TAO_ORBSVCS_Export TAO_AV_Endpoint_Strategy
// = DESCRIPTION
//    Base class to define various endpoint strategies
//    used by the MMDevice to create the Endpoint and Vdev
{
public:
  TAO_AV_Endpoint_Strategy (void);
  // Constructor
  
  virtual int create_A (AVStreams::StreamEndPoint_A_ptr &stream_endpoint,
                        AVStreams::VDev_ptr &vdev,
                        CORBA::Environment &env);
  // Called by the MMDevice, when it needs to create an A type endpoint

  virtual int create_B (AVStreams::StreamEndPoint_B_ptr &stream_endpoint,
                        AVStreams::VDev_ptr &vdev,
                        CORBA::Environment &env);
  // Called by the MMDevice, when it needs to create an B type endpoint

 protected:
  AVStreams::StreamEndPoint_A_ptr stream_endpoint_a_;
  // The "A" stream endpoint

  AVStreams::StreamEndPoint_B_ptr stream_endpoint_b_;
  // The "B" stream endpoint

  AVStreams::VDev_ptr vdev_;
  // The vdev

};

// ----------------------------------------------------------------------

class TAO_ORBSVCS_Export TAO_AV_Endpoint_Process_Strategy
  : public virtual TAO_AV_Endpoint_Strategy
// = DESCRIPTION
//    Process-based strategy for creating endpoints
//    Abstract base class.
{
 public:
  TAO_AV_Endpoint_Process_Strategy (ACE_Process_Options *process_options);
  // Constructor. The process_options contain the name and arguments
  // for the process to be created

  virtual int activate (void);
  // creates a new child process, and waits on a semaphore
  // until the child process has finished creating the endpoints

 protected:
  virtual int bind_to_naming_service (CORBA::Environment &env);
  // Bind to the naming service

  virtual int get_stream_endpoint (CORBA::Environment &env) = 0;
  // Get the object reference for the newly created stream
  // endpoint (which will be in the child process)
  // Subclasses will define the functionality for this

  virtual int get_vdev (CORBA::Environment &env);
  // Get the Vdev object reference for the newly created
  // endpoint 

  CosNaming::NamingContext_var naming_context_;
  // Naming context

 private:
  ACE_Process_Options *process_options_;
  // Name and arguments for the process to be created
};  


// ----------------------------------------------------------------------

class TAO_ORBSVCS_Export TAO_AV_Endpoint_Process_Strategy_A
  : public virtual TAO_AV_Endpoint_Process_Strategy
// = DESCRIPTION
//    Process-based strategy to create "A" type endpoints
{
 public:
  TAO_AV_Endpoint_Process_Strategy_A (ACE_Process_Options *process_options);
  // Constructor

 protected:  
  virtual int create_A (AVStreams::StreamEndPoint_A_ptr &stream_endpoint,
                        AVStreams::VDev_ptr &vdev,
                        CORBA::Environment &env);
  // Creates an "A" type stream endpoint, and a vdev

  virtual int get_stream_endpoint (CORBA::Environment &env);
  // Gets the "A" type stream endpoint from the child process
  
};  

// ----------------------------------------------------------------------

class TAO_ORBSVCS_Export TAO_AV_Endpoint_Process_Strategy_B
  : public virtual TAO_AV_Endpoint_Process_Strategy
// = DESCRIPTION
//    Process-based strategy to create "B" type endpoints
{
 public:
  TAO_AV_Endpoint_Process_Strategy_B (ACE_Process_Options *process_options);
  // Constructor

 protected:
  virtual int create_B (AVStreams::StreamEndPoint_B_ptr &stream_endpoint,
                        AVStreams::VDev_ptr &vdev,
                        CORBA::Environment &env);
  // Creates a "B" type stream endpoint, and a vdev
  
  
  virtual int get_stream_endpoint (CORBA::Environment &env);
  
};  
// ----------------------------------------------------------------------
template <class T_StreamEndpoint, class T_VDev , class T_MediaCtrl>
class TAO_ORBSVCS_Export TAO_AV_Endpoint_Reactive_Strategy
  : public virtual TAO_AV_Endpoint_Strategy
// = DESCRIPTION
//    Reactive strategy
{
 protected:
  TAO_AV_Endpoint_Reactive_Strategy (TAO_ORB_Manager *orb_manager);
  // Constructor

  virtual int activate (void);
  
  virtual int activate_stream_endpoint (void);
  
  virtual int activate_vdev (void);

  virtual int activate_mediactrl (void);

  virtual int make_vdev (T_VDev *&vdev);
  // Bridge method to create a vdev, a la Acceptor. Applications
  // can override this

  TAO_ORB_Manager *orb_manager_;

};
// ----------------------------------------------------------------------

template <class T_StreamEndpoint, class T_VDev , class T_MediaCtrl>
class TAO_ORBSVCS_Export TAO_AV_Endpoint_Reactive_Strategy_A
  : public virtual TAO_AV_Endpoint_Reactive_Strategy<T_StreamEndpoint, T_VDev , T_MediaCtrl>
// = DESCRIPTION
//    Reactive strategy
{
 public:
  TAO_AV_Endpoint_Reactive_Strategy_A (TAO_ORB_Manager *orb_manager);

  virtual int activate_stream_endpoint (void);

  virtual int create_A (AVStreams::StreamEndPoint_A_ptr &stream_endpoint,
                        AVStreams::VDev_ptr &vdev,
                        CORBA::Environment &env);
  // Called by the MMDevice, when it needs to create an A type endpoint

  
};

// ----------------------------------------------------------------------
template <class T_StreamEndpoint, class T_Vdev , class T_MediaCtrl>
class TAO_ORBSVCS_Export TAO_AV_Endpoint_Reactive_Strategy_B
  : public virtual TAO_AV_Endpoint_Reactive_Strategy
// = DESCRIPTION
//    Reactive strategy
{
 public:
  TAO_AV_Endpoint_Reactive_Strategy_B (TAO_ORB_Manager *);

  virtual int activate_stream_endpoint (void);

  virtual int create_B (AVStreams::StreamEndPoint_B_ptr &stream_endpoint,
                        AVStreams::VDev_ptr &vdev,
                        CORBA::Environment &env);
};

// ----------------------------------------------------------------------

template <class T_StreamEndpoint, class T_VDev , class T_MediaCtrl>
class TAO_ORBSVCS_Export TAO_AV_Child_Process
// = DESCRIPTION
//    Helper class for the child process created in TAO_AV_Endpoint_Process_Strategy
{
public:
  TAO_AV_Child_Process ();
  int init (int argc, char **argv);
  int run (ACE_Time_Value *tv = 0);

 protected:
  const char *stream_endpoint_name_;

  int activate_objects (int argc, 
                        char **argv,
                        CORBA::Environment &env);

  int bind_to_naming_service (CORBA::Environment &env);
  int register_vdev (CORBA::Environment &env);
  int register_stream_endpoint (CORBA::Environment &env);
  int register_with_naming_service (CORBA::Environment &env);
  int release_semaphore ();

  TAO_ORB_Manager orb_manager_;
  // The ORB Manager

  CosNaming::NamingContext_var naming_context_;
  // The root Naming Context of the TAO naming service

  T_StreamEndpoint stream_endpoint_;
  // The stream endpoint member

  T_VDev vdev_;
  // The virtual device

  T_MediaCtrl media_control_;
  // Media controller
};

// ----------------------------------------------------------------------

template <class T_StreamEndpoint, class T_VDev , class T_MediaCtrl>
class TAO_ORBSVCS_Export TAO_AV_Child_Process_A
  : public virtual TAO_AV_Child_Process <T_StreamEndpoint, T_VDev, T_MediaCtrl>
// = DESCRIPTION
//    Helper class for the child process created in TAO_AV_Child_Process
{
public:
  TAO_AV_Child_Process_A ();

};

// ----------------------------------------------------------------------

template <class T_StreamEndpoint, class T_VDev , class T_MediaCtrl>
class TAO_ORBSVCS_Export TAO_AV_Child_Process_B
  : public virtual TAO_AV_Child_Process <T_StreamEndpoint, T_VDev, T_MediaCtrl>
// = DESCRIPTION
//    Helper class for the child process created in TAO_AV_Child_Process
{
public:
  TAO_AV_Child_Process_B ();

};

#endif /* TAO_AV_ENDPOINT_STRATEGY_H */
