/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/orbsvcs/tests/AVStreams/Pluggable
//
// = FILENAME
//    ftp.h
//
// = DESCRIPTION
//    Ftp client to send data
//
// = AUTHOR
//    Yamuna Krishnamurthy <yamuna@cs.wustl.edu>
//
// ============================================================================


#ifndef TAO_AV_FTP_H
#define TAO_AV_FTP_H

#include "ace/Get_Opt.h"
#include "orbsvcs/Naming/Naming_Utils.h"
#include "orbsvcs/AV/AVStreams_i.h"
#include "orbsvcs/AV/Endpoint_Strategy.h"
#include "orbsvcs/AV/Policy.h"
#include "orbsvcs/AV/Protocol_Factory.h"


class FTP_Client_Callback : public TAO_AV_Callback
{
  // = TITLE
  //    Defines the client applcation callback.
  //
  // = DESCRIPTION
  //    This class can override the methods of
  //    the TAO_AV_Callback to do application
  //    specific processing.
public:
  FTP_Client_Callback (void);
  //Constructor
};


class FTP_Client_StreamEndPoint  : public TAO_Client_StreamEndPoint
{
  // = TITLE
  //    Defines the client stream endpoint.
  //
  // = DESCRIPTION
  //    This class overrides the methods of TAO_ClientStreamendpoint
  //    so the application can perform its processing during post and pre
  //    connection set up.

public:
  FTP_Client_StreamEndPoint (void);
  //Contructor

  virtual int get_callback (const char *flowname,
                            TAO_AV_Callback *&callback);
  // Create the application client callback and return its handle to the
  // AVSTreams for further application callbacks

  virtual int set_protocol_object (const char *flowname,
                                   TAO_AV_Protocol_Object *object);
  // Set protocol object corresponding to the transport protocol chosen.

protected:
  FTP_Client_Callback callback_;
  // reference to the cllient application callback.
};

typedef TAO_AV_Endpoint_Reactive_Strategy_A <FTP_Client_StreamEndPoint,TAO_VDev,AV_Null_MediaCtrl> ENDPOINT_STRATEGY;


class Client
{
  // = TITLE
  //    Defines the Client Application
  //
  // = DESCRIPTION
  //    The actual client program that acts as the ftp client that streams data
  //    to the ftp servers that are waiting for data.
public:
  Client (void);
  // Constructor

  int init (int argc,
            char **argv
            ACE_ENV_ARG_DECL_NOT_USED);
  // Method to initialize the various data components.

  void set_protocol_object (TAO_AV_Protocol_Object *protocol_object);
  // Set the protocol object corresponding to the transport protocol chosen.

  int pace_data (ACE_ENV_SINGLE_ARG_DECL_NOT_USED);
  // Method to pace and send data from a file.

  FILE *file (void);
  // File handle from which data is read to be sent.

  TAO_StreamCtrl* streamctrl (void);
  // The stream control interface that manages the stream set up

  char *flowname (void);
  // name of the flow set up.

  int frame_rate (void);
  // The requested frame rate for sending each frame of data read from the file.

private:
  int parse_args (int argc, char **argv);
  // Method to parse the command line arguments.

  int bind_to_server (ACE_ENV_SINGLE_ARG_DECL);
  // Method that binds the ftp client to the server

  ENDPOINT_STRATEGY endpoint_strategy_;
  // The reacfive strategy of the client.

  AVStreams::MMDevice_var server_mmdevice_;
  // The server MMDevice that the ftpo client connects to

  TAO_MMDevice client_mmdevice_;
  // The ftp client MMDevice.

  TAO_StreamCtrl streamctrl_;
  // Video stream controller

  int count_;
  // Number of frames sent.

  int argc_;
  char **argv_;

  const char *filename_;
  // File from which data is read.

  const char *address_;
  // Address of the ftp client host machine or a multicast address - Default is
  // UDP multicast addess

  TAO_Naming_Client my_naming_client_;
  // The Naming Service client.

  FILE *fp_;
  // File handle of the file read from.

  char *protocol_;
  // Selected protocol - default is UDP

  char *flowname_;

  int use_sfp_;
  // If set to 1 then use sfp as the flow carrier protocol.

  int frame_rate_;

  ACE_Message_Block mb;
  // Message block into which data is read from a file and then sent.

  TAO_AV_Protocol_Object* protocol_object_;
  // Protocol object corresponding to the transport protocol selected.

};

typedef ACE_Singleton<Client,ACE_Null_Mutex> CLIENT;

#endif /* TAO_AV_FTP_H */

