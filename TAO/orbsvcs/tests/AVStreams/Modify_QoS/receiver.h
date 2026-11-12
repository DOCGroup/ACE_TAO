/* -*- C++ -*- */
//=============================================================================
/**
 *  @file   receiver.h
 *
 *  receiver.h,v 1.1 2001/04/15 02:56:26 yamuna Exp
 *
 *  @author Yamuna Krishnamurthy <yamuna@cs.wustl.edu>
 */
//=============================================================================

#ifndef TAO_ORBsvcs_AVSTREAMS_TESTS_MODIFY_QOS_RECEIVER_H
#define TAO_ORBsvcs_AVSTREAMS_TESTS_MODIFY_QOS_RECEIVER_H

#include <cstddef>

#include "orbsvcs/Naming/Naming_Client.h"
#include "orbsvcs/AV/AVStreams_i.h"
#include "orbsvcs/AV/Endpoint_Strategy.h"
#include "orbsvcs/AV/Policy.h"

/// AVStreams calls this class when data shows up from a sender.
class Receiver_Callback : public TAO_AV_Callback
{
public:
  Receiver_Callback () = default;
  Receiver_Callback (Receiver_Callback const &) = delete;
  Receiver_Callback &operator= (Receiver_Callback const &) = delete;
  Receiver_Callback (Receiver_Callback &&) = delete;
  Receiver_Callback &operator= (Receiver_Callback &&) = delete;

  /// Method that is called when there is data to be received from a
  /// sender.
  int receive_frame (ACE_Message_Block *frame,
                     TAO_AV_frame_info *frame_info,
                     ACE_Addr const &peer_address) override;

  /// Called when the sender is done sending data and wants to close
  /// down the connection.
  int handle_destroy () override;

  /// Set the related stream control for this flow.
  void streamctrl (AVStreams::StreamCtrl_ptr streamctrl);

private:
  /// Keeping a count of the incoming frames.
  std::size_t frame_count_ {};

  /// Related stream control.
  AVStreams::StreamCtrl_ptr streamctrl_ {};
};

/// AVStreams calls this class during connection setup.
class Receiver_StreamEndPoint : public TAO_Server_StreamEndPoint
{
public:
  Receiver_StreamEndPoint () = default;
  Receiver_StreamEndPoint (Receiver_StreamEndPoint const &) = delete;
  Receiver_StreamEndPoint &operator= (Receiver_StreamEndPoint const &) = delete;
  Receiver_StreamEndPoint (Receiver_StreamEndPoint &&) = delete;
  Receiver_StreamEndPoint &operator= (Receiver_StreamEndPoint &&) = delete;

  /// Create a receiver application callback.
  int get_callback (const char *flowname,
                    TAO_AV_Callback *&callback) override;

private:
  /// Receiver application callback.
  Receiver_Callback callback_;
};

/// This class receives data from a AV sender and writes it to a file.
class Receiver
{
public:
  Receiver () = default;

  Receiver (Receiver const &) = delete;
  Receiver &operator= (Receiver const &) = delete;
  Receiver (Receiver &&) = delete;
  Receiver &operator= (Receiver &&) = delete;

  /// Initialize data components.
  int init (int argc,
            ACE_TCHAR *argv[]);

protected:
  /// The Naming Service Client.
  TAO_Naming_Client naming_client_;

  /// The endpoint reactive strategy.
  TAO_AV_Endpoint_Reactive_Strategy_B
  <Receiver_StreamEndPoint,TAO_VDev,AV_Null_MediaCtrl> reactive_strategy_;

  /// Receiver MMDevice.
  TAO_MMDevice *mmdevice_ {};
};

#endif /* TAO_ORBsvcs_AVSTREAMS_TESTS_MODIFY_QOS_RECEIVER_H */
