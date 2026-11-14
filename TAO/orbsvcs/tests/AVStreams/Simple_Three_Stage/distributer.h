/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    distributer.h
 *
 *  Process to receive data from the sender and send it to the
 *  receiver.
 *
 *  @author Yamuna Krishnamurthy <yamuna@cs.wustl.edu>
 */
//=============================================================================

#ifndef TAO_AVSTREAMS_SIMPLE_THREE_STAGE_DISTRIBUTER_H
#define TAO_AVSTREAMS_SIMPLE_THREE_STAGE_DISTRIBUTER_H

#include "orbsvcs/Naming/Naming_Client.h"
#include "orbsvcs/AV/AVStreams_i.h"
#include "orbsvcs/AV/Endpoint_Strategy.h"
#include "orbsvcs/AV/Policy.h"

/**
 * @class Distributer_Receiver_Callback
 *
 * @brief Application defined callback object.
 *
 * AVStreams calls this class when data shows up from a sender.
 */
class Distributer_Receiver_Callback : public TAO_AV_Callback
{
public:
  Distributer_Receiver_Callback () = default;
  Distributer_Receiver_Callback (Distributer_Receiver_Callback const &) = delete;
  Distributer_Receiver_Callback &operator= (Distributer_Receiver_Callback const &) = delete;
  Distributer_Receiver_Callback (Distributer_Receiver_Callback &&) = delete;
  Distributer_Receiver_Callback &operator= (Distributer_Receiver_Callback &&) = delete;

  /// Receive a frame from the sender and forward it to the receiver.
  int receive_frame (ACE_Message_Block *frame,
                     TAO_AV_frame_info *frame_info,
                     ACE_Addr const &peer_address) override;

  /// Shut down the receiver stream when the sender finishes.
  int handle_destroy () override;

private:
  /// Count of the frames passing through us.
  int frame_count_ {1};
};

/**
 * @class Distributer_Receiver_StreamEndPoint
 *
 * @brief Application defined stream endpoint object.
 *
 * AVStreams calls this class during connection setup.
 */
class Distributer_Receiver_StreamEndPoint : public TAO_Server_StreamEndPoint
{
public:
  Distributer_Receiver_StreamEndPoint () = default;
  Distributer_Receiver_StreamEndPoint (Distributer_Receiver_StreamEndPoint const &) = delete;
  Distributer_Receiver_StreamEndPoint &operator= (Distributer_Receiver_StreamEndPoint const &) = delete;
  Distributer_Receiver_StreamEndPoint (Distributer_Receiver_StreamEndPoint &&) = delete;
  Distributer_Receiver_StreamEndPoint &operator= (Distributer_Receiver_StreamEndPoint &&) = delete;

  /// Return the receiver's application callback.
  int get_callback (char const *flowname,
                    TAO_AV_Callback *&callback) override;

private:
  /// Receiver application callback.
  Distributer_Receiver_Callback callback_;
};

/**
 * @class Distributer_Sender_StreamEndPoint
 *
 * @brief Defines a sender stream endpoint.
 */
class Distributer_Sender_StreamEndPoint : public TAO_Client_StreamEndPoint
{
public:
  Distributer_Sender_StreamEndPoint () = default;
  Distributer_Sender_StreamEndPoint (Distributer_Sender_StreamEndPoint const &) = delete;
  Distributer_Sender_StreamEndPoint &operator= (Distributer_Sender_StreamEndPoint const &) = delete;
  Distributer_Sender_StreamEndPoint (Distributer_Sender_StreamEndPoint &&) = delete;
  Distributer_Sender_StreamEndPoint &operator= (Distributer_Sender_StreamEndPoint &&) = delete;

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
          <Distributer_Sender_StreamEndPoint,
           TAO_VDev,
           AV_Null_MediaCtrl>;

using RECEIVER_ENDPOINT_STRATEGY = TAO_AV_Endpoint_Reactive_Strategy_B
          <Distributer_Receiver_StreamEndPoint,
           TAO_VDev,
           AV_Null_MediaCtrl>;

/**
 * @class Distributer
 *
 * @brief Distributer Application.
 *
 * The distributer is the intermediate receiver that receives
 * data from the sender and forwards to a receiver.
 */
class Distributer
{
public:
  Distributer () = default;

  Distributer (Distributer const &) = delete;
  Distributer &operator= (Distributer const &) = delete;
  Distributer (Distributer &&) = delete;
  Distributer &operator= (Distributer &&) = delete;

  /// Initialize data components.
  int init (int argc,
            ACE_TCHAR *argv[]);

  /// Resolve the reference of the mmdevice from the naming service.
  void bind_to_mmdevice (AVStreams::MMDevice_ptr &mmdevice,
                         ACE_CString const &mmdevice_name);

  /// Return the receiver's stream controller servant.
  TAO_StreamCtrl *receiver_streamctrl ();

  /// Set and return the sender protocol object borrowed from AVStreams.
  int sender_protocol_object  (TAO_AV_Protocol_Object *object);
  TAO_AV_Protocol_Object *sender_protocol_object ();

  /// Get or set whether the stream has finished.
  bool done () const;
  void done (bool);

protected:
  /// The Naming Service client.
  TAO_Naming_Client naming_client_;

  /// The sender endpoint strategy.
  SENDER_ENDPOINT_STRATEGY sender_endpoint_strategy_;

  /// The receiver endpoint strategy.
  RECEIVER_ENDPOINT_STRATEGY receiver_endpoint_strategy_;

  /// Servant reference borrowed from the POA after activation.
  TAO_MMDevice *distributer_receiver_mmdevice_ {};

  /// Servant reference borrowed from the POA after activation.
  TAO_MMDevice *distributer_sender_mmdevice_ {};

  /// The receiver MMDevice.
  AVStreams::MMDevice_var receiver_mmdevice_;

  /// The sender MMDevice.
  AVStreams::MMDevice_var sender_mmdevice_;

  /// Protocol object borrowed from the AVStreams endpoint strategy.
  TAO_AV_Protocol_Object *sender_protocol_object_ {};

  /// Stream controller servant reference borrowed from the POA.
  TAO_StreamCtrl *receiver_streamctrl_ {};

  /// Selected protocol - default is UDP
  ACE_CString protocol_ {"UDP"};

  /// Flag to know when we are done.
  bool done_ {};
};

#endif /* TAO_AVSTREAMS_SIMPLE_THREE_STAGE_DISTRIBUTER_H */
