/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    receiver.h
 *
 *  $Id$
 *
 *  This application receives data from a AV sender and writes it to
 *  a file.
 *
 *
 *  @author Yamuna Krishnamurthy <yamuna@cs.wustl.edu>
 */
//=============================================================================


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

  Signal_Handler (void);

  /// Override this method to implement graceful shutdown.
  int handle_signal(int signum, siginfo_t*,ucontext_t*);

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

  //// Constructor.
  Receiver_Callback (void);

  /// Method that is called when there is data to be received from a
  /// sender.
  int receive_frame (ACE_Message_Block *frame,
                     TAO_AV_frame_info *frame_info,
                     const ACE_Addr &peer_address);

  int handle_destroy (void);

  /// Accessor methods for the flowname of the callback.
  ACE_CString &flowname (void);
  void flowname (const ACE_CString &flowname);

private:
  //// Keeping a count of the incoming frames.
  int frame_count_;

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
  /// Create a receiver application callback.
  int get_callback (const char *flowname,
                    TAO_AV_Callback *&callback);

  //// Called when a distributor tries to connect to the receiver
  virtual CORBA::Boolean handle_connection_requested (AVStreams::flowSpec &the_spec);

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
  //// Constructor
  Receiver (void);

  //// Destructor.
  virtual ~Receiver (void);

  //// Initialize data components.
  int init (int argc,
            ACE_TCHAR **argv);

  //// Parse args.
  int parse_args (int argc,
                  ACE_TCHAR **argv);

  //// Name of the output file.
  ACE_CString output_file_name (void);

  ACE_CString sender_name (void);
  ACE_CString receiver_name (void);

  void shut_down (void);

protected:
  //// Connection manager.
  Connection_Manager connection_manager_;

  //// The endpoint reactive strategy.
  TAO_AV_Endpoint_Reactive_Strategy_B
  <Receiver_StreamEndPoint,TAO_VDev,AV_Null_MediaCtrl> reactive_strategy_;

  //// The receiver MMDevice.
  AVStreams::MMDevice_var mmdevice_obj_;

  //// Receiver MMDevice.
  TAO_MMDevice *mmdevice_;

  //// The sender MMDevice
  AVStreams::MMDevice_var sender_mmdevice_;

  //// File name of the file into which received data is written.
  ACE_CString output_file_name_;

  //// Sender name.
  ACE_CString sender_name_;

  //// Receiver name.
  ACE_CString receiver_name_;

  //// Reference to the signal handler.
  Signal_Handler signal_handler_;
};
