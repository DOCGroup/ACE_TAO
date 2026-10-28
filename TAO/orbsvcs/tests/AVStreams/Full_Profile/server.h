/* -*- C++ -*- */
#include "ace/Get_Opt.h"
#include "orbsvcs/Naming/Naming_Client.h"
#include "orbsvcs/AV/AVStreams_i.h"
#include "orbsvcs/AV/Flows_T.h"
#include "orbsvcs/AV/Endpoint_Strategy.h"
#include "orbsvcs/AV/Transport.h"
#include "orbsvcs/AV/sfp.h"
#include "orbsvcs/AV/MCast.h"
#include "orbsvcs/AV/Policy.h"

class Server;

class FTP_Server_Callback
  :public TAO_AV_Callback
{
public:
  FTP_Server_Callback ();
  int handle_stop () override;
  int receive_frame (ACE_Message_Block *frame,
                     TAO_AV_frame_info *,
                     ACE_Addr const &) override;
  int handle_end_stream ();
};

class FTP_Server_FlowEndPoint
  :public TAO_FlowConsumer
{
public:
  FTP_Server_FlowEndPoint ();
  int get_callback (const char *flowname,
                    TAO_AV_Callback *&callback) override;
};

class Server
{
public:
  Server ();
  Server (Server const &) = delete;
  Server &operator= (Server const &) = delete;
  Server (Server &&) = delete;
  Server &operator= (Server &&) = delete;
  int init (int argc,
            ACE_TCHAR *argv[]);
  int run ();
  FILE *file ();
  AVStreams::protocolSpec protocols ();
  CORBA::ORB_ptr orb ();
  void orb (CORBA::ORB_ptr orb_in);
  PortableServer::POA_ptr poa ();
  void poa (PortableServer::POA_ptr poa_in);
  char const *format ();
protected:
  int parse_args (int argc, ACE_TCHAR *argv[]);
  TAO_Naming_Client my_naming_client_;
  TAO_AV_Endpoint_Reactive_Strategy_B <TAO_StreamEndPoint_B,TAO_VDev,AV_Null_MediaCtrl> reactive_strategy_;
  FILE *fp_;
  char *protocol_;
  CORBA::ORB_var orb_;
  PortableServer::POA_ptr poa_;
  TAO_StreamEndPoint_B *streamendpoint_b_;
  AVStreams::StreamEndPoint_B_var sep_b_;
  FTP_Server_FlowEndPoint *fep_b_;
  AVStreams::FlowConsumer_var fep_b_obj_;
};

using FTP_SERVER = ACE_Unmanaged_Singleton<Server, ACE_Null_Mutex>;
