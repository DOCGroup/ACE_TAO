/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/orbsvcs/tests/AVStreams/Asynch_Three_Stage
//
// = FILENAME
//    distributer.h
//
// = DESCRIPTION
//    Process to receive data from the sender and send it to the
//    receiver.
//
// = AUTHOR
//    Yamuna Krishnamurthy <yamuna@cs.wustl.edu>
//
// ============================================================================

#include "Connection_Manager.h"
#include "orbsvcs/AV/AVStreams_i.h"
#include "orbsvcs/AV/Endpoint_Strategy.h"
#include "orbsvcs/AV/Policy.h"

class Distributer_Receiver_Callback : public TAO_AV_Callback
{
  // = TITLE
  //    Application defined callback object.
  //
  // = DESCRIPTION
  //    AVStreams calls this class when data shows up from a sender.
public:

  Distributer_Receiver_Callback (void);
  // Constructor.

  // Method that is called when there is data to be received from a
  // sender.
  int receive_frame (ACE_Message_Block *frame,
                     TAO_AV_frame_info *frame_info,
                     const ACE_Addr &peer_address);

  // Called when the sender is done sending data and wants to close
  // down the connection.
  int handle_destroy (void);

private:
  int frame_count_;
  // Count of the frames passing through us.
};

class Distributer_Receiver_StreamEndPoint : public TAO_Server_StreamEndPoint
{
  // = TITLE
  //    Application defined stream endpoint object.
  //
  // = DESCRIPTION
  //    AVStreams calls this class during connection setup.
public:
  // Create a receiver application callback.
  int get_callback (const char *flowname,
                    TAO_AV_Callback *&callback);

private:
  Distributer_Receiver_Callback callback_;
  // Receiver application callback.
};

class Distributer_Sender_StreamEndPoint : public TAO_Client_StreamEndPoint
{
  // = TITLE
  //    Defines a sender stream endpoint.
public:
  int get_callback (const char *flowname,
                    TAO_AV_Callback *&callback);
  // Create the application callback and return its handle to
  // AVStreams for further application callbacks.

  int set_protocol_object (const char *flowname,
                           TAO_AV_Protocol_Object *object);
  // Set protocol object corresponding to the transport protocol
  // chosen.

protected:
  TAO_AV_Callback callback_;
  // Application callback.
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

class Distributer
{
  // = TITLE
  //    Distributer Application.
  //
  // = DESCRIPTION
  //    The distributer is the intermediate receiver that receives
  //    data from the sender and forwards to a receiver.
public:
  Distributer (void);
  // Constructor

  ~Distributer (void);
  // Destructor.

  int init (int argc,
            char **argv
            TAO_ENV_ARG_DECL_NOT_USED);
  // Initialize data components.

  int parse_args (int argc,
                  char **argv);
  // Parse args.

  // Flag to know when we are done.
  int done (void) const;
  void done (int);

  Connection_Manager &connection_manager (void);
  // Accessor to connection manager.

protected:
  Connection_Manager connection_manager_;
  // Connection manager.

  SENDER_ENDPOINT_STRATEGY sender_endpoint_strategy_;
  // The sender endpoint strategy.

  RECEIVER_ENDPOINT_STRATEGY receiver_endpoint_strategy_;
  // The receiver endpoint strategy.

  TAO_MMDevice* distributer_receiver_mmdevice_;
  // The distributer receiver multimedia device

  TAO_MMDevice* distributer_sender_mmdevice_;
  // The distributer receiver multimedia device

  ACE_CString sender_name_;
  // The name of the sender to connect to.

  ACE_CString distributer_name_;
  // Our name.

  int done_;
  // Flag to know when we are done.
};
