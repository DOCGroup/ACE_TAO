/* -*- c++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    server
//
// = FILENAME
//    server.h
//
// = DESCRIPTION
//     This defines the Audio/Video Server using TAO'S Audio/Video streaming service.
//
// = AUTHORS
//    Sumedh Mungee (sumedh@cs.wustl.edu)
//    Nagarajan Surendran (naga@cs.wustl.edu)
//
// ============================================================================

#ifndef TAO_AV_SERVER_H
#define TAO_AV_SERVER_H

#include "ace/Get_Opt.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Acceptor.h"
#include "ace/Svc_Handler.h"
#include "ace/SOCK_Acceptor.h"
#include "ace/INET_Addr.h"
#include "ace/SOCK_CODgram.h"
#include "ace/Select_Reactor.h"

#include "include/common.h"
#include "mpeg_server/server_proto.h"
#include "mpeg_shared/fileio.h"
#include "mpeg_shared/routine.h"
#include "mpeg_shared/com.h"
#include "mpeg_server/Video_Control_i.h"
#include "orbsvcs/orbsvcs/Naming/Naming_Utils.h"

#if defined (NATIVE_ATM)
#include "atmcom.h"
#endif /* NATIVE_ATM */

#include "mpeg_server/Video_Server.h"
#include "mpeg_server/Audio_Server.h"
#include "orbsvcs/AV/AVStreams_i.h"
#include "orbsvcs/AV/Endpoint_Strategy.h"
#include "vs.h"

#include "Property_Exporter.h"
#include "Machine_Properties.h"
#include "Video_Repository.h"

#include "mpeg_shared/MMDevice_ExporterS.h"



class AV_Audio_MMDevice
  : public TAO_MMDevice
{
public:

  static const char* NUMBER_OF_CONNECTIONS;
  static const char* MAX_CONNECTIONS;
  static const char* SERVER_NAME;

  AV_Audio_MMDevice (TAO_AV_Endpoint_Process_Strategy *endpoint_strategy);
  // Default constructor

  virtual AVStreams::StreamEndPoint_B_ptr
  create_B (AVStreams::StreamCtrl_ptr the_requester,
            AVStreams::VDev_out the_vdev,
            AVStreams::streamQoS &the_qos,
            CORBA::Boolean_out met_qos,
            char *&named_vdev,
            const AVStreams::flowSpec &the_spec,
            CORBA::Environment &env);
  // Called by StreamCtrl to create a "B" type streamandpoint and vdev


};

class AV_Video_MMDevice
  : public TAO_MMDevice//,
  //    public TAO_Exportable
{
public:

    AV_Video_MMDevice (TAO_AV_Endpoint_Process_Strategy *endpoint_strategy);
  // Default constructor

  virtual AVStreams::StreamEndPoint_B_ptr
  create_B (AVStreams::StreamCtrl_ptr the_requester,
            AVStreams::VDev_out the_vdev,
            AVStreams::streamQoS &the_qos,
            CORBA::Boolean_out met_qos,
            char *&named_vdev,
            const AVStreams::flowSpec &the_spec,
            CORBA::Environment &env);
  // Called by StreamCtrl to create a "B" type streamandpoint and vdev


};

class MMDevice_Exporter_i
  : public POA_MMDevice_Exporter,
    public TAO_Exportable,
    public TAO_PropertySet<POA_CosPropertyService::PropertySet>
{
public:
  CORBA_Object_ptr audio_mmdevice_;
  CORBA_Object_ptr video_mmdevice_;

  static const char* NUMBER_OF_CONNECTIONS;
  static const char* MAX_CONNECTIONS;
  static const char* SERVER_NAME;

  MMDevice_Exporter_i();

  CORBA::ULong connections (void) const;
  // Retrieve the number of connections.

  //   virtual CORBA::Object_ptr audio_mmdevice_reference_ (CORBA_Environment &_env = CORBA_Environment::default_environment ()); 

  //   virtual CORBA::Object_ptr video_mmdevice_reference_ (CORBA_Environment &_env = CORBA_Environment::default_environment ()); 

  virtual CORBA::Object_ptr get_audio_mmdevice (CORBA_Environment &_env = CORBA_Environment::default_environment ());
  virtual CORBA::Object_ptr get_video_mmdevice (CORBA_Environment &_env = CORBA_Environment::default_environment ());

  virtual void export_properties (TAO_Property_Exporter& prop_exporter);

  virtual int define_properties (CosTradingRepos::ServiceTypeRepository::PropStructSeq& prop_seq,
                                 CORBA::ULong offset = 0) const;
private:
  
  CORBA::ULong connections_;
  // Number of active connections

  CORBA::ULong max_connections_;

};

