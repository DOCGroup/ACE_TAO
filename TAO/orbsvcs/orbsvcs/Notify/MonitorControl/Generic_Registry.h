// $Id$
#ifndef GENERIC_REGISTRY_H
#define GENERIC_REGISTRY_H

#include /**/ "ace/pre.h"

#include "ace/SString.h"
#include "ace/Null_Mutex.h"
#include "ace/Hash_Map_Manager_T.h"
#include "tao/TAO_Singleton.h"
#include "orbsvcs/Notify/MonitorControl/Generic.h"
#include "orbsvcs/Notify/MonitorControl/NotificationServiceMCC.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_Notify_MC_Export TAO_Generic_Registry
{
public:
  /// In order to minimize the number of allocations, we're reusing the
  /// NameList that comes from the idl generated interface
  /// CosNotification::NotificationServiceMonitor
  typedef CosNotification::NotificationServiceMonitorControl::NameList NameList;

  /// Exception thrown in the event that something goes wrong
  /// when adding to the registry
  class TAO_Notify_MC_Export Map_Error
  {
  public:
    enum ErrorReason { MAP_ERROR_BIND_FAILURE, MAP_ERROR_INVALID_VALUE };

    ErrorReason why_;

    Map_Error (ErrorReason why)
     : why_ (why) {
    }
  };

  /// Empty out the map of objects
  ~TAO_Generic_Registry (void);

  /// Adds an object to the map.
  /// Throws Map_Error if the object is null or if there
  /// is a fatal error adding it to the map.
  /// Returns true if the object is successfully added to the map.
  /// Returns false otherwise.
  bool add (TAO_NS_Generic* type);

  /// Removes an object from the map.
  /// Returns true if the object is successfully removed from the map.
  /// Returns false otherwise.
  bool remove (const ACE_CString& name);

  /// Returns a list of names stored in the registry
  const NameList& names (void);

protected:
  /// Gets an object from the map
  /// Returns the object if it is successfully located.
  /// Returns null otherwise.
  TAO_NS_Generic* getobj (const ACE_CString& name) const;

private:
  typedef ACE_Hash_Map_Manager<ACE_CString,
                               TAO_NS_Generic*,
                               ACE_SYNCH_NULL_MUTEX> Map;

  mutable TAO_SYNCH_RW_MUTEX mutex_;
  Map map_;
  NameList name_cache_;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"
#endif /* GENERIC_REGISTRY_H */
