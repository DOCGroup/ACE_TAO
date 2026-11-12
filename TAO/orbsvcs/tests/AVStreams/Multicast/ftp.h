/* -*- C++ -*- */
#ifndef TAO_AV_FTP_H
#define TAO_AV_FTP_H

#include <cstdio>
#include <cstddef>
#include <memory>
#include <string>

#include "ace/Get_Opt.h"
#include "ace/OS_NS_stdio.h"
#include "ace/SString.h"
#include "ace/Time_Value.h"
#include "tao/PortableServer/PortableServer.h"
#include "tao/Strategies/advanced_resource.h"
#include "orbsvcs/Naming/Naming_Client.h"
#include "orbsvcs/AV/AVStreams_i.h"
#include "orbsvcs/AV/Endpoint_Strategy.h"
#include "orbsvcs/AV/Transport.h"
#include "orbsvcs/AV/sfp.h"
#include "orbsvcs/AV/MCast.h"

class FTP_Client_Callback
  : public TAO_AV_Callback
{
public:
  FTP_Client_Callback () = default;
  FTP_Client_Callback (FTP_Client_Callback const &) = delete;
  FTP_Client_Callback &operator= (FTP_Client_Callback const &) = delete;
  FTP_Client_Callback (FTP_Client_Callback &&) = delete;
  FTP_Client_Callback &operator= (FTP_Client_Callback &&) = delete;

  int handle_end_stream ();
  void set_protocol_object (TAO_AV_Protocol_Object *protocol_object)
  {
    this->protocol_object_ = protocol_object;
  }
  int handle_timeout (void *arg) override;
  void get_timeout (ACE_Time_Value *&tv,
                    void *&arg) override;

protected:
  int count_ {};
  ACE_Time_Value timeout_ {2};
  /// Borrowed from AVStreams transport setup; this callback does not own it.
  TAO_AV_Protocol_Object *protocol_object_ {};
};

class FTP_Client_StreamEndPoint
  : public TAO_Client_StreamEndPoint
{
public:
  FTP_Client_StreamEndPoint () = default;
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

using ENDPOINT_STRATEGY = TAO_AV_Endpoint_Reactive_Strategy_A<FTP_Client_StreamEndPoint, TAO_VDev, AV_Null_MediaCtrl>;

class Endpoint_Reactive_Strategy
  : public ENDPOINT_STRATEGY
{
public:
  Endpoint_Reactive_Strategy (CORBA::ORB_ptr orb,
                              PortableServer::POA_ptr poa);

  int make_stream_endpoint (FTP_Client_StreamEndPoint *&endpoint) override;
};

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
  /// Borrowed handle; Client retains ownership through its File_Ptr member.
  FILE *file ();
  TAO_StreamCtrl &streamctrl ();

private:
  struct Options
  {
    ACE_TString filename {ACE_TEXT ("input")};
    std::string address {"224.9.9.2:12345"};
    std::string protocol {"UDP"};
    bool use_sfp {};
  };

  using File_Ptr = std::unique_ptr<FILE, int (*) (FILE *)>;

  int parse_args (int argc, ACE_TCHAR *argv[]);
  int bind_to_server (char const *name);

  Endpoint_Reactive_Strategy endpoint_strategy_;
  AVStreams::MMDevice_var server_mmdevice_;
  TAO_MMDevice client_mmdevice_;
  TAO_StreamCtrl streamctrl_;
  Options options_;

  TAO_Naming_Client my_naming_client_;
  File_Ptr fp_ {nullptr, std::fclose};
};

using CLIENT = ACE_Unmanaged_Singleton<Client, ACE_Null_Mutex>;

#endif /* TAO_AV_FTP_H */
