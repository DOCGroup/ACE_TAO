// -*- C++ -*-

//=============================================================================
/**
 * @file MonitorQuery.h
 *
 * $Id$
 *
 * @author Jeff Parsons <j.parsons@vanderbilt.edu>
 */
//=============================================================================

#ifndef MONITORQUERY_H
#define MONITORQUERY_H

#include /**/ "ace/pre.h"

#include "ace/Event_Handler.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if defined (ACE_HAS_MONITOR_FRAMEWORK) && (ACE_HAS_MONITOR_FRAMEWORK == 1)

#include "ace/MonitorControl/MonitorControl_export.h"

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

namespace ACE
{
  namespace Monitor_Control
  {
    /**
     * @class MonitorPointAutoQuery
     *
     * @brief Automates periodic querying of monitor point classes.
     *
     */
    class MONITORCONTROL_Export MonitorPointAutoQuery : public ACE_Event_Handler
    {
    public:
      /// Override of ACE base class method.
      virtual int handle_timeout (const ACE_Time_Value& current,
                                  const void* monitor_query);
    };

    /**
     * @class MonitorQuery
     *
     * @brief Handles queries for a specific monitor point, and
     *        evaluates its constraint(s) with each query.
     *
     */
    class MONITORCONTROL_Export MonitorQuery
    {
    public:
      MonitorQuery (const char* monitor_name);

      void query (void);

    private:
      Monitor_Base* monitor_;
    };
  }
}

ACE_END_VERSIONED_NAMESPACE_DECL

#endif /* ACE_HAS_MONITOR_FRAMEWORK==1 */

#include /**/ "ace/post.h"

#endif // MONITORPOINT_H
