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

#include "Notify_QoSAdmin_i.h"
#include "Notify_FilterAdmin_i.h"
#include "Notify_Listeners.h"
#include "orbsvcs/CosNotifyChannelAdminS.h"
#include "orbsvcs/CosNotificationS.h"
#include "ace/Containers_T.h"

class TAO_Notify_Resource_Manager;
class TAO_Notify_Event_Manager;

#if defined(_MSC_VER)
#if (_MSC_VER >= 1200)
#pragma warning(push)
#endif /* _MSC_VER >= 1200 */
#pragma warning(disable:4250)
#endif /* _MSC_VER */

template <class SERVANT_TYPE>
class TAO_ORBSVCS_Export TAO_Notify_Proxy : public SERVANT_TYPE, public TAO_Notify_Update_Listener
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
  TAO_Notify_Proxy (TAO_Notify_Resource_Manager* resource_manager);
  // Constructor

  virtual ~TAO_Notify_Proxy (void);
  // Destructor

  void init (CORBA::Environment &ACE_TRY_ENV);
  // Init

  // = Notify_Update_Listener methods
  virtual void dispatch_update (EVENTTYPE_LIST& added_list, EVENTTYPE_LIST& removed_list, CORBA::Environment &ACE_TRY_ENV);

  virtual CosNotifyChannelAdmin::ProxyType MyType (
    CORBA::Environment &ACE_TRY_ENV
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ));

virtual void validate_event_qos (
    const CosNotification::QoSProperties & required_qos,
    CosNotification::NamedPropertyRangeSeq_out available_qos,
    CORBA::Environment &ACE_TRY_ENV
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException,
    CosNotification::UnsupportedQoS
  ));

 virtual CosNotification::QoSProperties * get_qos (
    CORBA::Environment &ACE_TRY_ENV
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ));

virtual void set_qos (
    const CosNotification::QoSProperties & qos,
    CORBA::Environment &ACE_TRY_ENV
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException,
    CosNotification::UnsupportedQoS
  ));

virtual void validate_qos (
    const CosNotification::QoSProperties & required_qos,
    CosNotification::NamedPropertyRangeSeq_out available_qos,
    CORBA::Environment &ACE_TRY_ENV
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException,
    CosNotification::UnsupportedQoS
  ));

virtual CosNotifyFilter::FilterID add_filter (
    CosNotifyFilter::Filter_ptr new_filter,
    CORBA::Environment &ACE_TRY_ENV
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ));

virtual void remove_filter (
    CosNotifyFilter::FilterID filter,
    CORBA::Environment &ACE_TRY_ENV
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException,
    CosNotifyFilter::FilterNotFound
  ));

virtual CosNotifyFilter::Filter_ptr get_filter (
    CosNotifyFilter::FilterID filter,
    CORBA::Environment &ACE_TRY_ENV
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException,
    CosNotifyFilter::FilterNotFound
  ));

virtual CosNotifyFilter::FilterIDSeq * get_all_filters (
    CORBA::Environment &ACE_TRY_ENV
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ));

virtual void remove_all_filters (
    CORBA::Environment &ACE_TRY_ENV
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ));

 protected:
 virtual void dispatch_update_i (CosNotification::EventTypeSeq added, CosNotification::EventTypeSeq removed, CORBA::Environment &ACE_TRY_ENV) = 0;
 // Derived types should implement this to deliver the update.

// = Helper methods
 virtual void cleanup_i (CORBA::Environment &ACE_TRY_ENV = TAO_default_environment ());
 // Cleanup all resources used by this object.

 // = Data members
 CosNotifyChannelAdmin::ProxyType mytype_;
 // What type are we?

 TAO_Notify_Event_Manager* event_manager_;
 // Our event manager.

 TAO_Notify_Resource_Manager* resource_manager_;
 // The resource factory.

 CORBA::Boolean is_connected_;
 // True if we are connected to a consumer.

 CORBA::Boolean is_destroyed_;
 // True if we are destroyed.

 CORBA::Boolean updates_on_;
 // True by default, for subscription/publication updates.

 TAO_Notify_QoSAdmin_i qos_admin_;
 // Handle QoS admin methods.

 TAO_Notify_FilterAdmin_i filter_admin_;
 // Handles the Filter admin methods.
};

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "Notify_Proxy_T.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("Notify_Proxy_T.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma warning(pop)
#endif /* _MSC_VER */

#endif /* TAO_NOTIFY_PROXY_T_H */
