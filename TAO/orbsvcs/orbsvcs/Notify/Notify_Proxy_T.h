// $Id$
// =========================================================================
//
// = LIBRARY
//   orbsvcs
//
// = FILENAME
//   Notify_Proxy.h
//
// = DESCRIPTION
//   Template Base class for all Proxys.
//
// = AUTHOR
//    Pradeep Gore <pradeep@cs.wustl.edu>
//
// ==========================================================================
#ifndef TAO_NOTIFY_PROXY_T_H
#define TAO_NOTIFY_PROXY_T_H
#include "ace/pre.h"
#include "ace/Containers_T.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/CosNotifyChannelAdminS.h"
#include "orbsvcs/CosNotificationS.h"
#include "Notify_QoSAdmin_i.h"
#include "Notify_FilterAdmin_i.h"
#include "Notify_Listeners.h"
#include "notify_export.h"

class TAO_Notify_Event_Manager;

#if defined (_MSC_VER)
#if (_MSC_VER >= 1200)
#pragma warning (push)
#endif /* _MSC_VER >= 1200 */
#pragma warning (disable:4250)
#endif /* _MSC_VER */

// @@ Pradeep: this is cool, deriving from the template type is really
// neat.  I bet it is going to break something like 90% of the
// compilers, but they deserve it! ;-) ;-)

template <class SERVANT_TYPE>
class TAO_Notify_Export TAO_Notify_Proxy : public SERVANT_TYPE, virtual public TAO_Notify_UpdateListener, public PortableServer::RefCountServantBase
{
  // = TITLE
  //   TAO_Notify_Proxy
  //
  // = DESCRIPTION
  //   The is a base class for all proxys , templatized by the servant
  //   type. All the Filter Admin and QoS Admin interface methods are
  //   implemented here by delegating to the admin implementations.
  //

public:
  TAO_Notify_Proxy (void);
  // Constructor

  virtual ~TAO_Notify_Proxy (void);
  // Destructor

  // = TAO_Notify_RefCounted methods
  CORBA::ULong _incr_refcnt (void);
  CORBA::ULong _decr_refcnt (void);
  // Increment and decrement the reference count.

  // = The Servant methods
  virtual void _add_ref (ACE_ENV_SINGLE_ARG_DECL);
  virtual void _remove_ref (ACE_ENV_SINGLE_ARG_DECL);

  // = Notify_Update_Listener methods
  virtual void dispatch_update (TAO_Notify_EventType_List& added_list, TAO_Notify_EventType_List& removed_list ACE_ENV_ARG_DECL);

  virtual CosNotifyChannelAdmin::ProxyType MyType (
    ACE_ENV_SINGLE_ARG_DECL
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ));

  virtual void validate_event_qos (
    const CosNotification::QoSProperties & required_qos,
    CosNotification::NamedPropertyRangeSeq_out available_qos
    ACE_ENV_ARG_DECL
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException,
    CosNotification::UnsupportedQoS
  ));

 virtual CosNotification::QoSProperties * get_qos (
    ACE_ENV_SINGLE_ARG_DECL
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ));

  virtual void set_qos (
    const CosNotification::QoSProperties & qos
    ACE_ENV_ARG_DECL
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException,
    CosNotification::UnsupportedQoS
  ));

  virtual void validate_qos (
    const CosNotification::QoSProperties & required_qos,
    CosNotification::NamedPropertyRangeSeq_out available_qos
    ACE_ENV_ARG_DECL
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException,
    CosNotification::UnsupportedQoS
  ));

  virtual CosNotifyFilter::FilterID add_filter (
    CosNotifyFilter::Filter_ptr new_filter
    ACE_ENV_ARG_DECL
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ));

  virtual void remove_filter (
    CosNotifyFilter::FilterID filter
    ACE_ENV_ARG_DECL
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException,
    CosNotifyFilter::FilterNotFound
  ));

  virtual CosNotifyFilter::Filter_ptr get_filter (
    CosNotifyFilter::FilterID filter
    ACE_ENV_ARG_DECL
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException,
    CosNotifyFilter::FilterNotFound
  ));

  virtual CosNotifyFilter::FilterIDSeq * get_all_filters (
    ACE_ENV_SINGLE_ARG_DECL
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ));

  virtual void remove_all_filters (
    ACE_ENV_SINGLE_ARG_DECL
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ));

  const TAO_Notify_QoSAdmin_i& qos_admin (void) const;

protected:
  virtual void dispatch_update_i (CosNotification::EventTypeSeq added, CosNotification::EventTypeSeq removed ACE_ENV_ARG_DECL) = 0;
  // Derived types should implement this to deliver the update.

  virtual void setup_qos_policies (ACE_ENV_SINGLE_ARG_DECL);
  // Derived types may implement this if they need to do something
  // directly after setting the qos policies.

  // = Data members
  ACE_Lock* lock_;
  // The locking strategy.

  CORBA::ULong refcount_;
  // The reference count.

  CosNotifyChannelAdmin::ProxyType proxy_type_;
  // What type are we?

  TAO_Notify_Event_Manager* event_manager_;
  // Our event manager.

  CORBA::Boolean is_connected_;
  // True if we are connected to a consumer.

  CORBA::Boolean updates_on_;
  // True by default, for subscription/publication updates.

  TAO_Notify_QoSAdmin_i qos_admin_;
  // Handle QoS admin methods.

  TAO_Notify_FilterAdmin_i filter_admin_;
  // Handles the Filter admin methods.

  CosNotifyChannelAdmin::ProxyID proxy_id_;
  // The ID assigned to this Proxy.
};

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "Notify_Proxy_T.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("Notify_Proxy_T.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#if defined (_MSC_VER) && (_MSC_VER >= 1200)
#pragma warning (pop)
#endif /* _MSC_VER */

#include "ace/post.h"
#endif /* TAO_NOTIFY_PROXY_T_H */
