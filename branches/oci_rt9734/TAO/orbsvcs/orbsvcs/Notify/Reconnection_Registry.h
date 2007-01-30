// -*- C++ -*-

/**
 *  @file Reconnection_Registry.h
 *
 *  $Id$
 *
 *  @author Dale Wilson <wilson_d@ociweb.com>
 */

#ifndef TAO_NOTIFY_RECONNECTION_REGISTRY_H
#define TAO_NOTIFY_RECONNECTION_REGISTRY_H
#include /**/ <ace/pre.h>
#include /**/ <ace/config-all.h>

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/Notify/Topology_Object.h"

#include "orbsvcs/NotifyExtS.h"

#include "ace/Hash_Map_Manager_T.h"
#include "ace/Null_Mutex.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO_Notify
{
  extern const char REGISTRY_TYPE[];
  extern const char RECONNECT_ID[];
  extern const char RECONNECT_IOR[];
  extern const char REGISTRY_CALLBACK_TYPE[];

  /**
   * @class Reconnection_Registry
   *
   * @brief Implementation of ReconnectionRegistry
   *
   */
  class TAO_Notify_Serv_Export Reconnection_Registry
    : public Topology_Object
  {
    /// The registry consists of a map from ReconnectionID to stringified IOR
    typedef ACE_Hash_Map_Manager_Ex<NotifyExt::ReconnectionRegistry::ReconnectionID,
      ACE_CString,
      ACE_Hash<NotifyExt::ReconnectionRegistry::ReconnectionID>,
      ACE_Equal_To<NotifyExt::ReconnectionRegistry::ReconnectionID>,
      ACE_SYNCH_NULL_MUTEX> Reconnection_Registry_Type;

  public:

    /// Constructor
    Reconnection_Registry (Topology_Parent & parent);

    /// Destructor
    virtual ~Reconnection_Registry ();

    //////////////////////////
    // During normal operation

    /// add a new callback to the registry
    ::NotifyExt::ReconnectionRegistry::ReconnectionID register_callback (
      ::NotifyExt::ReconnectionCallback_ptr callback);

    ///
    void unregister_callback (::NotifyExt::ReconnectionRegistry::ReconnectionID id);

    CORBA::Boolean is_alive (void);

    //////////////////////
    // During topology save
    virtual void save_persistent (Topology_Saver& saver);

    ///////////////////////////////////////
    // During reload of persistent topology

    virtual Topology_Object* load_child (const ACE_CString & type,
      CORBA::Long id,
      const NVPList& attrs);

    void send_reconnect (CosNotifyChannelAdmin::EventChannelFactory_ptr dest_factory);

  private:
    void release (void);

    Reconnection_Registry_Type reconnection_registry_;
    ::NotifyExt::ReconnectionRegistry::ReconnectionID highest_id_;
  };
} // namespace TAO_Notify

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ <ace/post.h>
#endif /* TAO_NOTIFY_RECONNECTION_REGISTRY_H */
