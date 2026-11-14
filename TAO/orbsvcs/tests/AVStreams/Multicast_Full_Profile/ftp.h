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
#include "orbsvcs/Naming/Naming_Client.h"
#include "orbsvcs/AV/AVStreams_i.h"
#include "orbsvcs/AV/Flows_T.h"
#include "orbsvcs/AV/Endpoint_Strategy.h"
#include "orbsvcs/AV/sfp.h"
#include "orbsvcs/AV/MCast.h"

class FTP_Client_Callback;
class FTP_Client_Producer
  : public virtual TAO_FlowProducer
{
public:
  FTP_Client_Producer ();
  ~FTP_Client_Producer () override;
  FTP_Client_Producer (FTP_Client_Producer const &) = delete;
  FTP_Client_Producer &operator= (FTP_Client_Producer const &) = delete;
  FTP_Client_Producer (FTP_Client_Producer &&) = delete;
  FTP_Client_Producer &operator= (FTP_Client_Producer &&) = delete;
  int get_callback (char const *flowname,
                    TAO_AV_Callback *&callback) override;
  int set_protocol_object (char const *flowname,
                           TAO_AV_Protocol_Object *object) override;
protected:
  std::unique_ptr<FTP_Client_Callback> callback_ {};
};

class FTP_Client_Callback
  : public TAO_AV_Callback
{
public:
  FTP_Client_Callback () = default;
  FTP_Client_Callback (FTP_Client_Callback const &) = delete;
  FTP_Client_Callback &operator= (FTP_Client_Callback const &) = delete;
  FTP_Client_Callback (FTP_Client_Callback &&) = delete;
  FTP_Client_Callback &operator= (FTP_Client_Callback &&) = delete;
  int handle_timeout (void *arg) override;
  int handle_end_stream ();
  void get_timeout (ACE_Time_Value *&tv,
                    void *&arg) override;
  void set_protocol_object (TAO_AV_Protocol_Object *protocol_object)
  {
    this->protocol_object_ = protocol_object;
  }

protected:
  int count_ {};
  ACE_Time_Value timeout_ {2};
  /// Borrowed from AVStreams transport setup; the callback does not own it.
  TAO_AV_Protocol_Object *protocol_object_ {};
};

using ENDPOINT_STRATEGY = TAO_AV_Endpoint_Reactive_Strategy_A<TAO_StreamEndPoint_A, TAO_VDev, AV_Null_MediaCtrl>;

using FTP_Client_FDev = TAO_FDev<FTP_Client_Producer, TAO_FlowConsumer>;

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
  TAO_StreamCtrl *streamctrl ();
  AVStreams::protocolSpec protocols ();
  char const *format ();

private:
  struct Options
  {
    ACE_TString filename {ACE_TEXT ("input")};
    std::string address {"224.9.9.2:10002"};
    std::string protocol {"UDP"};
    bool use_sfp {};
  };

  using File_Ptr = std::unique_ptr<FILE, int (*) (FILE *)>;

  int parse_args (int argc, ACE_TCHAR *argv[]);
  int bind_to_server (char const *name);

  ENDPOINT_STRATEGY endpoint_strategy_;
  AVStreams::MMDevice_var server_mmdevice_;
  TAO_MMDevice client_mmdevice_;
  TAO_StreamCtrl streamctrl_;
  Options options_;

  TAO_Naming_Client my_naming_client_;
  File_Ptr fp_ {nullptr, std::fclose};
};

using CLIENT = ACE_Unmanaged_Singleton<Client, ACE_Null_Mutex>;

#endif /* TAO_AV_FTP_H */
