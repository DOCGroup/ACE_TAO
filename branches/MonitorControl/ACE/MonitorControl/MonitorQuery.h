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

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Event_Handler.h"

#include "MonitorControl/MonitorControl_export.h"

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

namespace ACE
{
  namespace MonitorControl
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
     *        evaluates its constraint(s).
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

#include /**/ "ace/post.h"

#endif // MONITORPOINT_H
