/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    server.h
 *
 *  Ftp server to receive data
 *
 *  @author Yamuna Krishnamurthy <yamuna@cs.wustl.edu>
 */
//=============================================================================

#ifndef TAO_AV_FTP_SERVER_H
#define TAO_AV_FTP_SERVER_H

#include <cstdio>
#include <memory>

#include "ace/Get_Opt.h"
#include "ace/OS_NS_stdio.h"
#include "ace/SString.h"
#include "ace/Singleton.h"
#include "orbsvcs/Naming/Naming_Client.h"
#include "orbsvcs/AV/AVStreams_i.h"
#include "orbsvcs/AV/Endpoint_Strategy.h"
#include "orbsvcs/AV/Policy.h"

using FTP_Server_File_Ptr = std::unique_ptr<FILE, int (*) (FILE *)>;

class Server;
using FTP_SERVER = ACE_Unmanaged_Singleton<Server, ACE_Null_Mutex>;

/** Server callback receiving AVStreams frames. */
class FTP_Server_Callback : public TAO_AV_Callback
{
public:
  FTP_Server_Callback () = default;
  FTP_Server_Callback (FTP_Server_Callback const &) = delete;
  FTP_Server_Callback &operator= (FTP_Server_Callback const &) = delete;
  FTP_Server_Callback (FTP_Server_Callback &&) = delete;
  FTP_Server_Callback &operator= (FTP_Server_Callback &&) = delete;

  int receive_frame (ACE_Message_Block *frame,
                     TAO_AV_frame_info *frame_info,
                     ACE_Addr const &peer_address) override;
  int handle_destroy () override;
};

/** Server stream endpoint providing the application callback. */
class FTP_Server_StreamEndPoint : public TAO_Server_StreamEndPoint
{
public:
  FTP_Server_StreamEndPoint () = default;
  FTP_Server_StreamEndPoint (FTP_Server_StreamEndPoint const &) = delete;
  FTP_Server_StreamEndPoint &operator= (FTP_Server_StreamEndPoint const &) = delete;
  FTP_Server_StreamEndPoint (FTP_Server_StreamEndPoint &&) = delete;
  FTP_Server_StreamEndPoint &operator= (FTP_Server_StreamEndPoint &&) = delete;

  int get_callback (char const *flowname,
                    TAO_AV_Callback *&callback) override;

private:
  FTP_Server_Callback callback_;
};

/** Server application receiving the streamed file. */
class Server
{
public:
  Server () = default;
  ~Server () = default;
  Server (Server const &) = delete;
  Server &operator= (Server const &) = delete;
  Server (Server &&) = delete;
  Server &operator= (Server &&) = delete;

  int init (int argc, ACE_TCHAR *argv[]);
  int run ();
  int write_frame (ACE_Message_Block *frame);
  void stop ();

private:
  struct Options
  {
    ACE_TString filename {ACE_TEXT ("output")};
  };

  int parse_args (int argc, ACE_TCHAR *argv[]);

  TAO_Naming_Client my_naming_client_;
  TAO_AV_Endpoint_Reactive_Strategy_B<
    FTP_Server_StreamEndPoint, TAO_VDev, AV_Null_MediaCtrl> reactive_strategy_;
  std::unique_ptr<TAO_MMDevice> mmdevice_;
  FTP_Server_File_Ptr output_file_ {nullptr, std::fclose};
  Options options_;
  bool done_ {};
};

#endif /* TAO_AV_FTP_SERVER_H */
