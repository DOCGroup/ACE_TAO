/* -*- C++ -*- */
/**
 *  @file Consumer_Client.h
 *
 *  @author Pradeep Gore <pradeep@oomworks.com>
 */

#ifndef TAO_Notify_CONSUMER_CLIENT_H
#define TAO_Notify_CONSUMER_CLIENT_H

#include /**/ "ace/pre.h"
#include "ace/Task.h"

#include "ORB_Objects.h"
#include "tao/RTCORBA/RTCORBA.h"

class TAO_Notify_ThreadPool_Consumer;

/**
 * @class TAO_Notify_ThreadPool_Consumer_Client
 *
 * @brief
 */
class TAO_Notify_ThreadPool_Consumer_Client  : public ACE_Task_Base
{
public:
  explicit TAO_Notify_ThreadPool_Consumer_Client (TAO_Notify_ORB_Objects& orb_objects);

  TAO_Notify_ThreadPool_Consumer_Client (TAO_Notify_ThreadPool_Consumer_Client const&) = delete;
  TAO_Notify_ThreadPool_Consumer_Client (TAO_Notify_ThreadPool_Consumer_Client&&) = delete;
  TAO_Notify_ThreadPool_Consumer_Client& operator= (TAO_Notify_ThreadPool_Consumer_Client const&) = delete;
  TAO_Notify_ThreadPool_Consumer_Client& operator= (TAO_Notify_ThreadPool_Consumer_Client&&) = delete;

  ~TAO_Notify_ThreadPool_Consumer_Client () override = default;

  /// Init
  void _init ();

  /// Run
  /// Parse Args
  int parse_args (int argc, ACE_TCHAR *argv[]);

  /// The thread entry point.
  int svc () override;

  /// Dump stats.
  void dump_stats ();

protected:
  /// Create an RT POA with a single threadpool.
  PortableServer::POA_ptr create_rt_poa ();

  /// ORB Objects.
  TAO_Notify_ORB_Objects& orb_objects_;

  // The POA retains ownership of this servant after implicit activation.
  TAO_Notify_ThreadPool_Consumer* consumer_ {};

  struct Options
  {
    int proxy_supplier_thread_count {};
    int max_events {10};
    long delay {};
  };

  Options options_ {};
};

#include /**/ "ace/post.h"
#endif /* TAO_Notify_CONSUMER_CLIENT_H */
