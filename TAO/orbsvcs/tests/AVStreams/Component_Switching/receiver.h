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

#ifndef TAO_AV_COMPONENT_SWITCHING_RECEIVER_H
#define TAO_AV_COMPONENT_SWITCHING_RECEIVER_H

#include <cstddef>
#include <string>

#include "Connection_Manager.h"
#include "orbsvcs/AV/AVStreams_i.h"
#include "orbsvcs/AV/Endpoint_Strategy.h"
#include "orbsvcs/AV/Policy.h"

/**
 * @class Signal_Handler
 * @brief Handles SIGINT through the Reactor to shut down gracefully.
 */
class Signal_Handler : public ACE_Event_Handler
{
public:
  Signal_Handler () = default;
  Signal_Handler (Signal_Handler const &) = delete;
  Signal_Handler &operator= (Signal_Handler const &) = delete;
  Signal_Handler (Signal_Handler &&) = delete;
  Signal_Handler &operator= (Signal_Handler &&) = delete;

  /// Override this method to implement graceful shutdown.
  int handle_signal (int signum, siginfo_t *, ucontext_t *) override;
};

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

  /// Receive a frame from a sender.
  int receive_frame (ACE_Message_Block *frame,
                     TAO_AV_frame_info *frame_info,
                     ACE_Addr const &peer_address) override;

  int handle_destroy () override;

  /// Accessor methods for the flowname of the callback.
  ACE_CString &flowname ();
  void flowname (ACE_CString const &flowname);

private:
  /// Number of received frames.
  std::size_t frame_count_ {1};

  /// Flow name associated with this callback.
  ACE_CString flowname_;
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

  /// Validate a connection request from a distributor.
  CORBA::Boolean handle_connection_requested (AVStreams::flowSpec &the_spec) override;

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

  /// Initialize receiver state and establish its stream.
  int init (int argc,
            ACE_TCHAR **argv);

  /// Parse command-line arguments.
  int parse_args (int argc,
                  ACE_TCHAR **argv);

  /// Return the output file name.
  std::string const &output_file_name () const;

  void shut_down ();

protected:
  /// Connection manager.
  Connection_Manager connection_manager_;

  /// Endpoint reactive strategy.
  TAO_AV_Endpoint_Reactive_Strategy_B
  <Receiver_StreamEndPoint,TAO_VDev,AV_Null_MediaCtrl> reactive_strategy_;

  /// Non-owning pointer to the POA-managed receiver MMDevice servant.
  TAO_MMDevice *mmdevice_ {};

  /// Output file and Naming Service names.
  struct Options
  {
    std::string output_file_name {"output"};
    std::string sender_name {"distributer"};
    std::string receiver_name {"receiver"};
  };

  Options options_;

  /// Signal handler registered with the reactor.
  Signal_Handler signal_handler_;
};

#endif /* TAO_AV_COMPONENT_SWITCHING_RECEIVER_H */
