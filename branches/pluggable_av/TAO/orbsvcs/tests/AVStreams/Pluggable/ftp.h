/* -*- C++ -*- */
// $Id$

#ifndef TAO_AV_FTP_H
#define TAO_AV_FTP_H

#include "ace/Get_Opt.h"
#include "tao/TAO.h"
#include "orbsvcs/Naming/Naming_Utils.h"
#include "orbsvcs/AV/AVStreams_i.h"
#include "orbsvcs/AV/Endpoint_Strategy.h"
#include "orbsvcs/AV/Transport.h"
#include "orbsvcs/AV/sfp.h"

class FTP_Client_Flow_Handler
  :public virtual TAO_AV_Flow_Handler
{
public:
  FTP_Client_Flow_Handler (TAO_ORB_Manager *orb_manager);
  virtual int handle_timeout (const ACE_Time_Value &tv,
                              const void *arg = 0);
protected:
  TAO_ORB_Manager *orb_manager_;
  long timer_id_;
  int count_;
};

class FTP_Client_UDP_Flow_Handler
:public FTP_Client_Flow_Handler,
 public TAO_AV_UDP_Flow_Handler
{
public:
  FTP_Client_UDP_Flow_Handler (TAO_ORB_Manager *orb_manager = 0);
  virtual int start (void);
  virtual int handle_timeout (const ACE_Time_Value &tv,
                              const void *arg = 0);
};

class FTP_Client_TCP_Flow_Handler
:public FTP_Client_Flow_Handler,
 public TAO_AV_TCP_Flow_Handler
{
public:
  FTP_Client_TCP_Flow_Handler (TAO_ORB_Manager *orb_manager = 0);
  virtual int start (void);
  virtual int handle_timeout (const ACE_Time_Value &tv,
                              const void *arg = 0);
};

class FTP_Client_SFP_Flow_Handler
  :public FTP_Client_Flow_Handler,
   public ACE_Event_Handler
{
public:
  FTP_Client_SFP_Flow_Handler (TAO_ORB_Manager *orb_manager);
  virtual int start (void);
  virtual int handle_timeout (const ACE_Time_Value &tv,
                              const void *arg = 0);
  virtual TAO_AV_Transport *transport (void);
  virtual void set_sfp_object (TAO_SFP_Object *object);
protected:
  TAO_SFP_Object *sfp_object_;
};

class FTP_SFP_Object
  :public TAO_SFP_Object
{
public:
  FTP_SFP_Object (FTP_Client_SFP_Flow_Handler *handler);
  virtual int start (void);
protected:
  FTP_Client_SFP_Flow_Handler *handler_;
};

class FTP_Client_StreamEndPoint
  :public TAO_Client_StreamEndPoint
{
public:
  FTP_Client_StreamEndPoint (TAO_ORB_Manager *orb_manager = 0);

  virtual int make_udp_flow_handler (TAO_AV_UDP_Flow_Handler *&handler);
  // call to make a new flow handler for a dgram flow.

  virtual int make_tcp_flow_handler (TAO_AV_TCP_Flow_Handler *&handler);
  // call to make a new flow handler for a dgram flow.
  virtual int get_sfp_object (const char *flowname,
                              TAO_SFP_Object *&sfp_object);
protected:
  TAO_ORB_Manager *orb_manager_;
};

typedef TAO_AV_Endpoint_Reactive_Strategy_A<FTP_Client_StreamEndPoint,TAO_VDev,AV_Null_MediaCtrl> ENDPOINT_STRATEGY;

class Client;
class Endpoint_Reactive_Strategy
  : public ENDPOINT_STRATEGY
{
public:
  Endpoint_Reactive_Strategy (TAO_ORB_Manager *orb_manager,
                              Client *client_ptr);
  // constructor . The orb manager is needed for the TAO_AV_Endpoint_Reactive_Strategy_A.

  virtual int make_stream_endpoint (FTP_Client_StreamEndPoint *& endpoint);
  // hook to make our streamendpoint taking a Client pointer
private:
  Client *client_;
  // pointer to command handler object
  TAO_ORB_Manager *orb_manager_;
};


class Client
{
public:
  Client (void);
  int init (int argc, char **argv);
  int run (void);
  FILE *file (void);
  TAO_StreamCtrl* streamctrl (void);
private:
  int parse_args (int argc, char **argv);
  int bind_to_server (void);
  TAO_ORB_Manager *orb_manager_;
  Endpoint_Reactive_Strategy endpoint_strategy_;
  AVStreams::MMDevice_var server_mmdevice_;
  TAO_MMDevice client_mmdevice_;
  TAO_StreamCtrl streamctrl_;
  // Video stream controller

  int argc_;
  char **argv_;
  const char *filename_;
  const char *address_;

  TAO_Naming_Client my_naming_client_;
  FILE *fp_;
  char *protocol_;
  int use_sfp_;
};

typedef ACE_Singleton<Client,ACE_Null_Mutex> CLIENT;

#endif /* TAO_AV_FTP_H */
