/* -*- C++ -*- */
/**
 *  @file EventChannelFactory.h
 *
 *  $Id$
 *
 *  @author Pradeep Gore <pradeep@oomworks.com>
 *
 *
 */

#ifndef TAO_NS_EVENTCHANNELFACTORY_H
#define TAO_NS_EVENTCHANNELFACTORY_H
#include "ace/pre.h"

#include "notify_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/CosNotifyChannelAdminS.h"
#include "Container_T.h"
#include "Destroy_Callback.h"

class TAO_NS_Notify_Service;
class TAO_NS_EventChannel;

#if defined(_MSC_VER)
#if (_MSC_VER >= 1200)
#pragma warning(push)
#endif /* _MSC_VER >= 1200 */
#pragma warning(disable:4250)
#endif /* _MSC_VER */

/**
 * @class TAO_NS_EventChannelFactory
 *
 * @brief Implementation of CosNotifyChannelAdmin::EventChannelFactory
 *
 */


class TAO_Notify_Export TAO_NS_EventChannelFactory : public virtual POA_CosNotifyChannelAdmin::EventChannelFactory, public TAO_NS_Container_T <TAO_NS_EventChannel, TAO_NS_EventChannelFactory, TAO_NS_Notify_Service>, public TAO_NS_Destroy_Callback
{
  friend class TAO_NS_Builder;

public:
  typedef TAO_NS_Container_T <TAO_NS_EventChannel, TAO_NS_EventChannelFactory, TAO_NS_Notify_Service> inherited;
  typedef ACE_Unbounded_Set <TAO_NS_Object_Id> TAO_NS_Object_Id_Seq;

  /// Constuctor
  TAO_NS_EventChannelFactory (void);

  /// Init the factory
  void init (ACE_ENV_SINGLE_ARG_DECL);

  /// Destructor
  virtual ~TAO_NS_EventChannelFactory ();

  /// Return servant
  virtual PortableServer::Servant servant (void);

  /// Insert object to this container.
  virtual void insert (TAO_NS_EventChannel* event_channel ACE_ENV_ARG_DECL);

  /// Remove type from container_
  virtual void remove (TAO_NS_EventChannel* event_channel ACE_ENV_ARG_DECL);

  /// = ServantBase  Methods
  virtual void _add_ref (ACE_ENV_SINGLE_ARG_DECL);
  virtual void _remove_ref (ACE_ENV_SINGLE_ARG_DECL);

  /// TAO_NS_Destroy_Callback methods
  virtual void release (void);

  /// Accesor for the default filter factory shared by all EC's.
  virtual CosNotifyFilter::FilterFactory_ptr get_default_filter_factory (ACE_ENV_SINGLE_ARG_DECL);

 protected:
  /// = Data Members

  /// List of Event Channel ID's
  TAO_NS_Object_Id_Seq event_channel_id_list_;

  /// The default filter factory.
  CosNotifyFilter::FilterFactory_var default_filter_factory_;

  /// = CosNotifyChannelAdmin Methods

  virtual ::CosNotifyChannelAdmin::EventChannel_ptr create_channel (const CosNotification::QoSProperties & initial_qos,
                                                                    const CosNotification::AdminProperties & initial_admin,
                                                                    CosNotifyChannelAdmin::ChannelID_out id ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((
                     CORBA::SystemException
                     , CosNotification::UnsupportedQoS
                     , CosNotification::UnsupportedAdmin
                     ));

  virtual ::CosNotifyChannelAdmin::ChannelIDSeq * get_all_channels (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((
                     CORBA::SystemException
                     ));

  virtual ::CosNotifyChannelAdmin::EventChannel_ptr get_event_channel (CosNotifyChannelAdmin::ChannelID id
                                                                       ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((
                     CORBA::SystemException
                     , CosNotifyChannelAdmin::ChannelNotFound
                     ));
};

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma warning(pop)
#endif /* _MSC_VER */

#if defined (__ACE_INLINE__)
#include "EventChannelFactory.inl"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* TAO_NS_EVENTCHANNELFACTORY_H */
