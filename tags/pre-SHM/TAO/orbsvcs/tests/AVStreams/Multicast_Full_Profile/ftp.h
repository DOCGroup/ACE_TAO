/* -*- C++ -*- */
// $Id$

#ifndef TAO_AV_FTP_H
#define TAO_AV_FTP_H

#include "ace/Get_Opt.h"
#include "tao/TAO.h"
#include "orbsvcs/Naming/Naming_Utils.h"
#include "orbsvcs/AV/AVStreams_i.h"
#include "orbsvcs/AV/Flows_T.h"
#include "orbsvcs/AV/Endpoint_Strategy.h"
#include "orbsvcs/AV/sfp.h"
#include "orbsvcs/AV/MCast.h"

// class FTP_Client_Flow_Handler
// //  :public virtual ACE_Event_Handler,
//    public virtual TAO_FlowProducer
// {
// public:
//   FTP_Client_Flow_Handler (TAO_ORB_Manager *orb_manager,
//                            ACE_Time_Value &timeout);
//   virtual int handle_timeout (const ACE_Time_Value &tv,
//                               const void *arg = 0);
//   virtual int start (void);
//   virtual int stop (void);
//   virtual int get_callback (const char *flowname,
//                             TAO_AV_Callback *&callback);
// protected:
//   TAO_ORB_Manager *orb_manager_;
//   long timer_id_;
//   int count_;
//   ACE_Time_Value timeout_;
// };

// class FTP_Client_Callback
//   :public TAO_AV_Callback
// {
// public:
//   FTP_Client_Callback (FTP_Client_Flow_Handler *handler);
//   virtual int handle_start (void);
//   virtual int handle_stop (void);
// protected:
//   FTP_Client_Flow_Handler *handler_;
// };

class FTP_Client_Callback;
class FTP_Client_Producer
  :public virtual TAO_FlowProducer
{
public:
  FTP_Client_Producer (void);
  virtual int get_callback (const char *flowname,
                            TAO_AV_Callback *&callback);
  int set_protocol_object (const char *flowname,
                           TAO_AV_Protocol_Object *object);
protected:
  FTP_Client_Callback *callback_;
};

class FTP_Client_Callback
  :public TAO_AV_Callback
{
public:
  FTP_Client_Callback (void);
  virtual int handle_timeout (void *arg);
  virtual int handle_end_stream (void);
  virtual void get_timeout (ACE_Time_Value *&tv,
                            void *&arg);
  //  virtual int get_frame (ACE_Message_Block *&block,TAO_AV_frame_info *&frame_info);
  void set_protocol_object (TAO_AV_Protocol_Object *protocol_object) {this->protocol_object_ = protocol_object;}

protected:
  //  FTP_Client_Flow_Handler *handler_;
  int count_;
  TAO_AV_Protocol_Object *protocol_object_;
};

// class FTP_Client_FDev
//   :public TAO_FDev
// {
// public:
//   FTP_Client_FDev (TAO_ORB_Manager *orb_manger);
//   virtual AVStreams::FlowProducer_ptr make_producer (AVStreams::FlowConnection_ptr the_requester,
//                                                      AVStreams::QoS & the_qos,
//                                                      CORBA::Boolean_out met_qos,
//                                                      char *& named_fdev,
//                                                      CORBA::Environment &env = CORBA::Environment::default_environment ());
// protected:
//   TAO_ORB_Manager *orb_manager_;
// };

typedef TAO_AV_Endpoint_Reactive_Strategy_A<TAO_StreamEndPoint_A,TAO_VDev,AV_Null_MediaCtrl> ENDPOINT_STRATEGY;

typedef TAO_FDev <FTP_Client_Producer,TAO_FlowConsumer> FTP_Client_FDev;
class Client
{
public:
  Client (void);
  int init (int argc, char **argv);
  int run (void);
  FILE *file (void);
  char *flowname (void);
  TAO_StreamCtrl* streamctrl (void);
  AVStreams::protocolSpec protocols (void);
  const char *format (void);
  const char *address (void);
private:
  int parse_args (int argc, char **argv);
  int bind_to_server (const char *name);
  TAO_ORB_Manager *orb_manager_;
  ENDPOINT_STRATEGY endpoint_strategy_;
  AVStreams::MMDevice_var server_mmdevice_;
  TAO_MMDevice client_mmdevice_;
  FTP_Client_FDev *fdev_;
  TAO_StreamCtrl streamctrl_;
  // Video stream controller

  int argc_;
  char **argv_;
  const char *filename_;
  const char *address_;

  TAO_Naming_Client my_naming_client_;
  FILE *fp_;
  char *protocol_;
  char *flowname_;
  int use_sfp_;
};

typedef ACE_Singleton<Client,ACE_Null_Mutex> CLIENT;

#endif /* TAO_AV_FTP_H */
