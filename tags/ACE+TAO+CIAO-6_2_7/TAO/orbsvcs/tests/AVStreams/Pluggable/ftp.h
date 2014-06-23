/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    ftp.h
 *
 *  $Id$
 *
 *  Ftp client to send data
 *
 *
 *  @author Yamuna Krishnamurthy <yamuna@cs.wustl.edu>
 */
//=============================================================================



#ifndef TAO_AV_FTP_H
#define TAO_AV_FTP_H

#include "ace/Get_Opt.h"
#include "orbsvcs/Naming/Naming_Client.h"
#include "orbsvcs/AV/AVStreams_i.h"
#include "orbsvcs/AV/Endpoint_Strategy.h"
#include "orbsvcs/AV/Policy.h"
#include "orbsvcs/AV/Protocol_Factory.h"


/**
 * @class FTP_Client_Callback
 *
 * @brief Defines the client applcation callback.
 *
 * This class can override the methods of
 * the TAO_AV_Callback to do application
 * specific processing.
 */
class FTP_Client_Callback : public TAO_AV_Callback
{
public:
  ///Constructor
  FTP_Client_Callback (void);
};


/**
 * @class FTP_Client_StreamEndPoint
 *
 * @brief Defines the client stream endpoint.
 *
 * This class overrides the methods of TAO_ClientStreamendpoint
 * so the application can perform its processing during post and pre
 * connection set up.
 */
class FTP_Client_StreamEndPoint  : public TAO_Client_StreamEndPoint
{

public:
  ///Contructor
  FTP_Client_StreamEndPoint (void);

  /// Create the application client callback and return its handle to the
  /// AVSTreams for further application callbacks
  virtual int get_callback (const char *flowname,
                            TAO_AV_Callback *&callback);

  /// Set protocol object corresponding to the transport protocol chosen.
  virtual int set_protocol_object (const char *flowname,
                                   TAO_AV_Protocol_Object *object);

protected:
  /// reference to the cllient application callback.
  FTP_Client_Callback callback_;
};

typedef TAO_AV_Endpoint_Reactive_Strategy_A <FTP_Client_StreamEndPoint,TAO_VDev,AV_Null_MediaCtrl> ENDPOINT_STRATEGY;


/**
 * @class Client
 *
 * @brief Defines the Client Application
 *
 * The actual client program that acts as the ftp client that streams data
 * to the ftp servers that are waiting for data.
 */
class Client
{
public:
  /// Constructor
  Client (void);

  /// Method to initialize the various data components.
  int init (int argc,
            ACE_TCHAR *argv[]);

  /// Set the protocol object corresponding to the transport protocol chosen.
  void set_protocol_object (TAO_AV_Protocol_Object *protocol_object);

  /// Method to pace and send data from a file.
  int pace_data (void);

  /// File handle from which data is read to be sent.
  FILE *file (void);

  /// The stream control interface that manages the stream set up
  TAO_StreamCtrl* streamctrl (void);

  /// name of the flow set up.
  char *flowname (void);

  /// The requested frame rate for sending each frame of data read from the file.
  int frame_rate (void);

private:
  /// Method to parse the command line arguments.
  int parse_args (int argc, ACE_TCHAR *argv[]);

  /// Method that binds the ftp client to the server
  int bind_to_server (void);

  /// The reacfive strategy of the client.
  ENDPOINT_STRATEGY endpoint_strategy_;

  /// The server MMDevice that the ftpo client connects to
  AVStreams::MMDevice_var server_mmdevice_;

  /// The ftp client MMDevice.
  TAO_MMDevice client_mmdevice_;

  /// Video stream controller
  TAO_StreamCtrl streamctrl_;

  /// Number of frames sent.
  int count_;

  int argc_;
  ACE_TCHAR **argv_;

  /// File from which data is read.
  const char *filename_;

  /// Address of the ftp client host machine or a multicast address - Default is
  /// UDP multicast addess
  const char *address_;
  const char *peer_addr_str_;

  /// The Naming Service client.
  TAO_Naming_Client my_naming_client_;

  /// File handle of the file read from.
  FILE *fp_;

  /// Selected protocol - default is UDP
  char *protocol_;

  char *flowname_;

  /// If set to 1 then use sfp as the flow carrier protocol.
  int use_sfp_;

  int frame_rate_;

  /// Message block into which data is read from a file and then sent.
  ACE_Message_Block mb;

  /// Protocol object corresponding to the transport protocol selected.
  TAO_AV_Protocol_Object* protocol_object_;

};

typedef ACE_Unmanaged_Singleton<Client,ACE_Null_Mutex> CLIENT;

#endif /* TAO_AV_FTP_H */

