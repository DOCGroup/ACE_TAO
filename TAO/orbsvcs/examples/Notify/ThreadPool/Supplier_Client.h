/* -*- C++ -*- */
/**
 *  @file Supplier_Client.h
 *
 *  @author Pradeep Gore <pradeep@oomworks.com>
 */

#ifndef TAO_Notify_SUPPLIER_CLIENT_H
#define TAO_Notify_SUPPLIER_CLIENT_H
#include /**/ "ace/pre.h"

#include "ORB_Objects.h"
#include "ace/Task.h"
#include "ace/SString.h"

class TAO_Notify_ThreadPool_Supplier;

/**
 * @class TAO_Notify_ThreadPool_Supplier_Client
 *
 * @brief Supplier Client
 */
class TAO_Notify_ThreadPool_Supplier_Client : public ACE_Task_Base
{
public:
  explicit TAO_Notify_ThreadPool_Supplier_Client (TAO_Notify_ORB_Objects& orb_objects);

  TAO_Notify_ThreadPool_Supplier_Client (TAO_Notify_ThreadPool_Supplier_Client const&) = delete;
  TAO_Notify_ThreadPool_Supplier_Client (TAO_Notify_ThreadPool_Supplier_Client&&) = delete;
  TAO_Notify_ThreadPool_Supplier_Client& operator= (TAO_Notify_ThreadPool_Supplier_Client const&) = delete;
  TAO_Notify_ThreadPool_Supplier_Client& operator= (TAO_Notify_ThreadPool_Supplier_Client&&) = delete;

  ~TAO_Notify_ThreadPool_Supplier_Client () override = default;

  /// Init
  void _init ();

  /// Run
  void run ();

  /// Parse Args
  int parse_args (int argc, ACE_TCHAR *argv[]);

  /// The thread entry point.
  int svc () override;

protected:
  /// Create an EC
  CosNotifyChannelAdmin::EventChannel_ptr create_ec ();

  /// Write ior to file.
  void write_ior ();

  /// ORB Objects.
  TAO_Notify_ORB_Objects& orb_objects_;

  // The POA retains ownership of this servant after implicit activation.
  TAO_Notify_ThreadPool_Supplier* supplier_ {};

  struct Options
  {
    int consumer_count {2};
    int event_channel_thread_count {};
    int proxy_consumer_thread_count {};
    int max_events {10};
    ACE_TString ior_file_name {ACE_TEXT ("supplier.ior")};
  };

  Options options_ {};
};

#include /**/ "ace/post.h"
#endif /* TAO_Notify_SUPPLIER_CLIENT_H */
