/* -*- C++ -*- */

#ifndef TAO_AV_BIDIRECTIONAL_FLOWS_RECEIVER_H
#define TAO_AV_BIDIRECTIONAL_FLOWS_RECEIVER_H

#include <cstdio>
#include <memory>
#include <string>

#include "orbsvcs/Naming/Naming_Client.h"
#include "orbsvcs/AV/AVStreams_i.h"
#include "orbsvcs/AV/Endpoint_Strategy.h"
#include "orbsvcs/AV/Policy.h"
#include "orbsvcs/AV/Protocol_Factory.h"

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

  /// Receive a frame from the sender and return the next frame.
  int receive_frame (ACE_Message_Block *frame,
                     TAO_AV_frame_info *frame_info,
                     ACE_Addr const &peer_address) override;

  /// Handle stream shutdown requested by the sender.
  int handle_destroy () override;

private:
  /// Keeping a count of the incoming frames.
  int frame_count_ {1};

  /// Message block into which data is read from a file and then sent.
  ACE_Message_Block mb_ {BUFSIZ};
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

  /// Return the receiver callback for AVStreams upcalls.
  int get_callback (char const *flowname,
                    TAO_AV_Callback *&callback) override;

  /// Set the protocol object selected for the transport.
  int set_protocol_object (char const *flowname,
                           TAO_AV_Protocol_Object *object) override;

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
  int init (int argc,
            ACE_TCHAR *argv[]);

  /// Set the protocol object corresponding to the selected transport protocol.
  void protocol_object (TAO_AV_Protocol_Object *protocol_object);

  /// Get the protocol object corresponding to the selected transport protocol.
  TAO_AV_Protocol_Object *protocol_object () const;

  /// Return the input file owned by this receiver.
  FILE *input_file () const;

protected:
  /// The Naming Service Client.
  TAO_Naming_Client naming_client_;

  /// The endpoint reactive strategy.
  TAO_AV_Endpoint_Reactive_Strategy_B
  <Receiver_StreamEndPoint,TAO_VDev,AV_Null_MediaCtrl> reactive_strategy_;

  /// Borrowed from AVStreams transport setup; this object does not own it.
  TAO_AV_Protocol_Object *protocol_object_ {};

  using File_Ptr = std::unique_ptr<FILE, int (*) (FILE *)>;
  File_Ptr input_file_ {nullptr, std::fclose};

  struct Options
  {
    std::string filename {"input"};
  };

  Options options_;

  /// Method to parse the command line arguments.
  int parse_args (int argc, ACE_TCHAR *argv[]);
};

#endif /* TAO_AV_BIDIRECTIONAL_FLOWS_RECEIVER_H */
