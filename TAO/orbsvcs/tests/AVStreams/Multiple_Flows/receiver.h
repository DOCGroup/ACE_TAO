/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/orbsvcs/tests/AVStreams/Simple
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

#include "orbsvcs/Naming/Naming_Utils.h"
#include "orbsvcs/AV/AVStreams_i.h"
#include "orbsvcs/AV/Endpoint_Strategy.h"
#include "orbsvcs/AV/Policy.h"
#include "orbsvcs/AV/Protocol_Factory.h"

class Receiver_Callback : public TAO_AV_Callback
{
  // = TITLE
  //    Application defined callback object.
  //
  // = DESCRIPTION
  //    AVStreams calls this class when data shows up from a sender.
public:

  Receiver_Callback (void);
  // Constructor.

  // Method that is called when there is data to be received from a
  // sender.
  int receive_frame (ACE_Message_Block *frame,
                     TAO_AV_frame_info *frame_info,
                     const ACE_Addr &peer_address);

  // Called when the sender is done sending data and wants to close
  // down the connection.
  int handle_destroy (void);

  void flowname (const char*);

private:
  int frame_count_;
  // Keeping a count of the incoming frames.

  ACE_Message_Block mb_;
  // Message block into which data is read from a file and then sent.

  ACE_CString flowname_;

  FILE *output_file_;
  // File handle of the file into which received data is written.

};

class Receiver_StreamEndPoint : public TAO_Server_StreamEndPoint
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

  int set_protocol_object (const char *flowname,
                           TAO_AV_Protocol_Object *object);
  // Set protocol object corresponding to the transport protocol
  // chosen.
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
  // Constructor

  ~Receiver (void);
  // Destructor.

  int init (int argc,
            char **argv
            ACE_ENV_ARG_DECL_NOT_USED);
  // Initialize data components.

  void protocol_object (TAO_AV_Protocol_Object *protocol_object);
  // Set the protocol object corresponding to the transport protocol chosen.

  TAO_AV_Protocol_Object * protocol_object (void);
  // Set the protocol object corresponding to the transport protocol chosen.

protected:
  TAO_Naming_Client naming_client_;
  // The Naming Service Client.

  TAO_AV_Endpoint_Reactive_Strategy_B
  <Receiver_StreamEndPoint,TAO_VDev,AV_Null_MediaCtrl> reactive_strategy_;
  // The endpoint reactive strategy.

  TAO_MMDevice *mmdevice_;
  // Receiver MMDevice.

  int frame_rate_;
  // Rate at which the data will be sent.

  TAO_AV_Protocol_Object *protocol_object_;
  // Protocol object corresponding to the transport protocol selected.

  int frame_count_;
  // Number of frames sent.

  ACE_CString filename_;
  // File from which data is read.

  int parse_args (int argc, char **argv);
  // Method to parse the command line arguments.

  ACE_Message_Block mb_;
  // Message block into which data is read from a file and then sent.

};






