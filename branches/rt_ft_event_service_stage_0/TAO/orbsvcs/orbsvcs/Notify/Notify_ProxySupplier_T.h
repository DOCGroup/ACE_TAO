/**
 * @file Notify_ProxySupplier_T.h
 *
 * $Id$
 *
 * Template Base class for all Proxy Suppliers.
 *
 * @author Pradeep Gore <pradeep@cs.wustl.edu>
 */

#ifndef TAO_NOTIFY_PROXY_SUPPLIER_T_H
#define TAO_NOTIFY_PROXY_SUPPLIER_T_H
#include "ace/pre.h"

#include "Notify_Proxy_T.h"
#include "Notify_Collection.h"

class TAO_Notify_ConsumerAdmin_i;

#if defined (_MSC_VER)
#if (_MSC_VER >= 1200)
#pragma warning (push)
#endif /* _MSC_VER >= 1200 */
#pragma warning (disable:4250)
#endif /* _MSC_VER */

/**
 * @class TAO_Notify_ProxySupplier
 *
 * @brief TAO_Notify_ProxySupplier
 *
 * The is a base class for all proxy suppliers.
 */
template <class SERVANT_TYPE>
class TAO_Notify_Export TAO_Notify_ProxySupplier : public TAO_Notify_Proxy <SERVANT_TYPE>, virtual public TAO_Notify_EventListener
{

public:
  /// Constructor
  TAO_Notify_ProxySupplier (TAO_Notify_ConsumerAdmin_i* consumeradmin);

  /// Destructor
  virtual ~TAO_Notify_ProxySupplier (void);

  /// Init the Proxy.
  void init (CosNotifyChannelAdmin::ProxyID myID ACE_ENV_ARG_DECL);

  // = Notify_Event_Listener methods
  virtual void dispatch_event (TAO_Notify_Event &event ACE_ENV_ARG_DECL);

  virtual CORBA::Boolean evaluate_filter (TAO_Notify_Event &event, CORBA::Boolean eval_parent ACE_ENV_ARG_DECL);

  /// The Worker task associated with the event listener for event dispatching
  virtual TAO_Notify_Worker_Task* event_dispatch_task (void);

  /// The Worker task associated with the event listener for filter evaluation.
  virtual TAO_Notify_Worker_Task* filter_eval_task (void);

  // = Interface methods
  virtual CosNotifyChannelAdmin::ConsumerAdmin_ptr MyAdmin (
    ACE_ENV_SINGLE_ARG_DECL
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ));

  virtual void suspend_connection (
    ACE_ENV_SINGLE_ARG_DECL
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException,
    CosNotifyChannelAdmin::ConnectionAlreadyInactive,
    CosNotifyChannelAdmin::NotConnected
  ));

  virtual void resume_connection (
    ACE_ENV_SINGLE_ARG_DECL
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException,
    CosNotifyChannelAdmin::ConnectionAlreadyActive,
    CosNotifyChannelAdmin::NotConnected
  ));

  virtual CosNotifyFilter::MappingFilter_ptr priority_filter (
    ACE_ENV_SINGLE_ARG_DECL
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ));

  virtual void priority_filter (
    CosNotifyFilter::MappingFilter_ptr priority_filter
    ACE_ENV_ARG_DECL
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ));

  virtual CosNotifyFilter::MappingFilter_ptr lifetime_filter (
    ACE_ENV_SINGLE_ARG_DECL
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ));

  virtual void lifetime_filter (
    CosNotifyFilter::MappingFilter_ptr lifetime_filter
    ACE_ENV_ARG_DECL
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ));

  virtual CosNotification::EventTypeSeq * obtain_offered_types (
    CosNotifyChannelAdmin::ObtainInfoMode mode
    ACE_ENV_ARG_DECL
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ));

  virtual void subscription_change (
    const CosNotification::EventTypeSeq & added,
    const CosNotification::EventTypeSeq & removed
    ACE_ENV_ARG_DECL
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException,
    CosNotifyComm::InvalidEventType
  ));

  // override the set_qos that comes from TAO_Notify_Proxy
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
  /// Derived classes should implement this.
  virtual void dispatch_event_i (TAO_Notify_Event &event ACE_ENV_ARG_DECL) = 0;

  /// Derived classes should call this when their consumers connect.
  void on_connected (ACE_ENV_SINGLE_ARG_DECL);

  /// Derived classes should call this when their consumers disconnect.
  void on_disconnected (ACE_ENV_SINGLE_ARG_DECL);

  // = Data members
  /// My parent consumer admin.
  TAO_Notify_ConsumerAdmin_i* consumer_admin_;

  /// A list of event types that we are interested in.
  TAO_Notify_EventType_List subscription_list_;

  /// True if we are connected to a consumer and suspended.
  CORBA::Boolean is_suspended_;

  /// A list of events populated when we're suspended.
  typedef ACE_Unbounded_Queue<TAO_Notify_Event*> TAO_Notify_Event_List;
  TAO_Notify_Event_List event_list_;

  /// The dispatching task to send events to a listener group affiliated with this listener.
  TAO_Notify_Worker_Task* dispatching_task_;

  /// The filter evaluation task for this listener.
  TAO_Notify_Worker_Task* filter_eval_task_;
};

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "Notify_ProxySupplier_T.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("Notify_ProxySupplier_T.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#if defined (_MSC_VER) && (_MSC_VER >= 1200)
#pragma warning (pop)
#endif /* _MSC_VER */

#include "ace/post.h"
#endif /* TAO_NOTIFY_PROXY_SUPPLIER_T_H */
