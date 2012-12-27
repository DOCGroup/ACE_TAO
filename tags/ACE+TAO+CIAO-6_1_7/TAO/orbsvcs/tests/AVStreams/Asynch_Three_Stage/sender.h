/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    sender.h
 *
 *  $Id$
 *
 *  This application reads data from a file and sends it to s
 *  receiver.
 *
 *
 *  @author Yamuna Krishnamurthy <yamuna@cs.wustl.edu>
 */
//=============================================================================


#include "Connection_Manager.h"
#include "orbsvcs/AV/AVStreams_i.h"
#include "orbsvcs/AV/Endpoint_Strategy.h"
#include "orbsvcs/AV/Protocol_Factory.h"

/**
 * @class Sender_StreamEndPoint
 *
 * @brief Defines a sender stream endpoint.
 */
class Sender_StreamEndPoint : public TAO_Client_StreamEndPoint
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
          <Sender_StreamEndPoint,
           TAO_VDev,
           AV_Null_MediaCtrl>
        SENDER_ENDPOINT_STRATEGY;

/**
 * @class Sender
 *
 * @brief Sender Application.
 *
 * Class is responsible for streaming (and pacing) data to a
 * receiver.
 */
class Sender
{
public:
  /// Constructor
  Sender (void);

  /// Method to initialize the various data components.
  int init (int argc,
            ACE_TCHAR **argv);

  /// Method to pace and send data from a file.
  int pace_data (void);

  /// Accessor to the connection manager.
  Connection_Manager &connection_manager (void);

private:
  /// Method to parse the command line arguments.
  int parse_args (int argc, ACE_TCHAR **argv);

  /// The endpoint strategy used by the sender.
  SENDER_ENDPOINT_STRATEGY endpoint_strategy_;

  /// The sender MMDevice.
  TAO_MMDevice *sender_mmdevice_;

  /// Number of frames sent.
  int frame_count_;

  /// File from which data is read.
  ACE_TString filename_;

  /// File handle of the file read from.
  FILE *input_file_;

  /// File from which data is read.
  ACE_TString addr_file_;

  /// Rate at which the data will be sent.
  double frame_rate_;

  /// Message block into which data is read from a file and then sent.
  ACE_Message_Block mb_;

  /// Name of this sender.
  ACE_TString sender_name_;

  // Connection manager.
  Connection_Manager connection_manager_;
};
