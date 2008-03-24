// -*- C++ -*-

//=============================================================================
/**
 * @file CDR_Buffer_Monitor.h
 *
 * $Id$
 *
 * @author Jeff Parsons <j.parsons@vanderbilt.edu>
 */
//=============================================================================

#ifndef CDR_BUFFER_MONITOR_H
#define CDR_BUFFER_MONITOR_H

#include /**/ "ace/pre.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Size_Monitor.h"

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

namespace ACE
{
  namespace MonitorControl
  {
    /**
     * @class CDR_Buffer_Monitor
     *
     * @brief Monitors the size of an input or output CDR stream.
     *
     */
    class ACE_Export CDR_Buffer_Monitor : public Size_Monitor
    {
    public:
      CDR_Buffer_Monitor (void);
      CDR_Buffer_Monitor (const char* name);
      virtual ~CDR_Buffer_Monitor (void);
    };
  }
}

ACE_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#endif // CDR_BUFFER_MONITOR_H
