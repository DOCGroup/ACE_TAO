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
//    Ftp server to receive data
//
// = AUTHOR
//    Yamuna Krishnamurthy <yamuna@cs.wustl.edu>
//
// ============================================================================

#include "orbsvcs/Naming/Naming_Utils.h"
#include "orbsvcs/AV/AVStreams_i.h"
#include "orbsvcs/AV/Endpoint_Strategy.h"
#include "orbsvcs/AV/Policy.h"


class FTP_Server_Callback : public TAO_AV_Callback
{
  // = TITLE
  //    Defines a class for the server application callback.
  //
  // = DESCRIPTION
  //    This class overides the methods of the TAO_AV_Callback so the
  //    AVStreams can make upcalls to the application.

public:

  // Method that is called when there is data to be received from the ftp client.
  int receive_frame (ACE_Message_Block *frame,
                     TAO_AV_frame_info *frame_info,
                     const ACE_Addr &peer_address);

  // Called when the ftp client has finished reading the file and wants
  // to close4 down the connection.
  int handle_destroy (void);
};

class FTP_Server_StreamEndPoint : public TAO_Server_StreamEndPoint
{
  // = TITLE
  //    Defines the aplication stream endpoint
  //
  // = DESCRIPTION
  //    This is the class that overrides the tao_server_enpodint to handle
  //    pre and post connect processing.
public:
  // Create the server application callback.
  int get_callback (const char *flowname,
                    TAO_AV_Callback *&callback);

private:
  FTP_Server_Callback callback_;
  // reference to the server application callback.
};

class Server
{
  // = TITLE
  //    Defines the server application class.
  //
  // = DESCRIPOTION
  //    The actual server progarm that acts as the ftp server that receives data
  //    sent by the ftp client.
public:
  Server (void);
  // Constructor

  ~Server (void);
  // Deestructor.

  int init (int argc,
            char **argv
            ACE_ENV_ARG_DECL_NOT_USED);
  // Initialize data components.

protected:
  TAO_Naming_Client my_naming_client_;
  // The Naming Service Client.

  TAO_AV_Endpoint_Reactive_Strategy_B
  <FTP_Server_StreamEndPoint,TAO_VDev,AV_Null_MediaCtrl> reactive_strategy_;
  // The endpoint reacxtive strategy.

  TAO_MMDevice *mmdevice_;
  // The server MMDevice.
};
