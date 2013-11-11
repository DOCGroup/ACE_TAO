/* -*- C++ -*- */
//=============================================================================
/**
 *  @file   Consumer.h
 *
 *  $Id$
 *
 * Test Consumer reconnect to existing channel/admin/proxy Notification Service
 *
 *
 *  @author Dale Wilson <wilson_d@ociweb.com>
 */
//=============================================================================


#ifndef RECONNECTNG_CONSUMER_H
#define RECONNECTNG_CONSUMER_H

#include "orbsvcs/CosNotifyChannelAdminC.h"
#include "orbsvcs/CosNamingC.h"
#include "orbsvcs/NotifyExtS.h"
#include "ace/SString.h"

class Consumer_Main;

class StructuredPushConsumer_i : public virtual POA_CosNotifyComm::StructuredPushConsumer
{
public:
  StructuredPushConsumer_i ();
  virtual void offer_change (
      const CosNotification::EventTypeSeq & added,
      const CosNotification::EventTypeSeq & removed
    );

  virtual void push_structured_event (
        const CosNotification::StructuredEvent & notification
      );

  virtual void disconnect_structured_push_consumer (
    );

  size_t received () const;
  void set_expectations (size_t expecte, size_t fail, size_t serial_number, bool verbose);
  bool has_problem () const;
  void set_connected (bool flag);
  bool is_connected () const;

private:
  void check_serial_number (CORBA::ULong seq);

private:
  bool verbose_;
  size_t received_;
  size_t expect_;
  size_t fail_;
  size_t may_discard_;
  size_t serial_number_;
  size_t exceptions_thrown_;
  bool problem_;
  bool connected_;
};

class SequencePushConsumer_i : public virtual POA_CosNotifyComm::SequencePushConsumer
{
public:
  SequencePushConsumer_i ();
  virtual void offer_change (
      const CosNotification::EventTypeSeq & added,
      const CosNotification::EventTypeSeq & removed
    );

  virtual void push_structured_events (
        const CosNotification::EventBatch & notifications
      );

  virtual void disconnect_sequence_push_consumer (
    );

  size_t received () const;
  void set_expectations (size_t expecte, size_t fail, size_t serial_number, bool verbose);
  bool has_problem () const;
  void set_connected (bool flag);
  bool is_connected () const;

private:
  void check_serial_number (CORBA::ULong seq);

private:
  bool verbose_;
  size_t received_;
  size_t expect_;
  size_t fail_;
  size_t may_discard_;
  size_t serial_number_;
  size_t exceptions_thrown_;
  bool problem_;
  bool connected_;
};

class AnyPushConsumer_i : public virtual POA_CosNotifyComm::PushConsumer
{
public:
  AnyPushConsumer_i ();

  virtual void offer_change (
      const CosNotification::EventTypeSeq & added,
      const CosNotification::EventTypeSeq & removed
    );

  virtual void push (
        const CORBA::Any & data
      );

  virtual void disconnect_push_consumer (
    );

  size_t received () const;
  void set_expectations (size_t expecte, size_t fail, size_t serial_number, bool verbose);
  bool has_problem () const;
  void set_connected (bool flag);
  bool is_connected () const;

private:
  void check_serial_number (CORBA::ULong seq);

private:
  bool verbose_;
  size_t received_;
  size_t expect_;
  size_t fail_;
  size_t may_discard_;
  size_t serial_number_;
  size_t exceptions_thrown_;
  bool problem_;
  bool connected_;
};

class ReconnectionCallback_i : public virtual POA_NotifyExt::ReconnectionCallback
{
public:
  ReconnectionCallback_i (Consumer_Main & consumer_main);

  virtual ~ReconnectionCallback_i ();

  void init (
    PortableServer::POA_ptr poa,
    CosNotifyChannelAdmin::EventChannelFactory_ptr ecf_);

  void fini (void);

  size_t reconnect_count () const;

  virtual void reconnect (CORBA::Object_ptr reconnection);

  virtual CORBA::Boolean is_alive (void);

private:
  Consumer_Main & consumer_main_;
  bool id_is_valid_;
  NotifyExt::ReconnectionRegistry::ReconnectionID callback_id_;
  CosNotifyChannelAdmin::EventChannelFactory_var ecf_;
  size_t reconnect_count_;
};

class Consumer_Main
{
 public:
  Consumer_Main ();
  ~Consumer_Main ();

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
  void init_consumer_admin (void);
  void init_structured_proxy_supplier (void);
  void init_sequence_proxy_supplier (void);
  void init_any_proxy_supplier (void);

  void set_proxy_qos (CosNotifyChannelAdmin::ProxySupplier_ptr proxy);

  void save_ids();
  bool load_ids();

  ////////////////////
  // forbidden methods
 private:
    Consumer_Main (const Consumer_Main & rhs);
    Consumer_Main & operator = (const Consumer_Main & rhs);

  ////////////////
  // Data members
 private:

  // set by command line parameters
  bool verbose_;            // -v make a glorious noise
  enum Mode_T{
    MODE_ANY,
    MODE_STRUCTURED,
    MODE_SEQUENCE} mode_;   // -any, -structured, or -sequence
  ACE_TString channel_file_; // -channel fileneame
  size_t expect_;           // -expect n
  size_t fail_;             // -fail n
  bool use_naming_service_; // -nonamesvc
  size_t serial_number_;    // -serial_number
  bool disconnect_on_exit_; // -disconnect
  size_t structured_count_;
  size_t sequence_count_;
  size_t any_count_;

  ACE_CString id_file_;     // -ids
  CORBA::ORB_var orb_;
  PortableServer::POA_var root_poa_;
  CosNaming::NamingContext_var naming_context_;
  CosNotifyChannelAdmin::EventChannelFactory_var ecf_;

  CosNotifyChannelAdmin::EventChannel_var ec_;
  CosNotifyChannelAdmin::ChannelID ec_id_;

  CosNotifyChannelAdmin::ConsumerAdmin_var sa_;
  CosNotifyChannelAdmin::AdminID sa_id_;

  CosNotifyChannelAdmin::StructuredProxyPushSupplier_var structured_proxy_push_supplier_;
  CosNotifyChannelAdmin::ProxyID structured_proxy_id_;
  StructuredPushConsumer_i structured_push_consumer_;
  CosNotifyComm::StructuredPushConsumer_var structured_push_consumer_ref_;

  CosNotifyChannelAdmin::SequenceProxyPushSupplier_var sequence_proxy_push_supplier_;
  CosNotifyChannelAdmin::ProxyID sequence_proxy_id_;
  SequencePushConsumer_i sequence_push_consumer_;
  CosNotifyComm::SequencePushConsumer_var sequence_push_consumer_ref_;

  CosNotifyChannelAdmin::ProxyPushSupplier_var any_proxy_push_supplier_;
  CosNotifyChannelAdmin::ProxyID any_proxy_id_;
  AnyPushConsumer_i any_push_consumer_;
  CosNotifyComm::PushConsumer_var any_push_consumer_ref_;

  ReconnectionCallback_i reconnection_callback_;

  bool reconnecting_;
};

#endif /* RECONNECTNG_CONSUMER_H */
