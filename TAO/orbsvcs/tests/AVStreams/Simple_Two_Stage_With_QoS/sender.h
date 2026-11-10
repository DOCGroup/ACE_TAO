/* -*- C++ -*- */

#ifndef TAO_AV_QOS_SENDER_H
#define TAO_AV_QOS_SENDER_H

#include <cstdio>
#include <memory>
#include <string>

#include "ace/Message_Block.h"
#include "ace/OS_NS_stdio.h"
#include "ace/Singleton.h"
#include "ace/SString.h"
#include "orbsvcs/Naming/Naming_Client.h"
#include "orbsvcs/AV/AVStreams_i.h"
#include "orbsvcs/AV/Endpoint_Strategy.h"
#include "orbsvcs/AV/Protocol_Factory.h"

using Sender_File_Ptr = std::unique_ptr<FILE, int (*) (FILE *)>;

class Sender;
using SENDER = ACE_Unmanaged_Singleton<Sender, ACE_Null_Mutex>;

class Sender_StreamEndPoint : public TAO_Client_StreamEndPoint
{
public:
  Sender_StreamEndPoint () = default;
  Sender_StreamEndPoint (Sender_StreamEndPoint const &) = delete;
  Sender_StreamEndPoint &operator= (Sender_StreamEndPoint const &) = delete;
  Sender_StreamEndPoint (Sender_StreamEndPoint &&) = delete;
  Sender_StreamEndPoint &operator= (Sender_StreamEndPoint &&) = delete;

  int get_callback (char const *flowname,
                    TAO_AV_Callback *&callback) override;
  int set_protocol_object (char const *flowname,
                           TAO_AV_Protocol_Object *object) override;
  CORBA::Boolean modify_QoS (AVStreams::streamQoS &new_qos,
                             AVStreams::flowSpec const &the_flows) override;

private:
  TAO_AV_Callback callback_;
};

using SENDER_ENDPOINT_STRATEGY = TAO_AV_Endpoint_Reactive_Strategy_A<
  Sender_StreamEndPoint, TAO_VDev, AV_Null_MediaCtrl>;

class Sender
{
public:
  Sender () = default;
  Sender (Sender const &) = delete;
  Sender &operator= (Sender const &) = delete;
  Sender (Sender &&) = delete;
  Sender &operator= (Sender &&) = delete;

  int init (int argc, ACE_TCHAR *argv[]);
  int pace_data ();
  void protocol_object (TAO_AV_Protocol_Object *protocol_object);
  void fill_qos (AVStreams::streamQoS &qos);

private:
  struct Options
  {
    ACE_TString filename {ACE_TEXT ("input")};
    std::string protocol {"UDP"};
    double frame_rate {1.0};
    ACE_TString address;
    ACE_TString peer_address;
  };

  int parse_args (int argc, ACE_TCHAR *argv[]);
  int bind_to_receiver ();

  SENDER_ENDPOINT_STRATEGY endpoint_strategy_;
  AVStreams::MMDevice_var receiver_mmdevice_;
  // Borrowed from streamctrl_owner_, which keeps the servant alive.
  TAO_StreamCtrl *streamctrl_ {};
  PortableServer::ServantBase_var streamctrl_owner_;
  TAO_Naming_Client naming_client_;
  Options options_;
  Sender_File_Ptr input_file_ {nullptr, ACE_OS::fclose};
  ACE_Message_Block mb_ {BUFSIZ};
  // Borrowed from the endpoint's protocol factory.
  TAO_AV_Protocol_Object *protocol_object_ {};
  ACE_Time_Value inter_frame_time_;
  CORBA::ULong peak_bandwidth_ {18400};
  int frame_count_ {};
};

#endif /* TAO_AV_QOS_SENDER_H */
