/* -*- C++ -*- */
/**
 *  @file Consumer_Client.h
 *
 *  $Id$
 *
 *  @author Pradeep Gore <pradeep@oomworks.com>
 *
 *
 */

#ifndef TAO_Notify_CONSUMER_CLIENT_H
#define TAO_Notify_CONSUMER_CLIENT_H
#include "ace/pre.h"

#include "ORB_Objects.h"
#include "ace/Task.h"
#include "tao/corba.h"
#include "tao/RTCORBA/RTCORBA.h"

class TAO_Notify_Lanes_Consumer;

/**
 * @class TAO_Notify_Lanes_Consumer_Client
 *
 * @brief
 *
 */
class TAO_Notify_Lanes_Consumer_Client  : public ACE_Task_Base
{
public:
  /// Constuctor
  TAO_Notify_Lanes_Consumer_Client (TAO_Notify_ORB_Objects& orb_objects);

  /// Destructor
  ~TAO_Notify_Lanes_Consumer_Client ();

  /// Init
  void initialize (ACE_ENV_SINGLE_ARG_DECL);

  /// Run
  void run (ACE_ENV_SINGLE_ARG_DECL);

  /// Parse Args
  int parse_args (int argc, char *argv[]);

  /// The thread entry point.
  virtual int svc (void);

protected:
  /// Create an RT POA with a single lane at the specified RT Priority.
  PortableServer::POA_ptr create_rt_poa (ACE_ENV_SINGLE_ARG_DECL);

  /// ORB Objects.
  TAO_Notify_ORB_Objects orb_objects_;

  /// Lock to serialize internal state.
  TAO_SYNCH_MUTEX lock_;

  /// Count how many consumers are done
  int consumer_done_count_;

  /// The Priority that we want the consumer lane to be at.
  RTCORBA::Priority lane_priority_;

  /// The Consumer.
  TAO_Notify_Lanes_Consumer* consumer_;

  /// The Type the Consumer should subscribe to.
  ACE_CString event_type_;
};

#include "ace/post.h"
#endif /* TAO_Notify_CONSUMER_CLIENT_H */
