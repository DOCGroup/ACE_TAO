/* -*- C++ -*- */
// $Id$
// ============================================================================
//
// = LIBRARY
//    TAO/orbsvcs/tests/AVStreams/Simple_Three_Stage
//
// = FILENAME
//    receiver.h
//
// = DESCRIPTION
//    Receiver to receive data from the distributer
//
// = AUTHOR
//    Yamuna Krishnamurthy <yamuna@cs.wustl.edu>
//
// ============================================================================

#ifndef RECEIVER_H
#define RECEIVER_H

#include "orbsvcs/Naming/Naming_Utils.h"
#include "orbsvcs/AV/AVStreams_i.h"
#include "orbsvcs/AV/Endpoint_Strategy.h"
#include "orbsvcs/AV/Policy.h"

class Receiver_Callback : public TAO_AV_Callback
{
  // = TITLE
  //    Defines a class for the application callback.
  //
  // = DESCRIPTION
  //    This class overides the methods of the TAO_AV_Callback so the 
  //    AVStreams can make upcalls to the application.
  
public:

  // Method that is called when there is data to be received from the distributer
  int receive_frame (ACE_Message_Block *frame,
                     TAO_AV_frame_info *frame_info,
                     const ACE_Addr &peer_address);

  // Called when the distributer has finished sending the data and wants
  // to close down the connection.
  int handle_destroy (void);

};

class Receiver_StreamEndPoint : public TAO_Server_StreamEndPoint
{
  // = TITLE
  //    Defines the aplication stream endpoint
  //
  // = DESCRIPTION
  //    This is the class that overrides the tao_server_enpodint to handle
  //    pre and post connect processing.
public:
  // Create the application callback.
  int get_callback (const char *flowname,
                    TAO_AV_Callback *&callback);

private:
  Receiver_Callback callback_;
  // reference to the server application callback.
};

class Receiver
{
  // = TITLE
  //    Defines the receiver application class.
  // 
  // = DESCRIPTION
  //    The receiver progarm that receives data
  //    sent by the distributer.
public:
  Receiver (void);
  // Constructor

  ~Receiver (void);
  // Destructor.

  int init (int argc,
            char **argv,
            CORBA::Environment &);
  // Initialize data components.
  
protected:
  TAO_Naming_Client my_naming_client_;
  // The Naming Service Client.

  TAO_AV_Endpoint_Reactive_Strategy_B
  <Receiver_StreamEndPoint,TAO_VDev,AV_Null_MediaCtrl> reactive_strategy_;
  // The endpoint reactive strategy.
  
  TAO_MMDevice *mmdevice_;
  // The receiver MMDevice.
};

#endif /*RECEIVER_H*/
