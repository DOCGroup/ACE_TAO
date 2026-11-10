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

#include <cstdio>
#include <memory>

#include "Connection_Manager.h"
#include "ace/OS_NS_stdio.h"
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
  Sender_StreamEndPoint () = default;
  Sender_StreamEndPoint (Sender_StreamEndPoint const &) = delete;
  Sender_StreamEndPoint &operator= (Sender_StreamEndPoint const &) = delete;
  Sender_StreamEndPoint (Sender_StreamEndPoint &&) = delete;
  Sender_StreamEndPoint &operator= (Sender_StreamEndPoint &&) = delete;

  /// Create the application callback and return its handle to
  /// AVStreams for further application callbacks.
  int get_callback (char const *flowname,
                    TAO_AV_Callback *&callback) override;

  /// Set protocol object corresponding to the transport protocol
  /// chosen.
  int set_protocol_object (char const *flowname,
                           TAO_AV_Protocol_Object *object) override;

protected:
  /// Application callback.
  TAO_AV_Callback callback_;
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
  Sender () = default;
  Sender (Sender const &) = delete;
  Sender &operator= (Sender const &) = delete;
  Sender (Sender &&) = delete;
  Sender &operator= (Sender &&) = delete;

  /// Method to initialize the various data components.
  int init (int argc,
            ACE_TCHAR **argv);

  /// Method to pace and send data from a file.
  int pace_data ();

  /// Accessor to the connection manager.
  Connection_Manager &connection_manager ();

private:
  /// Method to parse the command line arguments.
  int parse_args (int argc, ACE_TCHAR **argv);

  /// The endpoint strategy used by the sender.
  SENDER_ENDPOINT_STRATEGY endpoint_strategy_;

  /// Number of frames sent.
  int frame_count_ {};

  /// File handle of the file read from.
  using File_Ptr = std::unique_ptr<FILE, int (*) (FILE *)>;
  File_Ptr input_file_ {nullptr, ACE_OS::fclose};

  /// Message block into which data is read from a file and then sent.
  ACE_Message_Block mb_ {BUFSIZ};

  struct Options
  {
    ACE_TString filename {ACE_TEXT ("input")};
    ACE_TString addr_file {ACE_TEXT ("addr_file")};
    ACE_CString sender_name {"sender"};
    double frame_rate {10.0};
  };

  Options options_;

  // Connection manager.
  Connection_Manager connection_manager_;
};
