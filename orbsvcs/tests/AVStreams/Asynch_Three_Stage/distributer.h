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
 * @class Distributer_Receiver_Callback
 *
 * @brief Application defined callback object.
 *
 * AVStreams calls this class when data shows up from a sender.
 */
class Distributer_Receiver_Callback : public TAO_AV_Callback
{
public:

  /// Constructor.
  Distributer_Receiver_Callback (void);

  // Method that is called when there is data to be received from a
  // sender.
  int receive_frame (ACE_Message_Block *frame,
                     TAO_AV_frame_info *frame_info,
                     const ACE_Addr &peer_address);

  // Called when the sender is done sending data and wants to close
  // down the connection.
  int handle_destroy (void);

private:
  /// Count of the frames passing through us.
  int frame_count_;
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
  // Create a receiver application callback.
  int get_callback (const char *flowname,
                    TAO_AV_Callback *&callback);

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
  /// Create the application callback and return its handle to
  /// AVStreams for further application callbacks.
  int get_callback (const char *flowname,
                    TAO_AV_Callback *&callback);

  /// Set protocol object corresponding to the transport protocol
  /// chosen.
  int set_protocol_object (const char *flowname,
                           TAO_AV_Protocol_Object *object);

protected:
  /// Application callback.
  TAO_AV_Callback callback_;
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
  /// Constructor
  Distributer (void);

  /// Destructor.
  ~Distributer (void);

  /// Initialize data components.
  int init (int argc, ACE_TCHAR *argv[]);

  /// Parse args.
  int parse_args (int argc, ACE_TCHAR *argv[]);

  // Flag to know when we are done.
  int done (void) const;
  void done (int);

  /// Accessor to connection manager.
  Connection_Manager &connection_manager (void);

protected:
  /// Connection manager.
  Connection_Manager connection_manager_;

  /// The sender endpoint strategy.
  SENDER_ENDPOINT_STRATEGY sender_endpoint_strategy_;

  /// The receiver endpoint strategy.
  RECEIVER_ENDPOINT_STRATEGY receiver_endpoint_strategy_;

  /// The distributer receiver multimedia device
  TAO_MMDevice* distributer_receiver_mmdevice_;

  /// The distributer receiver multimedia device
  TAO_MMDevice* distributer_sender_mmdevice_;

  /// The name of the sender to connect to.
  ACE_CString sender_name_;

  /// Our name.
  ACE_CString distributer_name_;

  /// Flag to know when we are done.
  int done_;

  ACE_TString addr_file_;
};
