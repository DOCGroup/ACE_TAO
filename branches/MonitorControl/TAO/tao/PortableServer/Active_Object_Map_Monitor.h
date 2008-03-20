// -*- C++ -*-

//=============================================================================
/**
 *  @file    Active_Object_Map_Monitor.h
 *
 *  $Id$
 *
 *  @author Jeff Parsons
 */
//=============================================================================


#ifndef TAO_ACTIVE_OBJECT_MAP_MONITOR_H
#define TAO_ACTIVE_OBJECT_MAP_MONITOR_H

#include /**/ "ace/pre.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Monitor_Base.h"

#include "tao/Versioned_Namespace.h"

#include "tao/PortableServer/portableserver_export.h"

using namespace ACE_VERSIONED_NAMESPACE_NAME::ACE::MonitorControl;

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

/**
 * @class Active_Object_Map_Monitor
 *
 * @brief Monitors the size of the POA's Active Object Map.
 *
 */
class TAO_PortableServer_Export Active_Object_Map_Monitor
  : public Monitor_Base
{
public:
      Active_Object_Map_Monitor (void);
      Active_Object_Map_Monitor (const char* name);
      virtual ~Active_Object_Map_Monitor (void);

      /// Implemented by the most-derived class. Does the actual
      /// work of fetching the monitored value.
      virtual void update (void);

      /// Updates the monitor's data if it is a numeric floating point.
      virtual void receive (double value);

      /// Updates the monitor's data if it is an integer size.
      virtual void receive (size_t value);

      /// Reset function.
      virtual void clear (void);
};

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#endif /* TAO_ACTIVE_OBJECT_MAP_MONITOR_H */
