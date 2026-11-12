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

/// Defines the sender stream endpoint for this application.
class Sender_StreamEndPoint : public TAO_Client_StreamEndPoint
{
public:
  Sender_StreamEndPoint () = default;
  Sender_StreamEndPoint (Sender_StreamEndPoint const &) = delete;
  Sender_StreamEndPoint &operator= (Sender_StreamEndPoint const &) = delete;
  Sender_StreamEndPoint (Sender_StreamEndPoint &&) = delete;
  Sender_StreamEndPoint &operator= (Sender_StreamEndPoint &&) = delete;

  /// Create the callback AVStreams uses for subsequent upcalls.
  int get_callback (const char *flowname,
                    TAO_AV_Callback *&callback) override;

  /// Set the protocol object selected for this flow.
  int set_protocol_object (const char *flowname,
                           TAO_AV_Protocol_Object *object) override;

  /// Update the QoS parameters for the specified flows.
  CORBA::Boolean modify_QoS (AVStreams::streamQoS &new_qos,
                             AVStreams::flowSpec const &flow_spec) override;

protected:
  /// Callback used by AVStreams.
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

  /// Initialize the sender and bind it to the receiver.
  int init (int argc,
            ACE_TCHAR *argv[]);

  /// Pace and send data from the input file.
  int pace_data ();

  /// Store the protocol object selected for the flow.
  void protocol_object (TAO_AV_Protocol_Object *protocol_object);

private:
  struct Options
  {
    std::string filename {"input"};
    std::string protocol {"UDP"};
    int frame_rate {1};
  };

  /// Parse the command-line options.
  int parse_args (int argc, ACE_TCHAR *argv[]);

  /// Resolve the receiver MMDevice through the Naming Service.
  int bind_to_receiver ();

  /// Endpoint strategy used by the sender.
  SENDER_ENDPOINT_STRATEGY endpoint_strategy_;

  /// Receiver MMDevice connected by this sender.
  AVStreams::MMDevice_var receiver_mmdevice_;

  /// Sender MMDevice servant.
  TAO_MMDevice *sender_mmdevice_ {};

  /// Stream controller servant.
  TAO_StreamCtrl *streamctrl_ {};

  /// Number of frames sent.
  std::size_t frame_count_ {};

  /// Command-line settings.
  Options options_;

  /// Naming Service client.
  TAO_Naming_Client naming_client_;

  /// Input file owned by this sender.
  std::unique_ptr<FILE, int (*) (FILE *)> input_file_ {nullptr, std::fclose};

  /// Buffer used to read and send file data.
  ACE_Message_Block mb_ {BUFSIZ};

  /// Protocol object selected for the transport.
  TAO_AV_Protocol_Object *protocol_object_ {};
};

#endif /* TAO_ORBsvcs_AVSTREAMS_TESTS_MODIFY_QOS_SENDER_H */
