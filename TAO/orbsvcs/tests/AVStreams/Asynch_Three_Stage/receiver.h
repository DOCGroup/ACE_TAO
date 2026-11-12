/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    receiver.h
 *
 *  This application receives data from a AV sender and writes it to
 *  a file.
 *
 *  @author Yamuna Krishnamurthy <yamuna@cs.wustl.edu>
 */
//=============================================================================

#ifndef TAO_AVSTREAMS_ASYNCH_THREE_STAGE_RECEIVER_H
#define TAO_AVSTREAMS_ASYNCH_THREE_STAGE_RECEIVER_H

#include "Connection_Manager.h"
#include "orbsvcs/AV/AVStreams_i.h"
#include "orbsvcs/AV/Endpoint_Strategy.h"
#include "orbsvcs/AV/Policy.h"

/**
 * @class Receiver_Callback
 *
 * @brief Application defined callback object.
 *
 * AVStreams calls this class when data shows up from a sender.
 */
class Receiver_Callback : public TAO_AV_Callback
{
public:
  Receiver_Callback () = default;
  Receiver_Callback (Receiver_Callback const &) = delete;
  Receiver_Callback &operator= (Receiver_Callback const &) = delete;
  Receiver_Callback (Receiver_Callback &&) = delete;
  Receiver_Callback &operator= (Receiver_Callback &&) = delete;

  /// Receive a frame sent by the sender.
  int receive_frame (ACE_Message_Block *frame,
                     TAO_AV_frame_info *frame_info,
                     ACE_Addr const &peer_address) override;

  /// Close down the connection when the sender finishes.
  int handle_destroy () override;

private:
  /// Keeping a count of the incoming frames.
  int frame_count_ {1};
};

/**
 * @class Receiver_StreamEndPoint
 *
 * @brief Application defined stream endpoint object.
 *
 * AVStreams calls this class during connection setup.
 */
class Receiver_StreamEndPoint : public TAO_Server_StreamEndPoint
{
public:
  Receiver_StreamEndPoint () = default;
  Receiver_StreamEndPoint (Receiver_StreamEndPoint const &) = delete;
  Receiver_StreamEndPoint &operator= (Receiver_StreamEndPoint const &) = delete;
  Receiver_StreamEndPoint (Receiver_StreamEndPoint &&) = delete;
  Receiver_StreamEndPoint &operator= (Receiver_StreamEndPoint &&) = delete;

  /// Return the receiver's application callback.
  int get_callback (char const *flowname,
                    TAO_AV_Callback *&callback) override;

private:
  /// Receiver application callback.
  Receiver_Callback callback_;
};

/**
 * @class Receiver
 *
 * @brief Receiver application class.
 *
 * This class receives data from a AV sender and writes it to
 * a file.
 */
class Receiver
{
public:
  Receiver () = default;
  Receiver (Receiver const &) = delete;
  Receiver &operator= (Receiver const &) = delete;
  Receiver (Receiver &&) = delete;
  Receiver &operator= (Receiver &&) = delete;

  /// Initialize data components.
  int init ();

  /// Parse args.
  int parse_args (int argc, ACE_TCHAR *argv[]);

  /// Name of the output file.
  ACE_TString const &output_file_name () const;

protected:
  /// Connection manager.
  Connection_Manager connection_manager_;

  /// The endpoint reactive strategy.
  TAO_AV_Endpoint_Reactive_Strategy_B
  <Receiver_StreamEndPoint,TAO_VDev,AV_Null_MediaCtrl> reactive_strategy_;

  struct Options
  {
    ACE_TString output_file_name {ACE_TEXT ("output")};
    ACE_TString addr_file {ACE_TEXT ("addr_file")};
    ACE_CString sender_name {"distributer"};
    ACE_CString receiver_name {"receiver"};
  };

  Options options_;
};

#endif /* TAO_AVSTREAMS_ASYNCH_THREE_STAGE_RECEIVER_H */
