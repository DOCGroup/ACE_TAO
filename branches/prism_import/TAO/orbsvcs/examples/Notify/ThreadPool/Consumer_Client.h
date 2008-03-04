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

#include /**/ "ace/pre.h"
#include "ace/Task.h"
#include "ace/SString.h"

/// @@Pradeep, pragma please...

#include "ORB_Objects.h"
#include "tao/RTCORBA/RTCORBA.h"

class TAO_Notify_ThreadPool_Consumer;

/**
 * @class TAO_Notify_ThreadPool_Consumer_Client
 *
 * @brief
 *
 */
class TAO_Notify_ThreadPool_Consumer_Client  : public ACE_Task_Base
{
public:
  /// Constuctor
  TAO_Notify_ThreadPool_Consumer_Client (TAO_Notify_ORB_Objects& orb_objects);

  /// Destructor
  ~TAO_Notify_ThreadPool_Consumer_Client ();

  /// Init
  void _init (void);

  /// Run
  void run (void);

  /// Parse Args
  int parse_args (int argc, char *argv[]);

  /// The thread entry point.
  virtual int svc (void);

  /// Dump stats.
  void dump_stats (void);

protected:
  /// Create an RT POA with a single threadpool.
  PortableServer::POA_ptr create_rt_poa (void);

  /// ORB Objects.
  TAO_Notify_ORB_Objects orb_objects_;

  /// Lock to serialize internal state.
  TAO_SYNCH_MUTEX lock_;

  /// Count how many consumers are done
  int consumer_done_count_;

  /// The Consumer.
  TAO_Notify_ThreadPool_Consumer* consumer_;

  /// ProxySuppler Thread count.
  int proxy_supplier_thread_count_;

  /// Max events that we expect to receive.
  int max_events_;

  /// Delay in ms
  long delay_;
};

#include /**/ "ace/post.h"
#endif /* TAO_Notify_CONSUMER_CLIENT_H */
