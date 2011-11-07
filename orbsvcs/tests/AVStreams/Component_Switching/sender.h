/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    sender.h
 *
 *  $Id$
 *
 *  This application reads data from a file and sends it to s
 *  receiver.
 *
 *
 *  @author Yamuna Krishnamurthy <yamuna@cs.wustl.edu>
 */
//=============================================================================


#include "Connection_Manager.h"
#include "orbsvcs/AV/AVStreams_i.h"
#include "orbsvcs/AV/Endpoint_Strategy.h"
#include "orbsvcs/AV/Protocol_Factory.h"

/**
 * @class Signal_Handler
 *
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
 * @class Sender_Callback
 *
 * @brief Defines a class for the sender application callback.
 *
 * This class overides the methods of the TAO_AV_Callback so the
 * AVStreams can make upcalls to the application.
 */
class Sender_Callback : public TAO_AV_Callback
{

public:

  /**
   * Called when the sender has finished reading the file and wants
   * to close down the connection. Also called when the distributer
   * tears down the connection when it switches to a new sender.
   */
  int handle_destroy (void);

  /// Accessor methods for the flowname of the callback
  ACE_CString &flowname (void);
  void flowname (const ACE_CString &flowname);

private:
  //// Flowname of the callback.
  ACE_CString flowname_;
};

/**
 * @class Sender_StreamEndPoint
 *
 * @brief Defines a sender stream endpoint.
 */
class Sender_StreamEndPoint : public TAO_Client_StreamEndPoint
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

  //// Perform application specific actions before accepting new
  //// connections.
  CORBA::Boolean handle_preconnect (AVStreams::flowSpec &flowspec);

protected:
  //// Application callback.
  Sender_Callback callback_;
};

typedef TAO_AV_Endpoint_Reactive_Strategy_A
          <Sender_StreamEndPoint,
           TAO_VDev,
           AV_Null_MediaCtrl>
        SENDER_ENDPOINT_STRATEGY;

/**
 * @class Sender
 *
 * @brief Sender Application.
 *
 * Class is responsible for streaming (and pacing) data to a
 * receiver.
 */
class Sender
{
public:
  //// Constructor
  Sender (void);

  //// Destructor
  ~Sender (void);

  void shut_down (void);

  //// Method to initialize the various data components.
  int init (int argc,
            ACE_TCHAR *argv[]);

  //// Method to pace and send data from a file.
  int pace_data (void);

  //// Accessor to the connection manager.
  Connection_Manager &connection_manager (void);

private:
  //// Method to parse the command line arguments.
  int parse_args (int argc, ACE_TCHAR *argv[]);

  //// The endpoint strategy used by the sender.
  SENDER_ENDPOINT_STRATEGY endpoint_strategy_;

  //// The sender MMDevice.
  TAO_MMDevice *sender_mmdevice_;

  //// Number of frames sent.
  int frame_count_;

  //// File from which data is read.
  ACE_CString filename_;

  //// File handle of the file read from.
  FILE *input_file_;

  //// Rate at which the data will be sent.
  double frame_rate_;

  //// Message block into which data is read from a file and then sent.
  ACE_Message_Block mb_;

  //// Name of this sender.
  ACE_CString sender_name_;

  //// Connection manager.
  Connection_Manager connection_manager_;

  //// Reference to the signal handler.
  Signal_Handler signal_handler_;
};
