// $Id$
#ifndef CONTROL_REGISTRY_H
#define CONTROL_REGISTRY_H

#include /**/ "ace/pre.h"

#include "orbsvcs/Notify/MonitorControl/Generic_Registry.h"
#include "orbsvcs/Notify/MonitorControl/Control.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_Notify_MC_Export TAO_Control_Registry:
                                   public TAO_Generic_Registry
{
public:
  /// Return the singleton instance of the registry
  static TAO_Control_Registry* instance (void);

  /// Gets an object from the map
  /// Returns the object if it is successfully located.
  /// Returns null otherwise.
  TAO_NS_Control* get (const ACE_CString& name) const;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"
#endif /* CONTROL_REGISTRY_H */
