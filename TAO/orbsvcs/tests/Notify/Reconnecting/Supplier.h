/* -*- C++ -*- */
//=============================================================================
/**
 *  @file   Supplier.h
 *
 * Test Supplier reconnect to existing channel/admin/proxy Notification Service
 *
 *  @author Dale Wilson <wilson_d@ociweb.com>
 */
//=============================================================================


#ifndef RECONNECTING_SUPPLIER_H
#define RECONNECTING_SUPPLIER_H

#include "orbsvcs/CosNotifyChannelAdminC.h"
#include "orbsvcs/CosNamingC.h"
#include "orbsvcs/NotifyExtS.h"
#include <ace/SString.h>

class Supplier_Main;

class StructuredPushSupplier_i : public virtual POA_CosNotifyComm::StructuredPushSupplier
{
public:
  StructuredPushSupplier_i () = default;
  StructuredPushSupplier_i (StructuredPushSupplier_i const&) = delete;
  StructuredPushSupplier_i (StructuredPushSupplier_i&&) = delete;
  StructuredPushSupplier_i& operator= (StructuredPushSupplier_i const&) = delete;
  StructuredPushSupplier_i& operator= (StructuredPushSupplier_i&&) = delete;

private:
  void subscription_change (
      CosNotification::EventTypeSeq const& added,
      CosNotification::EventTypeSeq const& removed) override;

  void disconnect_structured_push_supplier () override;
};

class SequencePushSupplier_i : public virtual POA_CosNotifyComm::SequencePushSupplier
{
public:
  SequencePushSupplier_i () = default;
  SequencePushSupplier_i (SequencePushSupplier_i const&) = delete;
  SequencePushSupplier_i (SequencePushSupplier_i&&) = delete;
  SequencePushSupplier_i& operator= (SequencePushSupplier_i const&) = delete;
  SequencePushSupplier_i& operator= (SequencePushSupplier_i&&) = delete;

private:
  void subscription_change (
      CosNotification::EventTypeSeq const& added,
      CosNotification::EventTypeSeq const& removed) override;

  void disconnect_sequence_push_supplier () override;
};

class AnyPushSupplier_i : public virtual POA_CosNotifyComm::PushSupplier
{
public:
  AnyPushSupplier_i () = default;
  AnyPushSupplier_i (AnyPushSupplier_i const&) = delete;
  AnyPushSupplier_i (AnyPushSupplier_i&&) = delete;
  AnyPushSupplier_i& operator= (AnyPushSupplier_i const&) = delete;
  AnyPushSupplier_i& operator= (AnyPushSupplier_i&&) = delete;

private:
  void subscription_change (
      CosNotification::EventTypeSeq const& added,
      CosNotification::EventTypeSeq const& removed) override;

  void disconnect_push_supplier () override;
};

class ReconnectionCallback_i : public virtual POA_NotifyExt::ReconnectionCallback
{
public:
  explicit ReconnectionCallback_i (Supplier_Main& supplier_main);

  ~ReconnectionCallback_i () override;

  ReconnectionCallback_i (ReconnectionCallback_i const&) = delete;
  ReconnectionCallback_i (ReconnectionCallback_i&&) = delete;
  ReconnectionCallback_i& operator= (ReconnectionCallback_i const&) = delete;
  ReconnectionCallback_i& operator= (ReconnectionCallback_i&&) = delete;

  void init (
    PortableServer::POA_ptr poa,
    CosNotifyChannelAdmin::EventChannelFactory_ptr ecf_);

  void fini ();

  size_t reconnect_count () const;

  void reconnect (CORBA::Object_ptr reconnection) override;

  CORBA::Boolean is_alive () override;

private:
  Supplier_Main& supplier_main_;
  bool id_is_valid_ {};
  NotifyExt::ReconnectionRegistry::ReconnectionID callback_id_ {};
  CosNotifyChannelAdmin::EventChannelFactory_var ecf_;
  size_t reconnect_count_ {};
};

class Supplier_Main
{
 public:
  Supplier_Main ();
  ~Supplier_Main () = default;

  Supplier_Main (Supplier_Main const&) = delete;
  Supplier_Main (Supplier_Main&&) = delete;
  Supplier_Main& operator= (Supplier_Main const&) = delete;
  Supplier_Main& operator= (Supplier_Main&&) = delete;

  int parse_args (int argc, ACE_TCHAR *argv[]);
  int parse_single_arg (int argc, ACE_TCHAR *argv[]);

  int init (int argc, ACE_TCHAR *argv[]);
  int run ();
  int fini ();
  void usage (FILE * log) const;

  void reconnect (
      CosNotifyChannelAdmin::EventChannelFactory_ptr dest_factory);

 private:
  /// Find naming service.
  int resolve_naming_service ();

  /// Resolve the Notify factory from the Naming service.
  int find_notify_factory ();

  /// Resolve the Notify factory using resolve_initial_reference ("NotifyEventChannelFactory")
  int resolve_notify_factory ();

  void init_event_channel ();
  void init_supplier_admin ();
  void init_structured_proxy_consumer ();
  void init_sequence_proxy_consumer ();
  void init_any_proxy_consumer ();

  /// send events.
  void send_structured_event ();
  void send_sequence_event ();
  void send_any_event ();

  void save_ids();
  bool load_ids();

  // Data members
 private:
  // set by command line parameters
  bool verbose_ {};          // -v make a glorious noise
  enum Mode_T {
    MODE_ANY,
    MODE_STRUCTURED,
    MODE_SEQUENCE} mode_ {MODE_ANY}; // -any, -structured, or -sequence
  ACE_TString channel_file_ {}; // -channel filename
  size_t send_ {10};         // -send n
  bool use_naming_service_ {true}; // -nonamesvc
  size_t serial_number_ {};  // -serial_number
  bool disconnect_on_exit_ {}; // -disconnect
  ACE_TString id_file_ {ACE_TEXT ("supplier.ids")}; // -ids
  size_t pause_ {};          // -pause n (pause after sending n messages)
  CORBA::ORB_var orb_;
  PortableServer::POA_var root_poa_;
  CosNaming::NamingContext_var naming_context_;
  CosNotifyChannelAdmin::EventChannelFactory_var ecf_;

  CosNotifyChannelAdmin::EventChannel_var ec_;
  CosNotifyChannelAdmin::ChannelID ec_id_ {};

  CosNotifyChannelAdmin::SupplierAdmin_var sa_;
  CosNotifyChannelAdmin::AdminID sa_id_ {};

  CosNotifyChannelAdmin::StructuredProxyPushConsumer_var structured_proxy_push_consumer_;
  CosNotifyChannelAdmin::ProxyID structured_proxy_id_ {};
  StructuredPushSupplier_i structured_push_supplier_;
  CosNotifyComm::StructuredPushSupplier_var structured_push_supplier_ref_;


  CosNotifyChannelAdmin::SequenceProxyPushConsumer_var sequence_proxy_push_consumer_;
  CosNotifyChannelAdmin::ProxyID sequence_proxy_id_ {};
  SequencePushSupplier_i sequence_push_supplier_;
  CosNotifyComm::SequencePushSupplier_var sequence_push_supplier_ref_;

  CosNotifyChannelAdmin::ProxyPushConsumer_var any_proxy_push_consumer_;
  CosNotifyChannelAdmin::ProxyID any_proxy_id_ {};
  AnyPushSupplier_i any_push_supplier_;
  CosNotifyComm::PushSupplier_var any_push_supplier_ref_;

  ReconnectionCallback_i reconnection_callback_;

  bool reconnecting_ {};
};


#endif /* RECONNECTING_SUPPLIER_H */
