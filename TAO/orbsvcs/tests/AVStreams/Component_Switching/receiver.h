/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/orbsvcs/tests/AVStreams/Component_Switching
//
// = FILENAME
//    receiver.h
//
// = DESCRIPTION
//    This application receives data from a AV sender and writes it to
//    a file.
//
// = AUTHOR
//    Yamuna Krishnamurthy <yamuna@cs.wustl.edu>
//
// ============================================================================

#include "Connection_Manager.h"
#include "orbsvcs/AV/AVStreams_i.h"
#include "orbsvcs/AV/Endpoint_Strategy.h"
#include "orbsvcs/AV/Policy.h"

class Signal_Handler : public ACE_Event_Handler
{
  // TITLE
  //   This class Handles the SIGINT signal through the Reactor.
  //   Useful to gracefully release the process

public:

  Signal_Handler (void);

  int handle_signal(int signum, siginfo_t*,ucontext_t*);
  // Override this method to implement graceful shutdown.

};

class Receiver_Callback : public TAO_AV_Callback
{
  // = TITLE
  //    Application defined callback object.
  //
  // = DESCRIPTION
  //    AVStreams calls this class when data shows up from a sender.
public:

  Receiver_Callback (void);
  /// Constructor.

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
  int frame_count_;
  /// Keeping a count of the incoming frames.

  ACE_CString flowname_;
  /// Flowname of the flow.
};

class Receiver_StreamEndPoint : public TAO_Server_StreamEndPoint
{
  // = TITLE
  //    Application defined stream endpoint object.
  //
  // = DESCRIPTION
  //    AVStreams calls this class during connection setup.
public:
  /// Create a receiver application callback.
  int get_callback (const char *flowname,
                    TAO_AV_Callback *&callback);

  virtual CORBA::Boolean handle_connection_requested (AVStreams::flowSpec &the_spec
                                                      TAO_ENV_ARG_DECL_NOT_USED);
  /// Called when a distributor tries to connect to the receiver

private:
  Receiver_Callback callback_;
  /// Receiver application callback.
};

class Receiver
{
  // = TITLE
  //    Receiver application class.
  //
  // = DESCRIPTION
  //    This class receives data from a AV sender and writes it to
  //    a file.
public:
  Receiver (void);
  /// Constructor

  virtual ~Receiver (void);
  /// Destructor.

  int init (int argc,
            char **argv
            TAO_ENV_ARG_DECL_NOT_USED);
  /// Initialize data components.

  int parse_args (int argc,
                  char **argv);
  /// Parse args.

  ACE_CString output_file_name (void);
  /// Name of the output file.

  ACE_CString sender_name (void);
  ACE_CString receiver_name (void);

  void shut_down (TAO_ENV_SINGLE_ARG_DECL_NOT_USED);

protected:
  Connection_Manager connection_manager_;
  /// Connection manager.

  TAO_AV_Endpoint_Reactive_Strategy_B
  <Receiver_StreamEndPoint,TAO_VDev,AV_Null_MediaCtrl> reactive_strategy_;
  /// The endpoint reactive strategy.

  AVStreams::MMDevice_var mmdevice_obj_;
  /// The receiver MMDevice.

  TAO_MMDevice *mmdevice_;
  /// Receiver MMDevice.

  AVStreams::MMDevice_var sender_mmdevice_;
  /// The sender MMDevice

  ACE_CString output_file_name_;
  /// File name of the file into which received data is written.

  ACE_CString sender_name_;
  /// Sender name.

  ACE_CString receiver_name_;
  /// Receiver name.

  Signal_Handler signal_handler_;
  /// Reference to the signal handler.
};
