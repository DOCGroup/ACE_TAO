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

#ifndef TAO_AV_COMPONENT_SWITCHING_DISTRIBUTER_H
#define TAO_AV_COMPONENT_SWITCHING_DISTRIBUTER_H

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

  /// Receive and forward a frame from the sender.
  int receive_frame (ACE_Message_Block *frame,
                     TAO_AV_frame_info *frame_info,
                     ACE_Addr const &peer_address) override;

  /// Handle stream shutdown requested by the sender.
  int handle_destroy () override;

  /// Accessor methods for the flowname of the callback.
  ACE_CString &flowname ();
  void flowname (ACE_CString const &flowname);

private:
  /// Number of frames passed to receivers.
  std::size_t frame_count_ {1};

  /// Flow name associated with this callback.
  ACE_CString flowname_;
};

/**
 * @class Distributer_Sender_Callback
 *
 * @brief Defines a class for the distributer application callback
 * for receiving data.
 *
 * This class overrides the methods of the TAO_AV_Callback so the
 * AVStreams can make upcalls to the application.
 */
class Distributer_Sender_Callback : public TAO_AV_Callback
{
public:
  Distributer_Sender_Callback () = default;
  Distributer_Sender_Callback (Distributer_Sender_Callback const &) = delete;
  Distributer_Sender_Callback &operator= (Distributer_Sender_Callback const &) = delete;
  Distributer_Sender_Callback (Distributer_Sender_Callback &&) = delete;
  Distributer_Sender_Callback &operator= (Distributer_Sender_Callback &&) = delete;

  /// Handle sender-initiated stream shutdown.
  int handle_destroy () override;

  /// Accessor methods for the flowname of the callback.
  ACE_CString &flowname ();
  void flowname (ACE_CString const &flowname);

private:
  ACE_CString flowname_;
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

  /// Return the receiver callback for AVStreams upcalls.
  int get_callback (char const *flowname,
                    TAO_AV_Callback *&callback) override;

  /// Store the protocol object selected for the transport.
  int set_protocol_object (char const *,
                           TAO_AV_Protocol_Object *object) override;

  /// Validate a connection request from a sender.
  CORBA::Boolean handle_connection_requested (AVStreams::flowSpec &the_spec) override;

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

  /// Return the sender callback for AVStreams upcalls.
  int get_callback (char const *flowname,
                    TAO_AV_Callback *&callback) override;

  /// Set the protocol object selected for the transport.
  int set_protocol_object (char const *flowname,
                           TAO_AV_Protocol_Object *object) override;

protected:
  /// Application callback.
  Distributer_Sender_Callback callback_;
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

  /// Initialize distributor state and establish its streams.
  int init (int argc,
            ACE_TCHAR **argv);

  /// Parse command-line arguments.
  int parse_args (int argc,
                  ACE_TCHAR **argv);

  /// Return whether all active streams have been destroyed.
  bool done () const;
  /// Set whether the distributor has finished processing.
  void done (bool);

  /// Access the connection manager.
  Connection_Manager &connection_manager ();

  /// Record creation of a stream.
  void stream_created ();

  /// Record destruction of a stream.
  void stream_destroyed ();

  /// Unbind the sender and receiver from the Naming Service.
  void shut_down ();

protected:
  /// Connection manager.
  Connection_Manager connection_manager_;

  /// Sender endpoint strategy.
  SENDER_ENDPOINT_STRATEGY sender_endpoint_strategy_;

  /// Receiver endpoint strategy.
  RECEIVER_ENDPOINT_STRATEGY receiver_endpoint_strategy_;

  /// Non-owning pointer to the POA-managed receiver MMDevice servant.
  TAO_MMDevice *distributer_receiver_mmdevice_ {};

  /// Non-owning pointer to the POA-managed sender MMDevice servant.
  TAO_MMDevice *distributer_sender_mmdevice_ {};

  /// Sender and distributor names in the Naming Service.
  struct Options
  {
    std::string sender_name {"sender"};
    std::string distributer_name {"distributer"};
  };

  Options options_;

  /// Whether all streams have been destroyed.
  bool done_ {};

  /// Number of active streams.
  int stream_count_ {};

  /// Signal handler registered with the reactor.
  Signal_Handler signal_handler_;
};

#endif /* TAO_AV_COMPONENT_SWITCHING_DISTRIBUTER_H */
