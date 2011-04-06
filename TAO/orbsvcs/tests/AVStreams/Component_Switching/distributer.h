/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    distributer.h
 *
 *  $Id$
 *
 *  Process to receive data from the sender and send it to the
 *  receiver.
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
 * @class Distributer_Receiver_Callback
 *
 * @brief Application defined callback object.
 *
 * AVStreams calls this class when data shows up from a sender.
 */
class Distributer_Receiver_Callback : public TAO_AV_Callback
{
public:

  //// Constructor.
  Distributer_Receiver_Callback (void);

  /// Method that is called when there is data to be received from a
  /// sender.
  int receive_frame (ACE_Message_Block *frame,
                     TAO_AV_frame_info *frame_info,
                     const ACE_Addr &peer_address);

  /// Called when the sender is done sending data and wants to close
  /// down the connection.
  int handle_destroy (void);

  /// Accessor methods for the flowname of the callback.
  ACE_CString &flowname (void);
  void flowname (const ACE_CString &flowname);

private:
  //// Count of the frames passing through us.
  int frame_count_;

  //// Flowname of the flow.
  ACE_CString flowname_;
};

/**
 * @class Distributer_Sender_Callback
 *
 * @brief Defines a class for the distributer application callback
 * for receiving data.
 *
 * This class overides the methods of the TAO_AV_Callback so the
 * AVStreams can make upcalls to the application.
 */
class Distributer_Sender_Callback : public TAO_AV_Callback
{

public:

  //// Called when the sender has finished reading the file and wants
  //// to close down the connection.
  int handle_destroy (void);

  /// Accessor methods for the flowname of the callback.
  ACE_CString &flowname (void);
  void flowname (const ACE_CString &flowname);

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
  /// Create a receiver application callback.
  int get_callback (const char *flowname,
                    TAO_AV_Callback *&callback);

  //// Store the reference to the protocol object corresponding
  //// to the transport
  int set_protocol_object (const char *,
                           TAO_AV_Protocol_Object *object);

  //// Called when a sender makes a connection request.
  virtual CORBA::Boolean handle_connection_requested (AVStreams::flowSpec &the_spec);

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
  //// Create the application callback and return its handle to
  //// AVStreams for further application callbacks.
  int get_callback (const char *flowname,
                    TAO_AV_Callback *&callback);

  //// Set protocol object corresponding to the transport protocol
  //// chosen.
  int set_protocol_object (const char *flowname,
                           TAO_AV_Protocol_Object *object);

protected:
  //// Application callback.
  Distributer_Sender_Callback callback_;
};

typedef TAO_AV_Endpoint_Reactive_Strategy_A
          <Distributer_Sender_StreamEndPoint,
           TAO_VDev,
           AV_Null_MediaCtrl>
        SENDER_ENDPOINT_STRATEGY;

typedef TAO_AV_Endpoint_Reactive_Strategy_B
          <Distributer_Receiver_StreamEndPoint,
           TAO_VDev,
           AV_Null_MediaCtrl>
        RECEIVER_ENDPOINT_STRATEGY;

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
  //// Constructor
  Distributer (void);

  //// Destructor.
  ~Distributer (void);

  //// Initialize data components.
  int init (int argc,
            ACE_TCHAR **argv);

  //// Parse args.
  int parse_args (int argc,
                  ACE_TCHAR **argv);

  /// Flag to know when we are done.
  bool done (void) const;
  void done (bool);

  //// Accessor to connection manager.
  Connection_Manager &connection_manager (void);

  //// Called when stream created
  void stream_created (void);

  //// Called when stream destroyed
  void stream_destroyed (void);

  ////Unbind the sender and receiver from the Naming Service
  void shut_down (void);

protected:
  //// Connection manager.
  Connection_Manager connection_manager_;

  //// The sender endpoint strategy.
  SENDER_ENDPOINT_STRATEGY sender_endpoint_strategy_;

  //// The receiver endpoint strategy.
  RECEIVER_ENDPOINT_STRATEGY receiver_endpoint_strategy_;

  //// The distributer receiver multimedia device
  TAO_MMDevice* distributer_receiver_mmdevice_;

  //// The distributer receiver multimedia device
  TAO_MMDevice* distributer_sender_mmdevice_;

  //// The name of the sender to connect to.
  ACE_CString sender_name_;

  //// Our name.
  ACE_CString distributer_name_;

  //// Flag to know when we are done.
  bool done_;

  //// Number of active streams. When a stream is disconnected this
  //// count is decremented.
  int stream_count_;

  //// Reference to the signal handler.
  Signal_Handler signal_handler_;
};
