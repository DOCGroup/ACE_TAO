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

#include <cstddef>
#include <string>

#include "Connection_Manager.h"
#include "orbsvcs/AV/AVStreams_i.h"
#include "orbsvcs/AV/Endpoint_Strategy.h"
#include "orbsvcs/AV/Policy.h"

/**
 * @class Signal_Handler
 TITLE
 * This class Handles the SIGINT signal through the Reactor.
 * Useful to gracefully release the process
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

  /// Method that is called when there is data to be received from a
  /// sender.
  int receive_frame (ACE_Message_Block *frame,
                     TAO_AV_frame_info *frame_info,
                     ACE_Addr const &peer_address) override;

  /// Called when the sender is done sending data and wants to close
  /// down the connection.
  int handle_destroy () override;

  /// Accessor methods for the flowname of the callback.
  ACE_CString &flowname ();
  void flowname (ACE_CString const &flowname);

private:
  //// Count of the frames passing through us.
  std::size_t frame_count_ {1};

  //// Flowname of the flow.
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

  //// Called when the sender has finished reading the file and wants
  //// to close down the connection.
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

  /// Create a receiver application callback.
  int get_callback (char const *flowname,
                    TAO_AV_Callback *&callback) override;

  //// Store the reference to the protocol object corresponding
  //// to the transport
  int set_protocol_object (char const *,
                           TAO_AV_Protocol_Object *object) override;

  //// Called when a sender makes a connection request.
  CORBA::Boolean handle_connection_requested (AVStreams::flowSpec &the_spec) override;

private:
  //// Receiver application callback.
  Distributer_Receiver_Callback callback_;
};

/**
 * @class Distributer_Sender_StreamEndPoint
 *
 * @brief /    Defines a sender stream endpoint.
 */
class Distributer_Sender_StreamEndPoint : public TAO_Client_StreamEndPoint
{
public:
  Distributer_Sender_StreamEndPoint () = default;
  Distributer_Sender_StreamEndPoint (Distributer_Sender_StreamEndPoint const &) = delete;
  Distributer_Sender_StreamEndPoint &operator= (Distributer_Sender_StreamEndPoint const &) = delete;
  Distributer_Sender_StreamEndPoint (Distributer_Sender_StreamEndPoint &&) = delete;
  Distributer_Sender_StreamEndPoint &operator= (Distributer_Sender_StreamEndPoint &&) = delete;

  //// Create the application callback and return its handle to
  //// AVStreams for further application callbacks.
  int get_callback (char const *flowname,
                    TAO_AV_Callback *&callback) override;

  //// Set protocol object corresponding to the transport protocol
  //// chosen.
  int set_protocol_object (char const *flowname,
                           TAO_AV_Protocol_Object *object) override;

protected:
  //// Application callback.
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

  //// Initialize data components.
  int init (int argc,
            ACE_TCHAR **argv);

  //// Parse args.
  int parse_args (int argc,
                  ACE_TCHAR **argv);

  /// Flag to know when we are done.
  bool done () const;
  void done (bool);

  //// Accessor to connection manager.
  Connection_Manager &connection_manager ();

  //// Called when stream created
  void stream_created ();

  //// Called when stream destroyed
  void stream_destroyed ();

  ////Unbind the sender and receiver from the Naming Service
  void shut_down ();

protected:
  //// Connection manager.
  Connection_Manager connection_manager_;

  //// The sender endpoint strategy.
  SENDER_ENDPOINT_STRATEGY sender_endpoint_strategy_;

  //// The receiver endpoint strategy.
  RECEIVER_ENDPOINT_STRATEGY receiver_endpoint_strategy_;

  //// Non-owning pointer to the POA-managed receiver MMDevice servant.
  TAO_MMDevice *distributer_receiver_mmdevice_ {};

  //// Non-owning pointer to the POA-managed sender MMDevice servant.
  TAO_MMDevice *distributer_sender_mmdevice_ {};

  //// The name of the sender to connect to.
  struct Options
  {
    std::string sender_name {"sender"};
    std::string distributer_name {"distributer"};
  };

  Options options_;

  //// Flag to know when we are done.
  bool done_ {};

  //// Number of active streams. When a stream is disconnected this
  //// count is decremented.
  int stream_count_ {};

  //// Reference to the signal handler.
  Signal_Handler signal_handler_;
};
