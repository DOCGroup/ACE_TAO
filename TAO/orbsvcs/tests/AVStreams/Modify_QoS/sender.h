/* -*- C++ -*- */

//=============================================================================
/**
 *  @file sender.h
 *
 *  @author Yamuna Krishnamurthy <yamuna@cs.wustl.edu>
 */
//=============================================================================

#ifndef TAO_ORBsvcs_AVSTREAMS_TESTS_MODIFY_QOS_SENDER_H
#define TAO_ORBsvcs_AVSTREAMS_TESTS_MODIFY_QOS_SENDER_H

#include <cstdio>
#include <cstddef>
#include <memory>
#include <string>

#include "ace/OS_NS_stdio.h"
#include "orbsvcs/Naming/Naming_Client.h"
#include "orbsvcs/AV/AVStreams_i.h"
#include "orbsvcs/AV/Endpoint_Strategy.h"
#include "orbsvcs/AV/Protocol_Factory.h"

/// Defines application specific sender stream endpoint.
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

  /// The over-ridden modify_qos method to change the qos of the
  /// flows specified in the flow spec to that in the new_qos.
  CORBA::Boolean modify_QoS (AVStreams::streamQoS &new_qos,
                             AVStreams::flowSpec const &flow_spec) override;

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
 * @brief Class is responsible for streaming (and pacing) data to a
 * receiver.
 */
class Sender
{
public:
  Sender () = default;
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
    std::string filename {"input"};
    std::string protocol {"UDP"};
    int frame_rate {1};
  };

  /// Method to parse the command line arguments.
  int parse_args (int argc, ACE_TCHAR *argv[]);

  /// Method that binds the sender to the receiver.
  int bind_to_receiver ();

  /// The endpoint strategy used by the sender.
  SENDER_ENDPOINT_STRATEGY endpoint_strategy_;

  /// The receiver MMDevice that the sender connects to.
  AVStreams::MMDevice_var receiver_mmdevice_;

  /// The sender MMDevice.
  TAO_MMDevice *sender_mmdevice_ {};

  /// Stream controller
  TAO_StreamCtrl *streamctrl_ {};

  /// Number of frames sent.
  std::size_t frame_count_ {};

  /// Command-line settings for the sender.
  Options options_;

  /// The Naming Service client.
  TAO_Naming_Client naming_client_;

  /// File handle of the file read from.
  std::unique_ptr<FILE, int (*) (FILE *)> input_file_ {nullptr, std::fclose};

  /// Message block into which data is read from a file and then sent.
  ACE_Message_Block mb_ {BUFSIZ};

  /// Protocol object corresponding to the transport protocol selected.
  TAO_AV_Protocol_Object *protocol_object_ {};
};

#endif /* TAO_ORBsvcs_AVSTREAMS_TESTS_MODIFY_QOS_SENDER_H */
