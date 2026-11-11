/* -*- C++ -*- */
/**
 *  @file Consumer_Client.h
 *
 *  @author Pradeep Gore <pradeep@oomworks.com>
 */

#ifndef TAO_Notify_CONSUMER_CLIENT_H
#define TAO_Notify_CONSUMER_CLIENT_H

#include /**/ "ace/pre.h"

#include "ORB_Objects.h"

#include "tao/RTCORBA/RTCORBA.h"

#include "ace/Task.h"

#include <string>

class TAO_Notify_Lanes_Consumer;

/**
 * @class TAO_Notify_Lanes_Consumer_Client
 *
 * @brief
 */
class TAO_Notify_Lanes_Consumer_Client  : public ACE_Task_Base
{
public:
  /// Constructor
  explicit TAO_Notify_Lanes_Consumer_Client (TAO_Notify_ORB_Objects& orb_objects);
  ~TAO_Notify_Lanes_Consumer_Client () override = default;
  TAO_Notify_Lanes_Consumer_Client (TAO_Notify_Lanes_Consumer_Client const&) = delete;
  TAO_Notify_Lanes_Consumer_Client& operator= (TAO_Notify_Lanes_Consumer_Client const&) = delete;
  TAO_Notify_Lanes_Consumer_Client (TAO_Notify_Lanes_Consumer_Client&&) = delete;
  TAO_Notify_Lanes_Consumer_Client& operator= (TAO_Notify_Lanes_Consumer_Client&&) = delete;

  /// Init
  void initialize ();

  /// Parse Args
  int parse_args (int argc, ACE_TCHAR *argv[]);

  /// The thread entry point.
  int svc () override;

protected:
  /// Create an RT POA with a single lane at the specified RT Priority.
  PortableServer::POA_ptr create_rt_poa () const;

  /// ORB Objects.
  TAO_Notify_ORB_Objects& orb_objects_;

  /// The Priority that we want the consumer lane to be at.
  RTCORBA::Priority lane_priority_ {};

  /// The Consumer.
  // The POA retains the activated servant; this pointer is non-owning.
  TAO_Notify_Lanes_Consumer* consumer_ {};

  /// The Type the Consumer should subscribe to.
  std::string event_type_;
};

#include /**/ "ace/post.h"
#endif /* TAO_Notify_CONSUMER_CLIENT_H */
