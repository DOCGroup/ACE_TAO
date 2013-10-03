/* -*- C++ -*- */
/**
 *  @file Supplier_Client.h
 *
 *  $Id$
 *
 *  @author Pradeep Gore <pradeep@oomworks.com>
 *
 *
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
 *
 */
class TAO_Notify_ThreadPool_Supplier_Client : public ACE_Task_Base
{
public:
  /// Constructor
  TAO_Notify_ThreadPool_Supplier_Client (TAO_Notify_ORB_Objects& orb_objects);

  /// Destructor
  ~TAO_Notify_ThreadPool_Supplier_Client ();

  /// Init
  void _init (void);

  /// Run
  void run (void);

  /// Parse Args
  int parse_args (int argc, ACE_TCHAR *argv[]);

  /// The thread entry point.
  virtual int svc (void);

protected:
  /// Create an EC
  CosNotifyChannelAdmin::EventChannel_ptr create_ec (void);

  /// Write ior to file.
  void write_ior (void);

  /// ORB Objects.
  TAO_Notify_ORB_Objects orb_objects_;

  /// Supplier that sends events.
  TAO_Notify_ThreadPool_Supplier* supplier_;

  /// The Number of consumers that we expect to send an event to.
  int consumer_count_;

  /// Name of the file to write the supplier ior to.
  ACE_TString ior_file_name_;

  /// Number of threads at the EC
  int ec_thread_count_;

  /// Number of Therads at the ProxyConsumer.
  int proxy_consumer_thread_count_;

  // Max events to send.
  int max_events_;
};

#include /**/ "ace/post.h"
#endif /* TAO_Notify_SUPPLIER_CLIENT_H */
