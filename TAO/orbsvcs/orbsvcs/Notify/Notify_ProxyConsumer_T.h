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
#include "ace/pre.h"
#define TAO_NOTIFY_PROXYCONSUMER_T_H

#include "Notify_Proxy_T.h"
class TAO_Notify_SupplierAdmin_i;

#if defined(_MSC_VER)
#if (_MSC_VER >= 1200)
#pragma warning(push)
#endif /* _MSC_VER >= 1200 */
#pragma warning(disable:4250)
#endif /* _MSC_VER */

template <class SERVANT_TYPE>
class TAO_Notify_Export TAO_Notify_ProxyConsumer : public TAO_Notify_Proxy<SERVANT_TYPE>
{
  // = TITLE
  //   TAO_Notify_ProxyConsumer
  //
  // = DESCRIPTION
  //   The is a base class for all proxy consumers.
  //

public:
  TAO_Notify_ProxyConsumer (TAO_Notify_SupplierAdmin_i* supplieradmin,
                            TAO_Notify_Resource_Manager* resource_manager);
  // Constructor

  virtual ~TAO_Notify_ProxyConsumer (void);
  // Destructor

  void init (CORBA::Environment &ACE_TRY_ENV);
  // Init

  virtual CosNotifyChannelAdmin::SupplierAdmin_ptr MyAdmin (
    CORBA::Environment &ACE_TRY_ENV
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ));

  virtual CosNotification::EventTypeSeq * obtain_subscription_types (
    CosNotifyChannelAdmin::ObtainInfoMode mode,
    CORBA::Environment &ACE_TRY_ENV
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ));

virtual void offer_change (
    const CosNotification::EventTypeSeq & added,
    const CosNotification::EventTypeSeq & removed,
    CORBA::Environment &ACE_TRY_ENV
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException,
    CosNotifyComm::InvalidEventType
  ));

 protected:
// = Helper methods
 virtual void cleanup_i (CORBA::Environment &ACE_TRY_ENV = TAO_default_environment ());
 // Cleanup all resources used by this object.

 CORBA::Boolean check_filters_i (const TAO_Notify_Event& event, CORBA::Environment& ACE_TRY_ENV);
 // Check filters.

 void on_connected (CORBA::Environment &ACE_TRY_ENV);
 // Derived classes should call this when their suppliers connect.

 // = Data members
 TAO_Notify_SupplierAdmin_i* myadmin_;
 // My parent supplier admin.
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
