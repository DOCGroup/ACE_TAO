/* -*- C++ -*- */
// $Id$

#include "ace/Get_Opt.h"
#include "tao/TAO.h"
#include "orbsvcs/Naming/Naming_Utils.h"
#include "orbsvcs/AV/AVStreams_i.h"
#include "orbsvcs/AV/Endpoint_Strategy.h"
#include "orbsvcs/AV/Transport.h"
#include "orbsvcs/AV/sfp.h"
#include "orbsvcs/AV/MCast.h"

class FTP_Server_StreamEndPoint
  :public TAO_Server_StreamEndPoint
{
public:
  FTP_Server_StreamEndPoint (void);
  virtual int make_udp_flow_handler (TAO_AV_UDP_Flow_Handler *&handler);
  // call to make a new flow handler for a dgram flow.
  virtual int make_tcp_flow_handler (TAO_AV_TCP_Flow_Handler *&handler);
  // call to make a new flow handler for a dgram flow.
  virtual int make_dgram_mcast_flow_handler (TAO_AV_UDP_MCast_Flow_Handler *&handler);
  // call to make a new flow handler for a mcast dgram flow.

  virtual int get_sfp_callback (const char *flowname,
                                TAO_SFP_Callback *&callback);
};

class FTP_Server_Flow_Handler
  :public virtual TAO_AV_Flow_Handler
{
public:
  FTP_Server_Flow_Handler (void);
  virtual int input (ACE_HANDLE fd);
  virtual int start (void);
  virtual int stop (void);
};

class FTP_Server_UDP_Flow_Handler
  :public TAO_AV_UDP_Flow_Handler,
   public FTP_Server_Flow_Handler
{
public:
  virtual int handle_input (ACE_HANDLE fd);
};

class FTP_Server_TCP_Flow_Handler
  :public TAO_AV_TCP_Flow_Handler,
   public FTP_Server_Flow_Handler
{
public:
  virtual int start (void);
  virtual int handle_input (ACE_HANDLE fd);
  virtual int get_handle (void) const;
};

class FTP_Server_UDP_MCast_Flow_Handler
  :public TAO_AV_UDP_MCast_Flow_Handler,
   public FTP_Server_Flow_Handler
{
public:
  virtual int handle_input (ACE_HANDLE fd);
};

class FTP_SFP_Callback
  :public TAO_SFP_Callback
{
  virtual int start_failed (void);
  // This is called for both active and passive start.

  virtual int stream_established (void);
  // This is a callback for both active and passive stream
  // establshment.

  virtual int receive_frame (ACE_Message_Block *frame);
  // upcall to the application to receive a frame.

  virtual void end_stream (void);
  // called when the EndofStream message is received.
};

class Server
{
public:
  Server (void);
  int init (int argc,
            char **argv);
  int run (void);
  FILE *file (void);
protected:
  int parse_args (int argc,char **argv);
  TAO_ORB_Manager *orb_manager_;
  TAO_Naming_Client my_naming_client_;
  TAO_AV_Endpoint_Reactive_Strategy_B <FTP_Server_StreamEndPoint,TAO_VDev,AV_Null_MediaCtrl> reactive_strategy_;
  TAO_MMDevice *mmdevice_;
  FILE *fp_;
  char *protocol_;
};

typedef ACE_Singleton<Server,ACE_Null_Mutex> SERVER;
