/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    receiver.h
 *
 *  This application receives data from a AV sender and writes it to
 *  a file.
 *
 *  @author Yamuna Krishnamurthy <yamuna@cs.wustl.edu>
 */
//=============================================================================

#include <cstddef>
#include <cstdio>
#include <memory>
#include <string>

#include "ace/OS_NS_stdio.h"
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
  Receiver_Callback () = default;
  Receiver_Callback (Receiver_Callback const &) = delete;
  Receiver_Callback &operator= (Receiver_Callback const &) = delete;
  Receiver_Callback (Receiver_Callback &&) = delete;
  Receiver_Callback &operator= (Receiver_Callback &&) = delete;

  // Method that is called when there is data to be received from a
  // sender.
  int receive_frame (ACE_Message_Block *frame,
                     TAO_AV_frame_info *frame_info,
                     ACE_Addr const &peer_address) override;

  // Called when the sender is done sending data and wants to close
  // down the connection.
  int handle_destroy () override;

  int flowname (std::string const &flow_name);

private:
  /// Keeping a count of the incoming frames.
  std::size_t frame_count_ {1};

  std::string flowname_;

  /// File handle of the file into which received data is written.
  std::unique_ptr<FILE, int (*) (FILE *)> output_file_ {nullptr, ACE_OS::fclose};
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
  Receiver_StreamEndPoint () = default;
  Receiver_StreamEndPoint (Receiver_StreamEndPoint const &) = delete;
  Receiver_StreamEndPoint &operator= (Receiver_StreamEndPoint const &) = delete;
  Receiver_StreamEndPoint (Receiver_StreamEndPoint &&) = delete;
  Receiver_StreamEndPoint &operator= (Receiver_StreamEndPoint &&) = delete;

  // Create a receiver application callback.
  int get_callback (char const *flowname,
                    TAO_AV_Callback *&callback) override;

  /// Set protocol object corresponding to the transport protocol
  /// chosen.
  int set_protocol_object (char const *flowname,
                           TAO_AV_Protocol_Object *object) override;
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
  Receiver () = default;
  Receiver (Receiver const &) = delete;
  Receiver &operator= (Receiver const &) = delete;
  Receiver (Receiver &&) = delete;
  Receiver &operator= (Receiver &&) = delete;

  /// Initialize data components.
  int init (int argc,
            ACE_TCHAR *argv[]);

  /// Set the protocol object corresponding to the transport protocol chosen.
  void protocol_object (TAO_AV_Protocol_Object *protocol_object);

  /// Set the protocol object corresponding to the transport protocol chosen.
  TAO_AV_Protocol_Object *protocol_object ();

protected:
  /// The Naming Service Client.
  TAO_Naming_Client naming_client_;

  /// The endpoint reactive strategy.
  TAO_AV_Endpoint_Reactive_Strategy_B
  <Receiver_StreamEndPoint,TAO_VDev,AV_Null_MediaCtrl> reactive_strategy_;

  /// Receiver MMDevice.
  // Non-owning; ServantBase_var and the POA manage the servant reference.
  TAO_MMDevice *mmdevice_ {};

  /// Protocol object corresponding to the transport protocol selected.
  // Non-owning pointer managed by the AVStreams framework.
  TAO_AV_Protocol_Object *protocol_object_ {};

  /// Method to parse the command line arguments.
  int parse_args (int argc, ACE_TCHAR *argv[]);
};
