/* -*- C++ -*- */
// $Id$

//=============================================================================
/**
 *  @file   receiver.h
 *
 *  receiver.h,v 1.1 2001/04/15 02:56:26 yamuna Exp
 *
 *  @author Yamuna Krishnamurthy <yamuna@cs.wustl.edu>
 */
//=============================================================================

#include "orbsvcs/Naming/Naming_Utils.h"
#include "orbsvcs/AV/AVStreams_i.h"
#include "orbsvcs/AV/Endpoint_Strategy.h"
#include "orbsvcs/AV/Policy.h"

// = AVStreams calls this class when data shows up from a sender.
class Receiver_Callback : public TAO_AV_Callback
{
public:

  Receiver_Callback (void);
  /// Constructor.

  /// Method that is called when there is data to be received from a
  /// sender.
  int receive_frame (ACE_Message_Block *frame,
                     TAO_AV_frame_info *frame_info,
                     const ACE_Addr &peer_address);

  /// Called when the sender is done sending data and wants to close
  /// down the connection.
  int handle_destroy (void);

  /// Set the related stream control for this flow.
  void streamctrl (AVStreams::StreamCtrl_ptr streamctrl);

private:
  int frame_count_;
  /// Keeping a count of the incoming frames.

  /// Related stream control.
  AVStreams::StreamCtrl_ptr streamctrl_;

};

// = AVStreams calls this class during connection setup.
class Receiver_StreamEndPoint : public TAO_Server_StreamEndPoint
{

public:
  /// Create a receiver application callback.
  int get_callback (const char *flowname,
                    TAO_AV_Callback *&callback);

private:
  Receiver_Callback callback_;
  /// Receiver application callback.
};

// = This class receives data from a AV sender and writes it to a file.
class Receiver
{
public:
  Receiver (void);
  /// Constructor

  ~Receiver (void);
  /// Destructor.

  int init (int argc,
            char **argv
            TAO_ENV_ARG_DECL_NOT_USED);
  /// Initialize data components.


protected:
  TAO_Naming_Client naming_client_;
  /// The Naming Service Client.

  TAO_AV_Endpoint_Reactive_Strategy_B
  <Receiver_StreamEndPoint,TAO_VDev,AV_Null_MediaCtrl> reactive_strategy_;
  /// The endpoint reactive strategy.

  TAO_MMDevice *mmdevice_;
  /// Receiver MMDevice.
};

