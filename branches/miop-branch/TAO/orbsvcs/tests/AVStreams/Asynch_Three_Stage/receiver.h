/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/orbsvcs/tests/AVStreams/Asynch_Three_Stage
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

private:
  int frame_count_;
  // Keeping a count of the incoming frames.
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

private:
  Receiver_Callback callback_;
  // Receiver application callback.
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
            TAO_ENV_ARG_DECL_NOT_USED);
  // Initialize data components.

  int parse_args (int argc,
                  char **argv);
  // Parse args.

  ACE_CString output_file_name (void);
  // Name of the output file.

protected:
  Connection_Manager connection_manager_;
  // Connection manager.

  TAO_AV_Endpoint_Reactive_Strategy_B
  <Receiver_StreamEndPoint,TAO_VDev,AV_Null_MediaCtrl> reactive_strategy_;
  // The endpoint reactive strategy.

  AVStreams::MMDevice_var mmdevice_obj_;
  // The receiver MMDevice.

  TAO_MMDevice *mmdevice_;
  // Receiver MMDevice.

  AVStreams::MMDevice_var sender_mmdevice_;
  // The sender MMDevice

  ACE_CString output_file_name_;
  // File name of the file into which received data is written.

  ACE_CString sender_name_;
  // Sender name.

  ACE_CString receiver_name_;
  // Receiver name.
};
