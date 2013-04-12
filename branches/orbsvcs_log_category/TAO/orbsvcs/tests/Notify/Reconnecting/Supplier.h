/* -*- C++ -*- */
//=============================================================================
/**
 *  @file   Supplier.h
 *
 *  $Id$
 *
 * Test Supplier reconnect to existing channel/admin/proxy Notification Service
 *
 *
 *  @author Dale Wilson <wilson_d@ociweb.com>
 */
//=============================================================================


#ifndef RECONNECTNG_SUPPLIER_H
#define RECONNECTNG_SUPPLIER_H

#include "orbsvcs/CosNotifyChannelAdminC.h"
#include "orbsvcs/CosNamingC.h"
#include "orbsvcs/NotifyExtS.h"
#include <ace/SString.h>

class Supplier_Main;

class StructuredPushSupplier_i : public virtual POA_CosNotifyComm::StructuredPushSupplier
{
  virtual void subscription_change (
      const CosNotification::EventTypeSeq & added,
      const CosNotification::EventTypeSeq & removed);

  virtual void disconnect_structured_push_supplier (void);
};

class SequencePushSupplier_i : public virtual POA_CosNotifyComm::SequencePushSupplier
{
  virtual void subscription_change (
      const CosNotification::EventTypeSeq & added,
      const CosNotification::EventTypeSeq & removed);

  virtual void disconnect_sequence_push_supplier (void);
};

class AnyPushSupplier_i : public virtual POA_CosNotifyComm::PushSupplier
{
  virtual void subscription_change (
      const CosNotification::EventTypeSeq & added,
      const CosNotification::EventTypeSeq & removed);

  virtual void disconnect_push_supplier (void);
};

class ReconnectionCallback_i : public virtual POA_NotifyExt::ReconnectionCallback
{
public:
  ReconnectionCallback_i (Supplier_Main & supplier_main);

  virtual ~ReconnectionCallback_i ();

  void init (
    PortableServer::POA_ptr poa,
    CosNotifyChannelAdmin::EventChannelFactory_ptr ecf_);

  void fini (void);

  size_t reconnect_count () const;

  virtual void reconnect (CORBA::Object_ptr reconnection);

  virtual CORBA::Boolean is_alive (void);

private:
  Supplier_Main & supplier_main_;
  bool id_is_valid_;
  NotifyExt::ReconnectionRegistry::ReconnectionID callback_id_;
  CosNotifyChannelAdmin::EventChannelFactory_var ecf_;
  size_t reconnect_count_;
};

class Supplier_Main
{
 public:
  Supplier_Main ();
  ~Supplier_Main ();

  int parse_args (int argc, ACE_TCHAR *argv[]);
  int parse_single_arg (int argc, ACE_TCHAR *argv[]);

  int init (int argc, ACE_TCHAR *argv[]);
  int run (void);
  int fini (void);
  void usage (FILE * log) const;

  void reconnect (
      CosNotifyChannelAdmin::EventChannelFactory_ptr dest_factory);

 private:
  /// Find naming service.
  int resolve_naming_service (void);

  /// Resolve the Notify factory from the Naming service.
  int find_notify_factory (void);

  /// Resolve the Notify factory using resolve_initial_reference ("NotifyEventChannelFactory")
  int resolve_notify_factory (void);

  void init_event_channel (void);
  void init_supplier_admin (void);
  void init_structured_proxy_consumer (void);
  void init_sequence_proxy_consumer (void);
  void init_any_proxy_consumer (void);

  /// send events.
  void send_structured_event (void);
  void send_sequence_event (void);
  void send_any_event (void);

  void save_ids();
  bool load_ids();

  ////////////////////
  // forbidden methods
 private:
    Supplier_Main (const Supplier_Main & rhs);
    Supplier_Main & operator = (const Supplier_Main & rhs);

  ////////////////
  // Data members
 private:

  // set by command line parameters
  bool verbose_;             // -v make a glorious noise
  enum Mode_T {
    MODE_ANY,
    MODE_STRUCTURED,
    MODE_SEQUENCE} mode_;    // -any, -structured, or -sequence
  ACE_TString channel_file_; // -channel filename
  size_t send_;              // -send n
  bool use_naming_service_;  // -nonamesvc
  size_t serial_number_;     // -serial_number
  bool disconnect_on_exit_;  // -disconnect
  ACE_TString id_file_;      // -ids
  size_t pause_;             // -pause n (pause after sending n messages)
  CORBA::ORB_var orb_;
  PortableServer::POA_var root_poa_;
  CosNaming::NamingContext_var naming_context_;
  CosNotifyChannelAdmin::EventChannelFactory_var ecf_;

  CosNotifyChannelAdmin::EventChannel_var ec_;
  CosNotifyChannelAdmin::ChannelID ec_id_;

  CosNotifyChannelAdmin::SupplierAdmin_var sa_;
  CosNotifyChannelAdmin::AdminID sa_id_;

  CosNotifyChannelAdmin::StructuredProxyPushConsumer_var structured_proxy_push_consumer_;
  CosNotifyChannelAdmin::ProxyID structured_proxy_id_;
  StructuredPushSupplier_i structured_push_supplier_;
  CosNotifyComm::StructuredPushSupplier_var structured_push_supplier_ref_;


  CosNotifyChannelAdmin::SequenceProxyPushConsumer_var sequence_proxy_push_consumer_;
  CosNotifyChannelAdmin::ProxyID sequence_proxy_id_;
  SequencePushSupplier_i sequence_push_supplier_;
  CosNotifyComm::SequencePushSupplier_var sequence_push_supplier_ref_;

  CosNotifyChannelAdmin::ProxyPushConsumer_var any_proxy_push_consumer_;
  CosNotifyChannelAdmin::ProxyID any_proxy_id_;
  AnyPushSupplier_i any_push_supplier_;
  CosNotifyComm::PushSupplier_var any_push_supplier_ref_;

  ReconnectionCallback_i reconnection_callback_;

  bool reconnecting_;
};



#endif /* RECONNECTNG_SUPPLIER_H */
