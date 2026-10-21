/* -*- C++ -*- */
#ifndef TAO_AV_FTP_H
#define TAO_AV_FTP_H

#include "ace/Get_Opt.h"
#include "ace/High_Res_Timer.h"
#include "orbsvcs/AV/AVStreams_i.h"
#include "orbsvcs/AV/Flows_T.h"
#include "orbsvcs/AV/Endpoint_Strategy.h"
#include "orbsvcs/AV/Transport.h"
#include "orbsvcs/AV/sfp.h"
#include "orbsvcs/AV/MCast.h"

class Client;

class FTP_Client_Callback;

class FTP_Client_Producer
  :public virtual TAO_FlowProducer
{
public:
  FTP_Client_Producer ();
  int get_callback (const char *flowname,
                    TAO_AV_Callback *&callback) override;
  int set_protocol_object (const char *flowname,
                           TAO_AV_Protocol_Object *object) override;
protected:
  FTP_Client_Callback *callback_;
};

class FTP_Client_Callback
  :public TAO_AV_Callback
{
public:
  FTP_Client_Callback ();
  int handle_timeout (void *arg) override;
  int handle_end_stream ();
  void get_timeout (ACE_Time_Value *&tv,
                    void *&arg) override;
  void set_protocol_object (TAO_AV_Protocol_Object *protocol_object) {this->protocol_object_ = protocol_object;}

protected:
  int count_;
  TAO_AV_Protocol_Object *protocol_object_;
};

using ENDPOINT_STRATEGY = TAO_AV_Endpoint_Reactive_Strategy_A<TAO_StreamEndPoint_A, TAO_VDev, AV_Null_MediaCtrl>;

class Client
{
public:
  Client ();
  Client (Client const &) = delete;
  Client &operator= (Client const &) = delete;
  Client (Client &&) = delete;
  Client &operator= (Client &&) = delete;
  int init (int argc, ACE_TCHAR *argv[]);
  int run ();
  FILE *file ();
  char const *flowname ();
  TAO_StreamCtrl *streamctrl ();
  AVStreams::protocolSpec protocols ();
  char const *format ();
  char const *address ();
private:
  int parse_args (int argc, ACE_TCHAR *argv[]);
  ENDPOINT_STRATEGY endpoint_strategy_;
  TAO_StreamCtrl streamctrl_;
  // Video stream controller

  int argc_;
  ACE_TCHAR **argv_;
  const char *filename_;
  ACE_CString address_;
  ACE_CString peer_addr_;

  FILE *fp_;
  char *protocol_;
  ACE_CString flowname_;
  int use_sfp_;
  CORBA::ORB_var orb_;
  PortableServer::POA_ptr poa_;
  TAO_StreamEndPoint_A *streamendpoint_a_;
  AVStreams::StreamEndPoint_A_var sep_a_;
  FTP_Client_Producer *fep_a_;
  AVStreams::FlowProducer_var fep_a_obj_;
};

using CLIENT = ACE_Unmanaged_Singleton<Client, ACE_Null_Mutex>;

#endif /* TAO_AV_FTP_H */
