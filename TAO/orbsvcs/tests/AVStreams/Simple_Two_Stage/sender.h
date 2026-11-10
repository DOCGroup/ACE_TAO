/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    sender.h
 *
 *  This application reads data from a file and sends it to s
 *  receiver.
 *
 *  @author Yamuna Krishnamurthy <yamuna@cs.wustl.edu>
 */
//=============================================================================


#include "orbsvcs/Naming/Naming_Client.h"
#include "orbsvcs/AV/AVStreams_i.h"
#include "orbsvcs/AV/Endpoint_Strategy.h"
#include "orbsvcs/AV/Protocol_Factory.h"
#include <cstddef>
#include <cstdio>
#include <memory>
#include <string>
#include <vector>

/**
 * @class Sender_StreamEndPoint
 *
 * @brief Defines a sender stream endpoint.
 */
class Sender_StreamEndPoint : public TAO_Client_StreamEndPoint
{
public:
  Sender_StreamEndPoint () = default;
  Sender_StreamEndPoint (Sender_StreamEndPoint const &) = delete;
  Sender_StreamEndPoint &operator= (Sender_StreamEndPoint const &) = delete;
  Sender_StreamEndPoint (Sender_StreamEndPoint &&) = delete;
  Sender_StreamEndPoint &operator= (Sender_StreamEndPoint &&) = delete;

  /// Create the application callback and return its handle to
  /// AVStreams for further application callbacks.
  int get_callback (const char *flowname,
                    TAO_AV_Callback *&callback) override;

  /// Set protocol object corresponding to the transport protocol
  /// chosen.
  int set_protocol_object (const char *flowname,
                           TAO_AV_Protocol_Object *object) override;

protected:
  /// Application callback.
  TAO_AV_Callback callback_;
};

using SENDER_ENDPOINT_STRATEGY = TAO_AV_Endpoint_Reactive_Strategy_A
          <Sender_StreamEndPoint,
           TAO_VDev,
           AV_Null_MediaCtrl>;

/**
 * @class Sender
 *
 * @brief Sender Application.
 *
 * Class is responsible for streaming (and pacing) data to a
 * receiver.
 */
class Sender
{
public:
  Sender ();
  Sender (Sender const &) = delete;
  Sender &operator= (Sender const &) = delete;
  Sender (Sender &&) = delete;
  Sender &operator= (Sender &&) = delete;

  /// Method to initialize the various data components.
  int init (int argc,
            ACE_TCHAR *argv[]);

  /// Method to pace and send data from a file.
  int pace_data ();

  /// Set the protocol object corresponding to the transport protocol chosen.
  void protocol_object (TAO_AV_Protocol_Object *protocol_object);

private:
  struct Options
  {
    ACE_TString filename {ACE_TEXT ("input")};
    ACE_TString protocol {ACE_TEXT ("UDP")};
    double frame_rate {30.0};
    long max_frame_count {20};
    int buffer_size {1000};
    std::string address;
    std::string peer_address;
    std::vector<std::string> local_sec_addrs;
    std::vector<std::string> peer_sec_addrs;
  };

  /// Method to parse the command line arguments.
  int parse_args (int argc, ACE_TCHAR *argv[]);

  /// Method that binds the sender to the receiver.
  int bind_to_receiver ();

  /// The endpoint strategy used by the sender.
  SENDER_ENDPOINT_STRATEGY endpoint_strategy_;

  /// The receiver MMDevice that the sender connects to.
  AVStreams::MMDevice_var receiver_mmdevice_;

  /// Sender servant pointer, borrowed after ownership transfers to the POA.
  TAO_MMDevice *sender_mmdevice_ {};

  /// Stream controller servant pointer, borrowed after ownership transfers to the POA.
  TAO_StreamCtrl *streamctrl_ {};

  /// Number of frames sent.
  int frame_count_ {};

  /// The Naming Service client.
  TAO_Naming_Client naming_client_;

  /// File handle of the file read from.
  std::unique_ptr<FILE, int (*) (FILE *)> input_file_;

  Options options_;

  /// Message block into which data is read from a file and then sent.
  ACE_Message_Block mb_;

  /// Protocol object borrowed from the AVStreams endpoint strategy.
  TAO_AV_Protocol_Object *protocol_object_ {};
};
