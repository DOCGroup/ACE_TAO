/* -*- C++ -*- */
#ifndef TAO_AV_FTP_H
#define TAO_AV_FTP_H

#include <cstddef>
#include <memory>
#include <cstdio>
#include <string>

#include "ace/Get_Opt.h"
#include "ace/High_Res_Timer.h"
#include "orbsvcs/AV/AVStreams_i.h"
#include "orbsvcs/AV/Flows_T.h"
#include "orbsvcs/AV/Endpoint_Strategy.h"
#include "orbsvcs/AV/Transport.h"
#include "orbsvcs/AV/sfp.h"
#include "orbsvcs/AV/MCast.h"

class Client;

class FTP_Client_Callback;

class FTP_Client_Producer
  : public virtual TAO_FlowProducer
{
public:
  FTP_Client_Producer ();
  FTP_Client_Producer (FTP_Client_Producer const &) = delete;
  FTP_Client_Producer &operator= (FTP_Client_Producer const &) = delete;
  FTP_Client_Producer (FTP_Client_Producer &&) = delete;
  FTP_Client_Producer &operator= (FTP_Client_Producer &&) = delete;
  int get_callback (char const *flowname,
                    TAO_AV_Callback *&callback) override;
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
  void get_timeout (ACE_Time_Value *&tv,
                    void *&arg) override;

protected:
  size_t count_ {};
  ACE_Time_Value timeout_ {2};
};

using ENDPOINT_STRATEGY = TAO_AV_Endpoint_Reactive_Strategy_A<TAO_StreamEndPoint_A, TAO_VDev, AV_Null_MediaCtrl>;

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
  /// Borrowed file handle, owned by this client.
  FILE *file () const;
  TAO_StreamCtrl *streamctrl ();
  AVStreams::protocolSpec protocols () const;
  char const *format () const;

private:
  friend class FTP_Client_Producer;

  struct Options
  {
    std::string filename {"input"};
    std::string address;
    std::string peer_addr;
    std::string protocol {"UDP"};
    bool use_sfp {};
  };

  FTP_Client_Callback &callback ();
  int parse_args (int argc, ACE_TCHAR *argv[]);
  ENDPOINT_STRATEGY endpoint_strategy_;
  TAO_StreamCtrl streamctrl_;
  FTP_Client_Callback callback_;
  Options options_;
  std::unique_ptr<FILE, int (*) (FILE *)> file_ {nullptr, std::fclose};
  CORBA::ORB_var orb_;
  AVStreams::StreamEndPoint_A_var sep_a_;
  AVStreams::FlowProducer_var fep_a_obj_;
};

using CLIENT = ACE_Unmanaged_Singleton<Client, ACE_Null_Mutex>;

#endif /* TAO_AV_FTP_H */
