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
//    Base class to define various strategies
//    used by the MMDevice to create the Endpoint and Vdev
{
public:
  TAO_AV_Endpoint_Strategy (void);
  
  virtual int create_A (AVStreams::StreamEndPoint_A_ptr &stream_endpoint,
                        AVStreams::VDev_ptr &vdev,
                        CORBA::Environment &env);

  virtual int create_B (AVStreams::StreamEndPoint_B_ptr &stream_endpoint,
                        AVStreams::VDev_ptr &vdev,
                        CORBA::Environment &env);

 protected:
  //  TAO_AV_Endpoint_Factory *endpoint_factory_;
  AVStreams::StreamEndPoint_A_ptr stream_endpoint_a_;
  AVStreams::StreamEndPoint_B_ptr stream_endpoint_b_;
  AVStreams::VDev_ptr vdev_;
};

// ----------------------------------------------------------------------

class TAO_ORBSVCS_Export TAO_AV_Endpoint_Process_Strategy
  : public virtual TAO_AV_Endpoint_Strategy
// = DESCRIPTION
//    Process-based strategy
{
 public:
  TAO_AV_Endpoint_Process_Strategy (ACE_Process_Options *process_options);
  virtual int activate (void);

 protected:
  virtual int bind_to_naming_service (CORBA::Environment &env);
  virtual int get_stream_endpoint (CORBA::Environment &env) = 0;
  virtual int get_vdev (CORBA::Environment &env);
  CosNaming::NamingContext_var naming_context_;

 private:
  ACE_Process_Options *process_options_;

};  


// ----------------------------------------------------------------------

class TAO_ORBSVCS_Export TAO_AV_Endpoint_Process_Strategy_A
  : public virtual TAO_AV_Endpoint_Process_Strategy
// = DESCRIPTION
//    Process-based strategy
{
 public:
  TAO_AV_Endpoint_Process_Strategy_A (ACE_Process_Options *process_options);

 protected:  
  virtual int create_A (AVStreams::StreamEndPoint_A_ptr &stream_endpoint,
                        AVStreams::VDev_ptr &vdev,
                        CORBA::Environment &env);

  virtual int get_stream_endpoint (CORBA::Environment &env);
  
};  

// ----------------------------------------------------------------------

class TAO_ORBSVCS_Export TAO_AV_Endpoint_Process_Strategy_B
  : public virtual TAO_AV_Endpoint_Process_Strategy
// = DESCRIPTION
//    Process-based strategy
{
 public:
  TAO_AV_Endpoint_Process_Strategy_B (ACE_Process_Options *process_options);

 protected:
  virtual int create_B (AVStreams::StreamEndPoint_B_ptr &stream_endpoint,
                        AVStreams::VDev_ptr &vdev,
                        CORBA::Environment &env);
  
  virtual int get_stream_endpoint (CORBA::Environment &env);
  
};  

// ----------------------------------------------------------------------

template <class T_StreamEndpoint, class T_VDev , class T_MediaCtrl>
class TAO_ORBSVCS_Export TAO_AV_Endpoint_Reactive_Strategy_A
  : public virtual TAO_AV_Endpoint_Strategy
// = DESCRIPTION
//    Reactive strategy
{
 public:
  TAO_AV_Endpoint_Reactive_Strategy_A (TAO_ORB_Manager *orb_manager);

  virtual int create_A (void);
  
 private:
  TAO_ORB_Manager *orb_manager_;

};

// ----------------------------------------------------------------------
template <class T_StreamEndpoint, class T_Vdev , class T_MediaCtrl>
class TAO_ORBSVCS_Export TAO_AV_Endpoint_Reactive_Strategy_B
  : public virtual TAO_AV_Endpoint_Strategy
// = DESCRIPTION
//    Reactive strategy
{
 public:
  TAO_AV_Endpoint_Reactive_Strategy_B (TAO_ORB_Manager *);

  virtual int create_B (void);
  
 private:
  TAO_ORB_Manager *orb_manager_;

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
  int run (ACE_Time_Value &tv = 0);

 protected:
  char *stream_endpoint_name_;

  int activate_objects (int argc, 
                        char **argv,
                        CORBA::Environment &env);

  int bind_to_naming_service (CORBA::Environment &env);
  int register_vdev (CORBA::Environment &env);
  int register_stream_endpoint (CORBA::Environment &env);
  int register_with_naming_service (CORBA::Environment &env);
  int release_semaphore ();

  TAO_ORB_Manager orb_manager_;
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
