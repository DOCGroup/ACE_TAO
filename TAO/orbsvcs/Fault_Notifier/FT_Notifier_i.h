// -*- C++ -*-
//
// $Id$

// @@ OCI folks -- file header please...
#ifndef FT_NOTIFIER_I_H_
#define FT_NOTIFIER_I_H_

// @@OCI folks -- please include pre.h before the pragma. Oh, BTW gcc
// 3.* would complain that  pragma has been deprecated. Probably you
// should add a simple header file before the pragma.

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/pre.h"

#include <orbsvcs/FT_NotifierS.h>
#include <orbsvcs/Notify/Notify_EventChannelFactory_i.h>
#include <ace/Vector_t.h>

/////////////////////
// Forward references
class TAO_ORB_Manager;

// @@OCI folks, please use the TAO namespace

class  FT_FaultNotifier_i : public virtual POA_FT::FaultNotifier
{
  //////////////////////
  // non-CORBA interface
public:
  /**
   * Default constructor.
   */
  FT_FaultNotifier_i ();

  /**
   * Virtual destructor.
   */
  virtual ~FT_FaultNotifier_i ();


  /**
   * Parse command line arguments.
   * @param argc traditional C argc
   * @param argv traditional C argv
   * @return zero for success; nonzero is process return code for failure.
   */
  int parse_args (int argc, char * argv[]);

  /**
   * Initialize this object.
   * @param orbManager our ORB -- we keep var to it.
   * @return zero for success; nonzero is process return code for failure.
   */
  int init (TAO_ORB_Manager & orbManager ACE_ENV_ARG_DECL_WITH_DEFAULTS);

  /**
   * Prepare to exit
   * @return zero for success; nonzero is process return code for failure.
   */
  int fini (ACE_ENV_SINGLE_ARG_DECL);

  /**
   * Identify this fault notifier.
   * @return a string to identify this object for logging/console message purposes.
   */
  const char * identity () const;

  /**
   * idle time activity.
   * @param result [out] status code to return from process
   * @returns 0 to continue; nonzero to quit
   */
  int idle(int &result ACE_ENV_ARG_DECL_WITH_DEFAULTS);

  //////////////////
  // CORBA interface
  // See IDL for documentation

  //@@OCI folks, please don't use "WITH_DEFAULTS" since that makes a
  // TSS lookup, which is slower. Yes, IDL compile generates with
  //them, but the implementation should have the raw macro.

  virtual void push_structured_fault (
      const CosNotification::StructuredEvent & event
      ACE_ENV_ARG_DECL_WITH_DEFAULTS
    )
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual void push_sequence_fault (
    const CosNotification::EventBatch & events
    ACE_ENV_ARG_DECL_WITH_DEFAULTS
  )
  ACE_THROW_SPEC ((CORBA::SystemException));

  virtual ::CosNotifyFilter::Filter_ptr create_subscription_filter (
    const char * constraint_grammar
    ACE_ENV_ARG_DECL_WITH_DEFAULTS
  )
  ACE_THROW_SPEC ((CORBA::SystemException, CosNotifyFilter::InvalidGrammar));

  virtual FT::FaultNotifier::ConsumerId connect_structured_fault_consumer (
    CosNotifyComm::StructuredPushConsumer_ptr push_consumer,
    CosNotifyFilter::Filter_ptr filter
    ACE_ENV_ARG_DECL_WITH_DEFAULTS
  )
  ACE_THROW_SPEC ((CORBA::SystemException));

  virtual FT::FaultNotifier::ConsumerId connect_sequence_fault_consumer (
    CosNotifyComm::SequencePushConsumer_ptr push_consumer,
    CosNotifyFilter::Filter_ptr filter
    ACE_ENV_ARG_DECL_WITH_DEFAULTS
  )
  ACE_THROW_SPEC ((CORBA::SystemException));

  virtual void disconnect_consumer (
    FT::FaultNotifier::ConsumerId connection
    ACE_ENV_ARG_DECL_WITH_DEFAULTS
  )
  ACE_THROW_SPEC ((CORBA::SystemException, CosEventComm::Disconnected));

  //////////////////////////////////////////
  // CORBA interface PullMonitorable methods
  virtual CORBA::Boolean is_alive (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
  ACE_THROW_SPEC ((CORBA::SystemException));

  /////////////////////////
  // Implementation methods
private:
  /**
   * Write this notifier's IOR to a file
   */
  int write_IOR ();

  ///////////////
  // Data Members
private:

  /**
   * Protect internal state.
   * Mutex should be locked by corba methods, or by
   * external (public) methods before calling implementation
   * methods.
   * Implementation methods should assume the mutex is
   * locked if necessary.
   */
  ACE_Mutex internals_;
  typedef ACE_Guard<ACE_Mutex> InternalGuard;

  /**
   * The orb
   */
  CORBA::ORB_var orb_;

  /**
   * IOR of this object as assigned by orb.
   */
  CORBA::String_var ior_;

  /**
   * A file to which the notifier's IOR should be written.
   */
  const char * ior_output_file_;

  /**
   * A name to be used to register the notifier with the name service.
   */
  const char * nsName_;

  CosNaming::NamingContext_var naming_context_;

  CosNaming::Name this_name_;

  /**
   * A human-readable string to distinguish this from other Notifiers.
   */
  ACE_CString identity_;

  /////////////////////////////
  //
  struct ProxyInfo
  {
    ::CosNotifyChannelAdmin::ProxyID proxyId_;
    ::CosNotifyChannelAdmin::ProxySupplier_var proxyVar_;

    ProxyInfo ();
    ProxyInfo (const ProxyInfo & rhs);
  };

  typedef ACE_Vector <ProxyInfo> ProxyInfoVec;

  ProxyInfoVec proxyInfos_;

  size_t consumerConnects_;
  size_t consumerDisconnects_;

/////////////////////
  ::CosNotifyChannelAdmin::ChannelID channelId_;
  ::CosNotifyChannelAdmin::EventChannel_var notify_channel_;
  ::CosNotifyFilter::FilterFactory_var filter_factory_;
  ::CosNotifyChannelAdmin::SupplierAdmin_var supplierAdmin_;
  ::CosNotifyChannelAdmin::ConsumerAdmin_var consumerAdmin_;

  ::CosNotifyChannelAdmin::StructuredProxyPushConsumer_var structuredProxyPushConsumer_;
  ::CosNotifyChannelAdmin::SequenceProxyPushConsumer_var sequenceProxyPushConsumer_;

  /**
    * boolean true means display debug messages (default is false)
    */
  int verbose_;

  /*
   * boolean quit when all consumers disconnect
   */
  int quitOnIdle_;

};

#include "ace/post.h"

#endif /* FT_NOTIFIER_I_H_  */
