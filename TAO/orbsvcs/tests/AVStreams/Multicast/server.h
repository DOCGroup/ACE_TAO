/* -*- C++ -*- */
#include "ace/Get_Opt.h"
#include "tao/Strategies/advanced_resource.h"
#include "orbsvcs/CosNamingC.h"
#include "orbsvcs/Naming/Naming_Client.h"
#include "orbsvcs/AV/AVStreams_i.h"
#include "orbsvcs/AV/Endpoint_Strategy.h"
#include "orbsvcs/AV/Transport.h"
#include "orbsvcs/AV/sfp.h"
#include "orbsvcs/AV/MCast.h"
#include "orbsvcs/AV/Policy.h"

class FTP_Server_StreamEndPoint
  :public TAO_Server_StreamEndPoint
{
public:
  FTP_Server_StreamEndPoint ();
  int get_callback (const char *flowname,
                    TAO_AV_Callback *&callback) override;
};

class FTP_Server_Callback
  :public TAO_AV_Callback
{
public:
  int handle_stop () override;
  int receive_frame (ACE_Message_Block *frame,
                     TAO_AV_frame_info *,
                     ACE_Addr const &) override;
  int handle_end_stream ();
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
protected:
  int parse_args (int argc, ACE_TCHAR *argv[]);
  TAO_Naming_Client my_naming_client_;
  TAO_AV_Endpoint_Reactive_Strategy_B <FTP_Server_StreamEndPoint,TAO_VDev,AV_Null_MediaCtrl> reactive_strategy_;
  TAO_MMDevice *mmdevice_;
  FILE *fp_;
  char *protocol_;
};

using FTP_SERVER = ACE_Unmanaged_Singleton<Server, ACE_Null_Mutex>;
