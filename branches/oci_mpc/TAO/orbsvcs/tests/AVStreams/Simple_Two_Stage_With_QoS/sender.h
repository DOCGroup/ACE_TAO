/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/orbsvcs/tests/AVStreams/Simple_Two_Stage_With_QoS
//
// = FILENAME
//    sender.h
//
// = DESCRIPTION
//    This application reads data from a file and sends it to s
//    receiver through an RSVP enabled stream.
//
// = AUTHOR
//    Yamuna Krishnamurthy <yamuna@cs.wustl.edu>
//
// ============================================================================

#include "orbsvcs/Naming/Naming_Utils.h"
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

  CORBA::Boolean modify_QoS (AVStreams::streamQoS &new_qos,
                             const AVStreams::flowSpec &/* the_flows */
                             ACE_ENV_ARG_DECL)
                            ACE_THROW_SPEC(( CORBA::SystemException,
                                             AVStreams::noSuchFlow,
                                             AVStreams::QoSRequestFailed ));
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

  void protocol_object (TAO_AV_Protocol_Object *protocol_object);
  // Set the protocol object corresponding to the transport protocol chosen.

  void fill_qos (AVStreams::streamQoS &);
  // Method to populate the required qos.

private:
  int parse_args (int argc, char **argv);
  // Method to parse the command line arguments.

  int bind_to_receiver (ACE_ENV_SINGLE_ARG_DECL);
  // Method that binds the sender to the receiver.

  SENDER_ENDPOINT_STRATEGY endpoint_strategy_;
  // The endpoint strategy used by the sender.

  AVStreams::MMDevice_var receiver_mmdevice_;
  // The receiver MMDevice that the sender connects to.

  TAO_MMDevice *sender_mmdevice_;
  // The sender MMDevice.

  TAO_StreamCtrl *streamctrl_;
  // Stream controller

  int frame_count_;
  // Number of frames sent.

  ACE_CString filename_;
  // File from which data is read.

  TAO_Naming_Client naming_client_;
  // The Naming Service client.

  FILE *input_file_;
  // File handle of the file read from.

  ACE_CString protocol_;
  // Selected protocol - default is UDP

  double frame_rate_;
  // Rate at which the data will be sent.

  ACE_Message_Block mb_;
  // Message block into which data is read from a file and then sent.

  TAO_AV_Protocol_Object *protocol_object_;
  // Protocol object corresponding to the transport protocol selected.

  ACE_CString address_;
  // Destination Address.

  ACE_CString flowname_;
};
