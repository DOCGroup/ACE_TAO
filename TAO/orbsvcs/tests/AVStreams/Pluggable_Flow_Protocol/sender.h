/* -*- C++ -*- */

#ifndef TAO_AV_TIMESTAMP_SENDER_H
#define TAO_AV_TIMESTAMP_SENDER_H

#include <cstddef>
#include <cstdio>
#include <memory>
#include <string>

#include "ace/Message_Block.h"
#include "ace/OS_NS_stdio.h"
#include "ace/SString.h"
#include "ace/Singleton.h"
#include "orbsvcs/Naming/Naming_Client.h"
#include "orbsvcs/AV/AVStreams_i.h"
#include "orbsvcs/AV/Endpoint_Strategy.h"
#include "orbsvcs/AV/Protocol_Factory.h"

using Sender_File_Ptr = std::unique_ptr<FILE, int (*) (FILE *)>;

class Sender;
using SENDER = ACE_Unmanaged_Singleton<Sender, ACE_Null_Mutex>;

/** Client stream endpoint for the timestamped flow. */
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

protected:
  TAO_AV_Callback callback_;
};

using SENDER_ENDPOINT_STRATEGY = TAO_AV_Endpoint_Reactive_Strategy_A<
  Sender_StreamEndPoint, TAO_VDev, AV_Null_MediaCtrl>;

/** Sends a file as timestamped AVStreams frames. */
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

private:
  struct Options
  {
    ACE_TString filename {ACE_TEXT ("input")};
    std::string protocol {"UDP"};
    int frame_rate {30};
  };

  int parse_args (int argc, ACE_TCHAR *argv[]);
  int bind_to_receiver ();

  SENDER_ENDPOINT_STRATEGY endpoint_strategy_;
  AVStreams::MMDevice_var receiver_mmdevice_;
  // Borrowed from streamctrl_servant_, which owns the servant reference.
  TAO_StreamCtrl *streamctrl_ {};
  PortableServer::ServantBase_var streamctrl_servant_;
  TAO_Naming_Client naming_client_;
  Options options_;
  Sender_File_Ptr input_file_ {nullptr, std::fclose};
  ACE_Message_Block mb_ {BUFSIZ};
  TAO_AV_Protocol_Object *protocol_object_ {};
  size_t frame_count_ {};
};

#endif /* TAO_AV_TIMESTAMP_SENDER_H */
