/* -*- C++ -*- */

#ifndef TAO_AV_TIMESTAMP_RECEIVER_H
#define TAO_AV_TIMESTAMP_RECEIVER_H

#include <cstddef>
#include <cstdio>
#include <memory>

#include "ace/OS_NS_stdio.h"
#include "ace/Singleton.h"
#include "ace/SString.h"
#include "orbsvcs/Naming/Naming_Client.h"
#include "orbsvcs/AV/AVStreams_i.h"
#include "orbsvcs/AV/Endpoint_Strategy.h"
#include "orbsvcs/AV/Policy.h"

using Receiver_File_Ptr = std::unique_ptr<FILE, int (*) (FILE *)>;

class Receiver;
using RECEIVER = ACE_Unmanaged_Singleton<Receiver, ACE_Null_Mutex>;

/** Application callback for incoming timestamped frames. */
class Receiver_Callback : public TAO_AV_Callback
{
public:
  Receiver_Callback () = default;
  Receiver_Callback (Receiver_Callback const &) = delete;
  Receiver_Callback &operator= (Receiver_Callback const &) = delete;
  Receiver_Callback (Receiver_Callback &&) = delete;
  Receiver_Callback &operator= (Receiver_Callback &&) = delete;

  int receive_frame (ACE_Message_Block *frame,
                     TAO_AV_frame_info *frame_info,
                     ACE_Addr const &peer_address) override;
  int handle_destroy () override;
};

/** Server stream endpoint providing the application callback. */
class Receiver_StreamEndPoint : public TAO_Server_StreamEndPoint
{
public:
  Receiver_StreamEndPoint () = default;
  Receiver_StreamEndPoint (Receiver_StreamEndPoint const &) = delete;
  Receiver_StreamEndPoint &operator= (Receiver_StreamEndPoint const &) = delete;
  Receiver_StreamEndPoint (Receiver_StreamEndPoint &&) = delete;
  Receiver_StreamEndPoint &operator= (Receiver_StreamEndPoint &&) = delete;

  int get_callback (char const *flowname,
                    TAO_AV_Callback *&callback) override;

private:
  Receiver_Callback callback_;
};

using RECEIVER_ENDPOINT_STRATEGY = TAO_AV_Endpoint_Reactive_Strategy_B<
  Receiver_StreamEndPoint, TAO_VDev, AV_Null_MediaCtrl>;

/** Receives timestamped frames and writes their payload to a file. */
class Receiver
{
public:
  Receiver () = default;
  ~Receiver () = default;
  Receiver (Receiver const &) = delete;
  Receiver &operator= (Receiver const &) = delete;
  Receiver (Receiver &&) = delete;
  Receiver &operator= (Receiver &&) = delete;

  int init (int argc, ACE_TCHAR *argv[]);
  int run ();
  int receive_frame (ACE_Message_Block *frame);
  void stop ();

private:
  struct Options
  {
    ACE_TString filename {ACE_TEXT ("output")};
  };

  int parse_args (int argc, ACE_TCHAR *argv[]);

  TAO_Naming_Client naming_client_;
  RECEIVER_ENDPOINT_STRATEGY reactive_strategy_;
  Receiver_File_Ptr output_file_ {nullptr, std::fclose};
  Options options_;
  std::size_t frame_count_ {};
  bool done_ {};
};

#endif /* TAO_AV_TIMESTAMP_RECEIVER_H */
