/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/orbsvcs/tests/AVStreams/Asynch_Three_Stage
//
// = FILENAME
//    sender.h
//
// = DESCRIPTION
//    This application reads data from a file and sends it to s
//    receiver.
//
// = AUTHOR
//    Yamuna Krishnamurthy <yamuna@cs.wustl.edu>
//
// ============================================================================

#include "Connection_Manager.h"
#include "orbsvcs/AV/AVStreams_i.h"
#include "orbsvcs/AV/Endpoint_Strategy.h"
#include "orbsvcs/AV/Protocol_Factory.h"

class Sender_StreamEndPoint : public TAO_Client_StreamEndPoint
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
          <Sender_StreamEndPoint,
           TAO_VDev,
           AV_Null_MediaCtrl>
        SENDER_ENDPOINT_STRATEGY;

class Sender
{
  // = TITLE
  //    Sender Application.
  //
  // = DESCRIPTION
  //    Class is responsible for streaming (and pacing) data to a
  //    receiver.
public:
  Sender (void);
  // Constructor

  int init (int argc,
            char **argv
            ACE_ENV_ARG_DECL_NOT_USED);
  // Method to initialize the various data components.

  int pace_data (ACE_ENV_SINGLE_ARG_DECL_NOT_USED);
  // Method to pace and send data from a file.

  Connection_Manager &connection_manager (void);
  // Accessor to the connection manager.

private:
  int parse_args (int argc, char **argv);
  // Method to parse the command line arguments.

  SENDER_ENDPOINT_STRATEGY endpoint_strategy_;
  // The endpoint strategy used by the sender.

  TAO_MMDevice *sender_mmdevice_;
  // The sender MMDevice.

  int frame_count_;
  // Number of frames sent.

  ACE_CString filename_;
  // File from which data is read.

  FILE *input_file_;
  // File handle of the file read from.

  ACE_CString addr_file_;
  // File from which data is read.

  double frame_rate_;
  // Rate at which the data will be sent.

  ACE_Message_Block mb_;
  // Message block into which data is read from a file and then sent.

  ACE_CString sender_name_;
  // Name of this sender.

  // Connection manager.
  Connection_Manager connection_manager_;
};
