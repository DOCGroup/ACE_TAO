// -*- C++ -*-

//=============================================================================
/**
 * @file Size_Monitor.h
 *
 * $Id$
 *
 * @author Jeff Parsons <j.parsons@vanderbilt.edu>
 */
//=============================================================================

#ifndef SIZE_MONITOR_H
#define SIZE_MONITOR_H

#include /**/ "ace/pre.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Monitor_Base.h"

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

namespace ACE
{
  namespace MonitorControl
  {
    /**
     * @class Size_Monitor
     *
     * @brief Base class from which ACE monitors of size are
     *        derived.
     *
     */
    class ACE_Export Size_Monitor : public Monitor_Base
    {
    public:
      Size_Monitor (void);
      Size_Monitor (const char* name);
      virtual ~Size_Monitor (void);

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
  }
}

ACE_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#endif // SIZE_MONITOR_H
