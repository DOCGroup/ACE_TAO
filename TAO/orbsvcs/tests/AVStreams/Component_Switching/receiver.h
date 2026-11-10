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

#include <cstddef>

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

  /// Method that is called when there is data to be received from a
  /// sender.
  int receive_frame (ACE_Message_Block *frame,
                     TAO_AV_frame_info *frame_info,
                     ACE_Addr const &peer_address) override;

  int handle_destroy () override;

  /// Accessor methods for the flowname of the callback.
  ACE_CString &flowname ();
  void flowname (ACE_CString const &flowname);

private:
  //// Keeping a count of the incoming frames.
  std::size_t frame_count_ {1};

  //// Flowname of the flow.
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

  /// Create a receiver application callback.
  int get_callback (char const *flowname,
                    TAO_AV_Callback *&callback) override;

  //// Called when a distributor tries to connect to the receiver
  CORBA::Boolean handle_connection_requested (AVStreams::flowSpec &the_spec) override;

private:
  //// Receiver application callback.
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
  ~Receiver () = default;
  Receiver (Receiver const &) = delete;
  Receiver &operator= (Receiver const &) = delete;
  Receiver (Receiver &&) = delete;
  Receiver &operator= (Receiver &&) = delete;

  //// Initialize data components.
  int init (int argc,
            ACE_TCHAR **argv);

  //// Parse args.
  int parse_args (int argc,
                  ACE_TCHAR **argv);

  //// Name of the output file.
  ACE_CString const &output_file_name () const;

  void shut_down ();

protected:
  //// Connection manager.
  Connection_Manager connection_manager_;

  //// The endpoint reactive strategy.
  TAO_AV_Endpoint_Reactive_Strategy_B
  <Receiver_StreamEndPoint,TAO_VDev,AV_Null_MediaCtrl> reactive_strategy_;

  //// Non-owning pointer to the POA-managed receiver MMDevice servant.
  TAO_MMDevice *mmdevice_ {};

  //// File name of the file into which received data is written.
  ACE_CString output_file_name_ {"output"};

  //// Sender name.
  ACE_CString sender_name_ {"distributer"};

  //// Receiver name.
  ACE_CString receiver_name_ {"receiver"};

  //// Reference to the signal handler.
  Signal_Handler signal_handler_;
};
