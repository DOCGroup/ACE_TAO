/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    sender.h
 *
 *  This application reads data from a file and sends it to s
 *  receiver.
 *
 *  @author Yamuna Krishnamurthy <yamuna@cs.wustl.edu>
 */
//=============================================================================

#include <cstddef>
#include <memory>
#include <stdio.h>

#include "Connection_Manager.h"
#include "orbsvcs/AV/AVStreams_i.h"
#include "orbsvcs/AV/Endpoint_Strategy.h"
#include "orbsvcs/AV/Protocol_Factory.h"

/**
 * @class Sender_Callback
 *
 * @brief Defines a class for the sender application callback.
 *
 * This class overrides the methods of the TAO_AV_Callback so the
 * AVStreams can make upcalls to the application.
 */
class Sender_Callback : public TAO_AV_Callback
{
public:
  Sender_Callback () = default;
  Sender_Callback (Sender_Callback const &) = delete;
  Sender_Callback &operator= (Sender_Callback const &) = delete;
  Sender_Callback (Sender_Callback &&) = delete;
  Sender_Callback &operator= (Sender_Callback &&) = delete;

  /**
   * Called when the sender has finished reading the file and wants
   * to close down the connection. Also called when the distributer
   * tears down the connection when it switches to a new sender.
   */
  int handle_destroy () override;

  /// Accessor methods for the flowname of the callback
  ACE_CString &flowname ();
  void flowname (ACE_CString const &flowname);

private:
  //// Flowname of the callback.
  ACE_CString flowname_;
};

/**
 * @class Sender_StreamEndPoint
 *
 * @brief Defines a sender stream endpoint.
 */
class Sender_StreamEndPoint : public TAO_Client_StreamEndPoint
{
public:
  Sender_StreamEndPoint () = default;
  Sender_StreamEndPoint (Sender_StreamEndPoint const &) = delete;
  Sender_StreamEndPoint &operator= (Sender_StreamEndPoint const &) = delete;
  Sender_StreamEndPoint (Sender_StreamEndPoint &&) = delete;
  Sender_StreamEndPoint &operator= (Sender_StreamEndPoint &&) = delete;

  //// Create the application callback and return its handle to
  //// AVStreams for further application callbacks.
  int get_callback (char const *flowname,
                    TAO_AV_Callback *&callback) override;

  //// Set protocol object corresponding to the transport protocol
  //// chosen.
  int set_protocol_object (char const *flowname,
                           TAO_AV_Protocol_Object *object) override;

  //// Perform application specific actions before accepting new
  //// connections.
  CORBA::Boolean handle_preconnect (AVStreams::flowSpec &flowspec) override;

protected:
  //// Application callback.
  Sender_Callback callback_;
};

using SENDER_ENDPOINT_STRATEGY = TAO_AV_Endpoint_Reactive_Strategy_A
          <Sender_StreamEndPoint,
           TAO_VDev,
           AV_Null_MediaCtrl>;

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
  Sender ();

  ~Sender ();
  Sender (Sender const &) = delete;
  Sender &operator= (Sender const &) = delete;
  Sender (Sender &&) = delete;
  Sender &operator= (Sender &&) = delete;

  void shut_down ();

  //// Method to initialize the various data components.
  int init (int argc,
            ACE_TCHAR *argv[]);

  //// Method to pace and send data from a file.
  int pace_data ();

  //// Accessor to the connection manager.
  Connection_Manager &connection_manager ();

private:
  //// Method to parse the command line arguments.
  int parse_args (int argc, ACE_TCHAR *argv[]);

  //// The endpoint strategy used by the sender.
  SENDER_ENDPOINT_STRATEGY endpoint_strategy_;

  //// Non-owning pointer to the POA-managed sender MMDevice servant.
  TAO_MMDevice *sender_mmdevice_ {};

  //// Number of frames sent.
  std::size_t frame_count_ {};

  //// File from which data is read.
  ACE_CString filename_ {"input"};

  //// File handle of the file read from.
  std::unique_ptr<FILE, int (*) (FILE *)> input_file_;

  //// Rate at which the data will be sent.
  double frame_rate_ {5};

  //// Message block into which data is read from a file and then sent.
  ACE_Message_Block mb_ {BUFSIZ};

  //// Name of this sender.
  ACE_CString sender_name_ {"sender"};

  //// Connection manager.
  Connection_Manager connection_manager_;
};
