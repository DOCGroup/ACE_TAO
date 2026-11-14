/* -*- C++ -*- */

#ifndef TAO_AV_FULL_PROFILE_SERVER_H
#define TAO_AV_FULL_PROFILE_SERVER_H

#include <memory>
#include <cstdio>

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
};

class FTP_Server_FlowEndPoint
  : public TAO_FlowConsumer
{
public:
  FTP_Server_FlowEndPoint ();
  FTP_Server_FlowEndPoint (FTP_Server_FlowEndPoint const &) = delete;
  FTP_Server_FlowEndPoint &operator= (FTP_Server_FlowEndPoint const &) = delete;
  FTP_Server_FlowEndPoint (FTP_Server_FlowEndPoint &&) = delete;
  FTP_Server_FlowEndPoint &operator= (FTP_Server_FlowEndPoint &&) = delete;
  int get_callback (char const *flowname,
                    TAO_AV_Callback *&callback) override;

private:
  FTP_Server_Callback callback_;
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
  /// Borrowed file handle, owned by this server.
  FILE *file () const;
  /// Close the output file after AVStreams stops receiving frames.
  void close_file ();
  AVStreams::protocolSpec protocols () const;
  char const *format () const;

protected:
  int parse_args (int argc, ACE_TCHAR *argv[]);
  TAO_Naming_Client my_naming_client_;
  TAO_AV_Endpoint_Reactive_Strategy_B<TAO_StreamEndPoint_B, TAO_VDev, AV_Null_MediaCtrl> reactive_strategy_;
  std::unique_ptr<FILE, int (*) (FILE *)> file_ {nullptr, std::fclose};
  CORBA::ORB_var orb_;
  PortableServer::POA_var poa_;
  AVStreams::StreamEndPoint_B_var sep_b_;
  AVStreams::FlowConsumer_var fep_b_obj_;
};

using FTP_SERVER = ACE_Unmanaged_Singleton<Server, ACE_Null_Mutex>;

#endif /* TAO_AV_FULL_PROFILE_SERVER_H */
