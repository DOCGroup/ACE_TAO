// -*- C++ -*-
//
// $Id$
//=============================================================================
/**
 *  @file    Fault_Notifier_i.h
 *
 *  $Id$
 *
 *  This file is part of Fault Tolerant CORBA.
 *  This file declares the Fault_Notifier_i class.
 *  A FaultNotifer is a subset of a notification channel.  It allows interested
 *  parties to subscribe to fault notifications.  The most likely interested party
 *  is the ReplicationManager.
 *
 *  @author Dale Wilson <wilson_d@ociweb.com>
 */
//=============================================================================

#ifndef TAO_FT_NOTIFIER_I_H_
#define TAO_FT_NOTIFIER_I_H_
#include /**/ "ace/pre.h"
#include "ace/ACE.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

//////////////////////////////////
// Classes declared in this header
namespace TAO
{
  class  FT_FaultNotifier_i;
}


#include <orbsvcs/FT_NotifierS.h>
#include <orbsvcs/Notify/Notify_EventChannelFactory_i.h>
#include <ace/Vector_t.h>

/////////////////////
// Forward references

namespace TAO
{
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
    int init (CORBA::ORB_var & orb ACE_ENV_ARG_DECL);

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
    int idle(int &result ACE_ENV_ARG_DECL);

    //////////////////
    // CORBA interface
    // See IDL for documentation

    virtual void push_structured_fault (
        const CosNotification::StructuredEvent & event
        ACE_ENV_ARG_DECL
      )
      ACE_THROW_SPEC ((CORBA::SystemException));

    virtual void push_sequence_fault (
      const CosNotification::EventBatch & events
      ACE_ENV_ARG_DECL
    )
    ACE_THROW_SPEC ((CORBA::SystemException));

    virtual ::CosNotifyFilter::Filter_ptr create_subscription_filter (
      const char * constraint_grammar
      ACE_ENV_ARG_DECL
    )
    ACE_THROW_SPEC ((CORBA::SystemException, CosNotifyFilter::InvalidGrammar));

    virtual FT::FaultNotifier::ConsumerId connect_structured_fault_consumer (
      CosNotifyComm::StructuredPushConsumer_ptr push_consumer,
      CosNotifyFilter::Filter_ptr filter
      ACE_ENV_ARG_DECL
    )
    ACE_THROW_SPEC ((CORBA::SystemException));

    virtual FT::FaultNotifier::ConsumerId connect_sequence_fault_consumer (
      CosNotifyComm::SequencePushConsumer_ptr push_consumer,
      CosNotifyFilter::Filter_ptr filter
      ACE_ENV_ARG_DECL
    )
    ACE_THROW_SPEC ((CORBA::SystemException));

    virtual void disconnect_consumer (
      FT::FaultNotifier::ConsumerId connection
      ACE_ENV_ARG_DECL
    )
    ACE_THROW_SPEC ((CORBA::SystemException, CosEventComm::Disconnected));

    //////////////////////////////////////////
    // CORBA interface PullMonitorable methods
    virtual CORBA::Boolean is_alive (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));


    /////////////////////////////////////////
    // Override CORBA servant virtual methods
    virtual PortableServer::POA_ptr _default_POA (ACE_ENV_SINGLE_ARG_DECL);

    virtual void _remove_ref (ACE_ENV_SINGLE_ARG_DECL);

    /////////////////////////
    // Implementation methods
  private:
    /**
     * Write this notifier's IOR to a file
     */
    int write_ior ();

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
    ACE_SYNCH_MUTEX internals_;
    typedef ACE_Guard<ACE_SYNCH_MUTEX> InternalGuard;

    /**
     * The orb 
     */
    CORBA::ORB_var orb_;

    /**
     * The POA used to activate this object.
     */
    PortableServer::POA_var poa_;

    /**
     * The CORBA object id assigned to this object.
     */
    PortableServer::ObjectId_var object_id_;


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
    const char * ns_name_;

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

    ProxyInfoVec proxy_infos_;

    size_t consumer_connects_;
    size_t consumer_disconnects_;

    ::CosNotifyChannelAdmin::ChannelID channel_id_;
    ::CosNotifyChannelAdmin::EventChannel_var notify_channel_;
    ::CosNotifyFilter::FilterFactory_var filter_factory_;
    ::CosNotifyChannelAdmin::SupplierAdmin_var supplier_admin_;
    ::CosNotifyChannelAdmin::ConsumerAdmin_var consumer_admin_;

    ::CosNotifyChannelAdmin::StructuredProxyPushConsumer_var structured_proxy_push_consumer_;
    ::CosNotifyChannelAdmin::SequenceProxyPushConsumer_var sequence_proxy_push_consumer_;

    /**
      * boolean true means display debug messages (default is false)
      */
    int verbose_;

    /*
     * boolean quit when all consumers disconnect
     */
    int quit_on_idle_;

    /**
     * boolean: set true we've asked CORBA to deactivate
     */
    int quitting_;

    /**
     * boolean: set true when CORBA is done with this object.
     */
    int gone_;

  };
} // namespace TAO
#include /**/ "ace/post.h"

#endif /* TAO_FT_NOTIFIER_I_H_  */
