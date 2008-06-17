// -*- C++ -*-

//=============================================================================
/**
 * @file BSD_Network_Interface_Monitor.h
 *
 * $Id$
 *
 * @author Jeff Parsons <j.parsons@vanderbilt.edu>
 */
//=============================================================================

#ifndef BSD_NETWORK_INTERFACE_MONITOR_H
#define BSD_NETWORK_INTERFACE_MONITOR_H

#include /**/ "ace/pre.h"

#include "ace/SString.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Monitor_Control/Monitor_Control_export.h"

#if defined (__FreeBSD__) || defined (__NetBSD__) || defined (__OpenBSD__)

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

namespace ACE
{
  namespace Monitor_Control
  {
    /**
     * @class BSD_Network_Interface_Monitor
     *
     * @brief Mixin class for network interface monitors compiled on
     *        *BSD machines.
     */
    class MONITOR_CONTROL_Export BSD_Network_Interface_Monitor
    {
    protected:
      BSD_Network_Interface_Monitor (const ACE_TCHAR *lookup_str);

      /// Platform-specific implementation.
      void update_i (void);

    protected:
      ACE_UINT64 value_;
      
    private:
      ACE_CString lookup_str_;
    };
  }
}

ACE_END_VERSIONED_NAMESPACE_DECL

#endif /* defined (__FreeBSD__) || defined (__NetBSD__) || defined (__OpenBSD__) */

#include /**/ "ace/post.h"

#endif // BSD_NETWORK_INTERFACE_MONITOR_H
