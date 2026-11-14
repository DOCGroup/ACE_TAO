/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    ftp.h
 *
 *  Ftp client to send data
 *
 *  @author Yamuna Krishnamurthy <yamuna@cs.wustl.edu>
 */
//=============================================================================

#ifndef TAO_AV_FTP_H
#define TAO_AV_FTP_H

#include <cstdio>
#include <memory>
#include <string>

#include "ace/Get_Opt.h"
#include "ace/Message_Block.h"
#include "ace/OS_NS_stdio.h"
#include "ace/SString.h"
#include "orbsvcs/Naming/Naming_Client.h"
#include "orbsvcs/AV/AVStreams_i.h"
#include "orbsvcs/AV/Endpoint_Strategy.h"
#include "orbsvcs/AV/Policy.h"
#include "orbsvcs/AV/Protocol_Factory.h"

using FTP_File_Ptr = std::unique_ptr<FILE, int (*) (FILE *)>;

/** Client callback for application-specific AVStreams processing. */
class FTP_Client_Callback : public TAO_AV_Callback
{
public:
  FTP_Client_Callback () = default;
  FTP_Client_Callback (FTP_Client_Callback const &) = delete;
  FTP_Client_Callback &operator= (FTP_Client_Callback const &) = delete;
  FTP_Client_Callback (FTP_Client_Callback &&) = delete;
  FTP_Client_Callback &operator= (FTP_Client_Callback &&) = delete;
};

/** Client stream endpoint providing the application callback and protocol object. */
class FTP_Client_StreamEndPoint : public TAO_Client_StreamEndPoint
{
public:
  FTP_Client_StreamEndPoint ();
  FTP_Client_StreamEndPoint (FTP_Client_StreamEndPoint const &) = delete;
  FTP_Client_StreamEndPoint &operator= (FTP_Client_StreamEndPoint const &) = delete;
  FTP_Client_StreamEndPoint (FTP_Client_StreamEndPoint &&) = delete;
  FTP_Client_StreamEndPoint &operator= (FTP_Client_StreamEndPoint &&) = delete;

  int get_callback (char const *flowname,
                    TAO_AV_Callback *&callback) override;
  int set_protocol_object (char const *flowname,
                           TAO_AV_Protocol_Object *object) override;

protected:
  FTP_Client_Callback callback_;
};

using ENDPOINT_STRATEGY = TAO_AV_Endpoint_Reactive_Strategy_A<
  FTP_Client_StreamEndPoint, TAO_VDev, AV_Null_MediaCtrl>;

/** Client application that streams file data to the server. */
class Client
{
public:
  Client ();
  Client (Client const &) = delete;
  Client &operator= (Client const &) = delete;
  Client (Client &&) = delete;
  Client &operator= (Client &&) = delete;

  int init (int argc, ACE_TCHAR *argv[]);
  void set_protocol_object (TAO_AV_Protocol_Object *protocol_object);
  int pace_data ();

private:
  struct Options
  {
    ACE_TString filename {ACE_TEXT ("input")};
    std::string address;
    std::string peer_address;
    std::string protocol {"UDP"};
    int frame_rate {30};
    bool use_sfp {};
  };

  int parse_args (int argc, ACE_TCHAR *argv[]);
  int bind_to_server ();

  ENDPOINT_STRATEGY endpoint_strategy_;
  AVStreams::MMDevice_var server_mmdevice_;
  TAO_MMDevice client_mmdevice_;
  TAO_StreamCtrl streamctrl_;
  TAO_Naming_Client my_naming_client_;
  Options options_;
  FTP_File_Ptr fp_ {nullptr, std::fclose};
  ACE_Message_Block mb_ {BUFSIZ};
  // The AVStreams framework owns the protocol object.
  TAO_AV_Protocol_Object *protocol_object_ {};
  int count_ {};
};

using CLIENT = ACE_Unmanaged_Singleton<Client, ACE_Null_Mutex>;

#endif /* TAO_AV_FTP_H */
