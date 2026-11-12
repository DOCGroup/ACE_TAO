/* -*- C++ -*- */
#ifndef TAO_AV_MULTICAST_FULL_PROFILE_SERVER_H
#define TAO_AV_MULTICAST_FULL_PROFILE_SERVER_H

#include <cstdio>
#include <cstddef>
#include <memory>

#include "ace/Get_Opt.h"
#include "ace/OS_NS_stdio.h"
#include "ace/SString.h"
#include "orbsvcs/Naming/Naming_Client.h"
#include "orbsvcs/AV/AVStreams_i.h"
#include "orbsvcs/AV/Endpoint_Strategy.h"
#include "orbsvcs/AV/Flows_T.h"
#include "orbsvcs/AV/sfp.h"
#include "orbsvcs/AV/MCast.h"
#include "orbsvcs/AV/Policy.h"

/// Receives full-profile AVStreams frames and writes them to a file.
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
                     TAO_AV_frame_info *frame_info,
                     ACE_Addr const &) override;
  int handle_end_stream ();
};

/// Flow consumer that creates the server callback.
class FTP_Server_FlowEndPoint
  : public TAO_FlowConsumer
{
public:
  FTP_Server_FlowEndPoint ();
  FTP_Server_FlowEndPoint (FTP_Server_FlowEndPoint const &) = delete;
  FTP_Server_FlowEndPoint &operator= (FTP_Server_FlowEndPoint const &) = delete;
  FTP_Server_FlowEndPoint (FTP_Server_FlowEndPoint &&) = delete;
  FTP_Server_FlowEndPoint &operator= (FTP_Server_FlowEndPoint &&) = delete;
  ~FTP_Server_FlowEndPoint () override;
  int get_callback (char const *flowname,
                    TAO_AV_Callback *&callback) override;

private:
  /// Callback whose lifetime is owned by this endpoint.
  std::unique_ptr<FTP_Server_Callback> callback_ {};
};

using FTP_Server_FDev = TAO_FDev<TAO_FlowProducer, FTP_Server_FlowEndPoint>;

/// Server for the full-profile multicast file transfer test.
class Server
{
public:
  Server ();
  Server (Server const &) = delete;
  Server &operator= (Server const &) = delete;
  Server (Server &&) = delete;
  Server &operator= (Server &&) = delete;
  /// Initialize the server, output file, and Naming Service bindings.
  int init (int argc,
            ACE_TCHAR *argv[]);
  /// Run the ORB event loop.
  int run ();
  /// Return the output file owned by this server.
  FILE *file ();
  /// Close the output file.
  void close_file ();
  /// Return the protocols supported by the server.
  AVStreams::protocolSpec protocols ();
  /// Return the format identifier used by this flow.
  char const *format ();

protected:
  struct Options
  {
    ACE_TString filename {ACE_TEXT ("output")};
  };

  using File_Ptr = std::unique_ptr<FILE, int (*) (FILE *)>;

  /// Parse the command-line options.
  int parse_args (int argc, ACE_TCHAR *argv[]);
  /// Server command-line options.
  Options options_;
  /// Naming Service client.
  TAO_Naming_Client my_naming_client_;
  /// Endpoint strategy for incoming flows.
  TAO_AV_Endpoint_Reactive_Strategy_B<TAO_StreamEndPoint_B,
                                      TAO_VDev,
                                      AV_Null_MediaCtrl> reactive_strategy_;
  /// Output file owned by this server.
  File_Ptr fp_ {nullptr, std::fclose};
};

using FTP_SERVER = ACE_Unmanaged_Singleton<Server, ACE_Null_Mutex>;

#endif /* TAO_AV_MULTICAST_FULL_PROFILE_SERVER_H */
