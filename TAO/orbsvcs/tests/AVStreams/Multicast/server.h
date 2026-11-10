/* -*- C++ -*- */
#ifndef TAO_AV_MULTICAST_SERVER_H
#define TAO_AV_MULTICAST_SERVER_H

#include <cstdio>
#include <cstddef>
#include <memory>

#include "ace/Get_Opt.h"
#include "ace/OS_NS_stdio.h"
#include "ace/SString.h"
#include "tao/Strategies/advanced_resource.h"
#include "orbsvcs/CosNamingC.h"
#include "orbsvcs/Naming/Naming_Client.h"
#include "orbsvcs/AV/AVStreams_i.h"
#include "orbsvcs/AV/Endpoint_Strategy.h"
#include "orbsvcs/AV/Transport.h"
#include "orbsvcs/AV/sfp.h"
#include "orbsvcs/AV/MCast.h"
#include "orbsvcs/AV/Policy.h"

class FTP_Server_Callback;

class FTP_Server_StreamEndPoint
  : public TAO_Server_StreamEndPoint
{
public:
  FTP_Server_StreamEndPoint () = default;
  FTP_Server_StreamEndPoint (FTP_Server_StreamEndPoint const &) = delete;
  FTP_Server_StreamEndPoint &operator= (FTP_Server_StreamEndPoint const &) = delete;
  FTP_Server_StreamEndPoint (FTP_Server_StreamEndPoint &&) = delete;
  FTP_Server_StreamEndPoint &operator= (FTP_Server_StreamEndPoint &&) = delete;
  ~FTP_Server_StreamEndPoint ();
  int get_callback (char const *flowname,
                    TAO_AV_Callback *&callback) override;

private:
  std::unique_ptr<FTP_Server_Callback> callback_ {};
};

class FTP_Server_Callback
  : public TAO_AV_Callback
{
public:
  FTP_Server_Callback () = default;
  FTP_Server_Callback (FTP_Server_Callback const &) = delete;
  FTP_Server_Callback &operator= (FTP_Server_Callback const &) = delete;
  FTP_Server_Callback (FTP_Server_Callback &&) = delete;
  FTP_Server_Callback &operator= (FTP_Server_Callback &&) = delete;

  int handle_stop () override;
  int receive_frame (ACE_Message_Block *frame,
                     TAO_AV_frame_info *,
                     ACE_Addr const &) override;
  int handle_end_stream ();
};

class Server
{
public:
  Server () = default;
  Server (Server const &) = delete;
  Server &operator= (Server const &) = delete;
  Server (Server &&) = delete;
  Server &operator= (Server &&) = delete;
  int init (int argc,
            ACE_TCHAR *argv[]);
  int run ();
  std::size_t write_output (char const *buffer, std::size_t length);
  void close_file ();

protected:
  struct Options
  {
    ACE_TString filename {ACE_TEXT ("output")};
  };

  using File_Ptr = std::unique_ptr<FILE, int (*) (FILE *)>;

  int parse_args (int argc, ACE_TCHAR *argv[]);
  Options options_;
  TAO_Naming_Client my_naming_client_;
  TAO_AV_Endpoint_Reactive_Strategy_B<FTP_Server_StreamEndPoint,
                                      TAO_VDev,
                                      AV_Null_MediaCtrl> reactive_strategy_;
  File_Ptr fp_ {nullptr, ACE_OS::fclose};
};

using FTP_SERVER = ACE_Unmanaged_Singleton<Server, ACE_Null_Mutex>;

#endif /* TAO_AV_MULTICAST_SERVER_H */
