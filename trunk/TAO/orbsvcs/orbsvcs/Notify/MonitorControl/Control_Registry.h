// $Id$

#ifndef CONTROL_REGISTRY_H
#define CONTROL_REGISTRY_H

#include /**/ "ace/pre.h"

#include "tao/TAO_Singleton.h"

#include "tao/Monitor/Monitor.h"

#include "orbsvcs/orbsvcs/Notify/MonitorControl/Control.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_Notify_MC_Export TAO_Control_Registry
{
public:
  /// Reusing NameList that comes from the TAO Monitor library.
  typedef Monitor::NameList NameList;

  /// Exception thrown in the event that something goes wrong
  /// when adding to the registry
  class TAO_Notify_MC_Export Map_Error
  {
  public:
    enum ErrorReason
    {
      MAP_ERROR_BIND_FAILURE,
      MAP_ERROR_INVALID_VALUE
    };

    ErrorReason why_;

    Map_Error (ErrorReason why)
      : why_ (why)
    {
    }
  };

  /// Return the singleton instance of the registry
  static TAO_Control_Registry* instance (void);

  /// Empty out the map of objects
  ~TAO_Control_Registry (void);

  /// Adds an object to the map.
  /// Throws Map_Error if the object is null or if there
  /// is a fatal error adding it to the map.
  /// Returns true if the object is successfully added to the map.
  /// Returns false otherwise.
  bool add (TAO_NS_Control* type);

  /// Removes an object from the map.
  /// Returns true if the object is successfully removed from the map.
  /// Returns false otherwise.
  bool remove (const ACE_CString& name);

  /// Returns a list of names stored in the registry
  const NameList& names (void);

  /// Gets an object from the map
  /// Returns the object if it is successfully located.
  /// Returns null otherwise.
  TAO_NS_Control* get (const ACE_CString& name) const;

private:
  typedef ACE_Hash_Map_Manager<ACE_CString,
                               TAO_NS_Control*,
                               ACE_SYNCH_NULL_MUTEX>
    Map;

  mutable TAO_SYNCH_RW_MUTEX mutex_;
  Map map_;
  NameList name_cache_;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#endif /* CONTROL_REGISTRY_H */
