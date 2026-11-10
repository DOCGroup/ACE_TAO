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

#include <cstddef>
#include <cstdio>
#include <memory>
#include <string>

#include "ace/OS_NS_stdio.h"
#include "orbsvcs/Naming/Naming_Client.h"
#include "orbsvcs/AV/AVStreams_i.h"
#include "orbsvcs/AV/Endpoint_Strategy.h"
#include "orbsvcs/AV/Protocol_Factory.h"

using ProtocolObject_Set = ACE_Unbounded_Set<TAO_AV_Protocol_Object*>;
using ProtocolObject_SetItor = ProtocolObject_Set::iterator;

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
  int get_callback (char const *flowname,
                    TAO_AV_Callback *&callback) override;

  /// Set protocol object corresponding to the transport protocol
  /// chosen.
  int set_protocol_object (char const *flowname,
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

  void shutdown ();

private:
  /// Method to parse the command line arguments.
  int parse_args (int argc, ACE_TCHAR *argv[]);

  /// Method that binds the sender to the receiver.
  int bind_to_receiver ();

  /// The endpoint strategy used by the sender.
  SENDER_ENDPOINT_STRATEGY endpoint_strategy_;

  /// The receiver MMDevice that the sender connects to.
  AVStreams::MMDevice_var receiver_mmdevice_;

  /// The sender MMDevice.
  // Non-owning; ServantBase_var and the POA manage the servant reference.
  TAO_MMDevice *sender_mmdevice_ {};

  /// Stream controller
  // Non-owning; ServantBase_var and the stream controller manage the servant reference.
  TAO_StreamCtrl *streamctrl_ {};

  /// Number of frames sent.
  std::size_t frame_count_ {};

  /// The Naming Service client.
  TAO_Naming_Client naming_client_;

  /// File handle of the file read from.
  std::unique_ptr<FILE, int (*) (FILE *)> input_file_ {nullptr, ACE_OS::fclose};

  /// Message block into which data is read from a file and then sent.
  ACE_Message_Block mb_ {BUFSIZ};

  struct Options
  {
    std::string filename {"input"};
    std::string protocol {"UDP"};
    int frame_rate {30};
  };

  Options options_;

  /// Protocol object corresponding to the transport protocol selected.
  // Non-owning protocol objects managed by the AVStreams framework.
  ProtocolObject_Set protocol_object_;
};