class AV_Server;

class AV_Server_Sig_Handler
  : public virtual ACE_Event_Handler
{
public:
  AV_Server_Sig_Handler (const AV_Server& av_server);

  virtual ACE_HANDLE get_handle (void) const;

  int register_handler (void);
  // this will register this sig_handler
  // with the reactor for SIGCHLD,SIGTERM,SIGINT

  virtual int shutdown (ACE_HANDLE,
                        ACE_Reactor_Mask);

  virtual int handle_input (ACE_HANDLE);
  // handle input on the dummy handle.

  virtual int handle_signal (ACE_HANDLE signum,
                             siginfo_t * = 0,
                             ucontext_t* = 0);
  // handles the SIGCHLD,SIGTERM,SIGINT for the parent process i.e
  // the main thread..

  void int_handler (int sig);
  // Signal handler function for SIGTERM,SIGBUS,SIGINT

  int remove_names (void);
  // Removes the audio server mmdevice and video server mmdevice from the naming service.

  void clear_child (int sig);
  // Signal handler function for SIGCHLD

  ~AV_Server_Sig_Handler (void);
  // Destructor

private:
  const AV_Server& av_server_;

  ACE_HANDLE handle_;
  // dummy handle for the sig handler.
  ACE_Sig_Set sig_set;
};

class AV_Server
{
  // = TITLE
  //   Defines a class that abstracts the functionality of a
  //   video and audio server.
  //
  // = DESCRIPTION
  //   Using the class is as simple as calling init () first and then
  //   run. It uses an acceptor with the default ACE_Reactor::instance ().
public:

  static const char* MMDEVICE_SERVICE_TYPE;

  AV_Server (void);
  // constructor

  int init (int argc,
            char **argv,
            CORBA::Environment& env);
  // Initialize the AV_Server

  int run (CORBA::Environment& env);
  // Run the AV_Server

  void shutdown (void) const;

  static void on_exit_routine (void);
  // Routine called when this process exits.

  static pid_t current_pid_;
  // %% the pid the server is currently waiting on

  static int done_;
  // %% the done flag, set by the signal handler

  ~AV_Server (void);
  // Destructor
  //private:
private:
  int parse_args (int argcs,
                  char **argv);
  // Parse the arguments.

  void export_properties (CORBA::Environment& _env);

  int resolve_trader (CORBA::Environment& _env);

  TAO_ORB_Manager orb_manager_;
  // the TAO ORB manager.

  TAO_Machine_Properties mach_props_;
  // Dynamic properties concerning the server's performance.

  TAO_Video_Repository video_rep_;
  // Dynamic property that assesses the contents of the sever's video
  // repository.

  CosTrading::Lookup_var trader_;
  // Reference to the Lookup interface of the trading service.

  //  CosTrading::OfferId_var audio_offer_id_;
  // Server offer id.

  CosTrading::OfferId_var mmdevice_offer_id_;
  // Server offer id.

  CosTradingRepos::ServiceTypeRepository::PropStructSeq prop_seq_;
  // Service type definition.

  CosNaming::NamingContext_var naming_context_;
  // The root naming context of the naming service

  AV_Server_Sig_Handler signal_handler_;
  // Signal handler for SIGCHLD,SIGINT,SIGTERM,SIGBUS

  ACE_INET_Addr server_control_addr_;
  // Control (TCP) Address of this server.

  ACE_Process_Options video_process_options_;
  // The process options for the process to be spawned by the process strategy

  TAO_AV_Endpoint_Process_Strategy_B video_process_strategy_;
  // The proces strategy for the video.

  //  TAO_MMDevice *video_mmdevice_;
  // The video server multimedia device

  ACE_Process_Options audio_process_options_;
  // The process options for the process to be spawned by the process strategy

  TAO_AV_Endpoint_Process_Strategy_B audio_process_strategy_;
  // The proces strategy for the audio.

  //  TAO_MMDevice *audio_mmdevice_;
  AV_Audio_MMDevice *audio_mmdevice_;
  // The audio server multimedia device

  AV_Video_MMDevice *video_mmdevice_;
  // The video server multimedia device

  MMDevice_Exporter_i *mmdevice_exporter_;
};



typedef ACE_Singleton<AV_Server,ACE_Null_Mutex> AV_SERVER;

#endif /* TAO_AV_SERVER_H */
