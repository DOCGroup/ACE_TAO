//=============================================================================
/**
 *  @file   Notify_ProxyConsumer_T.h
 *
 *  $Id$
 *
 * Template Base class for all Proxy Consumers.
 *
 *
 *  @author Pradeep Gore <pradeep@cs.wustl.edu>
 */
//=============================================================================


#ifndef TAO_NOTIFY_PROXYCONSUMER_T_H
#define TAO_NOTIFY_PROXYCONSUMER_T_H
#include "ace/pre.h"
#include "Notify_Proxy_T.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "Notify_Listeners.h"

class TAO_Notify_SupplierAdmin_i;

#if defined (_MSC_VER)
#if (_MSC_VER >= 1200)
#pragma warning (push)
#endif /* _MSC_VER >= 1200 */
#pragma warning (disable:4250)
#endif /* _MSC_VER */

/**
 * @class TAO_Notify_ProxyConsumer
 *
 * @brief TAO_Notify_ProxyConsumer
 *
 * The is a base class for all proxy consumers.
 */
template <class SERVANT_TYPE>
class TAO_Notify_Export TAO_Notify_ProxyConsumer : public TAO_Notify_Proxy<SERVANT_TYPE>, virtual public TAO_Notify_EventSource
{

public:
  /// Constructor
  TAO_Notify_ProxyConsumer (TAO_Notify_SupplierAdmin_i* supplier_admin);

  /// Destructor
  virtual ~TAO_Notify_ProxyConsumer (void);

  /// Init the Proxy.
  void init (CosNotifyChannelAdmin::ProxyID myID ACE_ENV_ARG_DECL);

  // = TAO_Notify_EventSource methods.
  /// Evaluates true if this event is acceptable by the Source.
  virtual CORBA::Boolean evaluate_filter (TAO_Notify_Event &event ACE_ENV_ARG_DECL);

  /// The Worker task associated with the event listener for filter evaluation.
  TAO_Notify_Worker_Task* filter_eval_task (void);

  virtual CosNotifyChannelAdmin::SupplierAdmin_ptr MyAdmin (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((
                     CORBA::SystemException
                     ));

  virtual CosNotification::EventTypeSeq * obtain_subscription_types (
    CosNotifyChannelAdmin::ObtainInfoMode mode
    ACE_ENV_ARG_DECL
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ));

  virtual void offer_change (
    const CosNotification::EventTypeSeq & added,
    const CosNotification::EventTypeSeq & removed
    ACE_ENV_ARG_DECL
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException,
    CosNotifyComm::InvalidEventType
  ));

  // Override the set_qos that comes from TAO_Notify_Proxy
  // so we can update the qos on our tasks.
  virtual void set_qos (
    const CosNotification::QoSProperties & qos
    ACE_ENV_ARG_DECL
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException,
    CosNotification::UnsupportedQoS
  ));

protected:
  // = Helper methods
  /// Derived classes should call this when their suppliers connect.
  void on_connected (ACE_ENV_SINGLE_ARG_DECL);

  /// Derived classes should call this when their suppliers disconnect.
  void on_disconnected (ACE_ENV_SINGLE_ARG_DECL);

  // = Data members
  /// My parent supplier admin.
  TAO_Notify_SupplierAdmin_i* supplier_admin_;

  /// The filter evaluation task for this listener.
  TAO_Notify_Worker_Task* filter_eval_task_;
};

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "Notify_ProxyConsumer_T.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("Notify_ProxyConsumer_T.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#if defined (_MSC_VER) && (_MSC_VER >= 1200)
#pragma warning (pop)
#endif /* _MSC_VER */

#include "ace/post.h"
#endif /* TAO_NOTIFY_PROXYPUSHCONSUMER_T_H */
