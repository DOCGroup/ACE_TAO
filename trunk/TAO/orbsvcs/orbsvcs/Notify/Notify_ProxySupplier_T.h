// $Id$
// =========================================================================
//
// = LIBRARY
//   orbsvcs
//
// = FILENAME
//   Notify_ProxySupplier.h
//
// = DESCRIPTION
//   Template Base class for all Proxy Suppliers.
//
// = AUTHOR
//    Pradeep Gore <pradeep@cs.wustl.edu>
//
// ==========================================================================
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

template <class SERVANT_TYPE>
class TAO_Notify_Export TAO_Notify_ProxySupplier : public TAO_Notify_Proxy <SERVANT_TYPE>, virtual public TAO_Notify_EventListener
{
  // = TITLE
  //   TAO_Notify_ProxySupplier
  //
  // = DESCRIPTION
  //   The is a base class for all proxy suppliers.
  //

public:
  TAO_Notify_ProxySupplier (TAO_Notify_ConsumerAdmin_i* consumeradmin);
  // Constructor

  virtual ~TAO_Notify_ProxySupplier (void);
  // Destructor

  void init (CosNotifyChannelAdmin::ProxyID myID TAO_ENV_ARG_DECL);
  // Init the Proxy.

  // = Notify_Event_Listener methods
  virtual void dispatch_event (TAO_Notify_Event &event TAO_ENV_ARG_DECL);

  virtual CORBA::Boolean evaluate_filter (TAO_Notify_Event &event, CORBA::Boolean eval_parent TAO_ENV_ARG_DECL);

  virtual TAO_Notify_Worker_Task* event_dispatch_task (void);
  // The Worker task associated with the event listener for event dispatching

  virtual TAO_Notify_Worker_Task* filter_eval_task (void);
  // The Worker task associated with the event listener for filter evaluation.

  // = Interface methods
  virtual CosNotifyChannelAdmin::ConsumerAdmin_ptr MyAdmin (
    TAO_ENV_SINGLE_ARG_DECL
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ));

  virtual void suspend_connection (
    TAO_ENV_SINGLE_ARG_DECL
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException,
    CosNotifyChannelAdmin::ConnectionAlreadyInactive,
    CosNotifyChannelAdmin::NotConnected
  ));

  virtual void resume_connection (
    TAO_ENV_SINGLE_ARG_DECL
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException,
    CosNotifyChannelAdmin::ConnectionAlreadyActive,
    CosNotifyChannelAdmin::NotConnected
  ));

  virtual CosNotifyFilter::MappingFilter_ptr priority_filter (
    TAO_ENV_SINGLE_ARG_DECL
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ));

  virtual void priority_filter (
    CosNotifyFilter::MappingFilter_ptr priority_filter
    TAO_ENV_ARG_DECL
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ));

  virtual CosNotifyFilter::MappingFilter_ptr lifetime_filter (
    TAO_ENV_SINGLE_ARG_DECL
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ));

  virtual void lifetime_filter (
    CosNotifyFilter::MappingFilter_ptr lifetime_filter
    TAO_ENV_ARG_DECL
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ));

  virtual CosNotification::EventTypeSeq * obtain_offered_types (
    CosNotifyChannelAdmin::ObtainInfoMode mode
    TAO_ENV_ARG_DECL
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ));

  virtual void subscription_change (
    const CosNotification::EventTypeSeq & added,
    const CosNotification::EventTypeSeq & removed
    TAO_ENV_ARG_DECL
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException,
    CosNotifyComm::InvalidEventType
  ));

  // override the set_qos that comes from TAO_Notify_Proxy
  // so we can update the qos on our tasks.
  virtual void set_qos (
    const CosNotification::QoSProperties & qos
    TAO_ENV_ARG_DECL
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException,
    CosNotification::UnsupportedQoS
  ));

 protected:
  // = Helper methods
  virtual void dispatch_event_i (TAO_Notify_Event &event TAO_ENV_ARG_DECL) = 0;
  // Derived classes should implement this.

  void on_connected (TAO_ENV_SINGLE_ARG_DECL);
  // Derived classes should call this when their consumers connect.

  void on_disconnected (TAO_ENV_SINGLE_ARG_DECL);
  // Derived classes should call this when their consumers disconnect.

  // = Data members
  TAO_Notify_ConsumerAdmin_i* consumer_admin_;
  // My parent consumer admin.

  TAO_Notify_EventType_List subscription_list_;
  // A list of event types that we are interested in.

  CORBA::Boolean is_suspended_;
  // True if we are connected to a consumer and suspended.

  typedef ACE_Unbounded_Queue<TAO_Notify_Event*> TAO_Notify_Event_List;
  TAO_Notify_Event_List event_list_;
  // A list of events populated when we're suspended.

  TAO_Notify_Worker_Task* dispatching_task_;
  // The dispatching task to send events to a listener group affiliated with this listener.

  TAO_Notify_Worker_Task* filter_eval_task_;
  // The filter evaluation task for this listener.
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
