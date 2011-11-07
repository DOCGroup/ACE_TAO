/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    receiver.h
 *
 *  $Id$
 *
 *  This application receives data from a AV sender and writes it to
 *  a file.
 *
 *
 *  @author Yamuna Krishnamurthy <yamuna@cs.wustl.edu>
 */
//=============================================================================


#include "orbsvcs/Naming/Naming_Client.h"
#include "orbsvcs/AV/AVStreams_i.h"
#include "orbsvcs/AV/Endpoint_Strategy.h"
#include "orbsvcs/AV/Policy.h"
#include "orbsvcs/AV/Protocol_Factory.h"

/**
 * @class Receiver_Callback
 *
 * @brief Application defined callback object.
 *
 * AVStreams calls this class when data shows up from a sender.
 */
class Receiver_Callback : public TAO_AV_Callback
{
public:

  /// Constructor.
  Receiver_Callback (void);

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
  /// Keeping a count of the incoming frames.
  int frame_count_;

  /// Message block into which data is read from a file and then sent.
  ACE_Message_Block mb_;

  ACE_CString flowname_;

  /// File handle of the file into which received data is written.
  FILE *output_file_;

};

/**
 * @class Receiver_StreamEndPoint
 *
 * @brief Application defined stream endpoint object.
 *
 * AVStreams calls this class during connection setup.
 */
class Receiver_StreamEndPoint : public TAO_Server_StreamEndPoint
{
public:
  // Create a receiver application callback.
  int get_callback (const char *flowname,
                    TAO_AV_Callback *&callback);

  /// Set protocol object corresponding to the transport protocol
  /// chosen.
  int set_protocol_object (const char *flowname,
                           TAO_AV_Protocol_Object *object);
};

/**
 * @class Receiver
 *
 * @brief Receiver application class.
 *
 * This class receives data from a AV sender and writes it to
 * a file.
 */
class Receiver
{
public:
  /// Constructor
  Receiver (void);

  /// Destructor.
  ~Receiver (void);

  /// Initialize data components.
  int init (int argc,
            ACE_TCHAR *argv[]);

  /// Set the protocol object corresponding to the transport protocol chosen.
  void protocol_object (TAO_AV_Protocol_Object *protocol_object);

  /// Set the protocol object corresponding to the transport protocol chosen.
  TAO_AV_Protocol_Object * protocol_object (void);

protected:
  /// The Naming Service Client.
  TAO_Naming_Client naming_client_;

  /// The endpoint reactive strategy.
  TAO_AV_Endpoint_Reactive_Strategy_B
  <Receiver_StreamEndPoint,TAO_VDev,AV_Null_MediaCtrl> reactive_strategy_;

  /// Receiver MMDevice.
  TAO_MMDevice *mmdevice_;

  /// Rate at which the data will be sent.
  int frame_rate_;

  /// Protocol object corresponding to the transport protocol selected.
  TAO_AV_Protocol_Object *protocol_object_;

  /// Number of frames sent.
  int frame_count_;

  /// File from which data is read.
  ACE_CString filename_;

  /// Method to parse the command line arguments.
  int parse_args (int argc, ACE_TCHAR *argv[]);

  /// Message block into which data is read from a file and then sent.
  ACE_Message_Block mb_;

};
