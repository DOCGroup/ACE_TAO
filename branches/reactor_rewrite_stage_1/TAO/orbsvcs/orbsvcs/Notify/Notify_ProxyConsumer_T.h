// $Id$
// ==========================================================================
//
// = LIBRARY
//   orbsvcs
//
// = FILENAME
//   Notify_ProxyConsumer_T.h
//
// = DESCRIPTION
//   Template Base class for all Proxy Consumers.
//
// = AUTHOR
//   Pradeep Gore <pradeep@cs.wustl.edu>
//
// ==========================================================================

#ifndef TAO_NOTIFY_PROXYCONSUMER_T_H
#define TAO_NOTIFY_PROXYCONSUMER_T_H
#include "ace/pre.h"
#include "Notify_Proxy_T.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "Notify_Listeners.h"

class TAO_Notify_SupplierAdmin_i;

#if defined(_MSC_VER)
#if (_MSC_VER >= 1200)
#pragma warning(push)
#endif /* _MSC_VER >= 1200 */
#pragma warning(disable:4250)
#endif /* _MSC_VER */

template <class SERVANT_TYPE>
class TAO_Notify_Export TAO_Notify_ProxyConsumer : public TAO_Notify_Proxy<SERVANT_TYPE>, virtual public TAO_Notify_EventSource
{
  // = TITLE
  //   TAO_Notify_ProxyConsumer
  //
  // = DESCRIPTION
  //   The is a base class for all proxy consumers.
  //

public:
  TAO_Notify_ProxyConsumer (TAO_Notify_SupplierAdmin_i* supplier_admin);
  // Constructor

  virtual ~TAO_Notify_ProxyConsumer (void);
  // Destructor

  void init (CosNotifyChannelAdmin::ProxyID myID TAO_ENV_ARG_DECL);
  // Init the Proxy.

  // = TAO_Notify_EventSource methods.
  virtual CORBA::Boolean evaluate_filter (TAO_Notify_Event &event TAO_ENV_ARG_DECL);
  // Evaluates true if this event is acceptable by the Source.

  TAO_Notify_Worker_Task* filter_eval_task (void);
  // The Worker task associated with the event listener for filter evaluation.

  virtual CosNotifyChannelAdmin::SupplierAdmin_ptr MyAdmin (TAO_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((
                     CORBA::SystemException
                     ));

  virtual CosNotification::EventTypeSeq * obtain_subscription_types (
    CosNotifyChannelAdmin::ObtainInfoMode mode
    TAO_ENV_ARG_DECL
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ));

virtual void offer_change (
    const CosNotification::EventTypeSeq & added,
    const CosNotification::EventTypeSeq & removed
    TAO_ENV_ARG_DECL
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException,
    CosNotifyComm::InvalidEventType
  ));

protected:
  // = Helper methods
  void on_connected (TAO_ENV_SINGLE_ARG_DECL);
  // Derived classes should call this when their suppliers connect.

  void on_disconnected (TAO_ENV_SINGLE_ARG_DECL);
  // Derived classes should call this when their suppliers disconnect.

  // = Data members
  TAO_Notify_SupplierAdmin_i* supplier_admin_;
  // My parent supplier admin.

  TAO_Notify_Worker_Task* filter_eval_task_;
  // The filter evaluation task for this listener.
};

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "Notify_ProxyConsumer_T.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("Notify_ProxyConsumer_T.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma warning(pop)
#endif /* _MSC_VER */

#include "ace/post.h"
#endif /* TAO_NOTIFY_PROXYPUSHCONSUMER_T_H */
