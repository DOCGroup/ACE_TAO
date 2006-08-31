// -*- C++ -*-

//=============================================================================
/**
 * @file      Component_Metric.h
 *
 * $Id$
 *
 * @author    James H. Hill
 */
//=============================================================================

#ifndef _CUTS_COMPONENT_METRIC_H_
#define _CUTS_COMPONENT_METRIC_H_

#include "cuts/config.h"
#include "cuts/CUTS_export.h"
#include "ace/Time_Value.h"
#include "ace/RW_Thread_Mutex.h"
#include <map>
#include <string>
#include <ostream>

// Forward decl.
class CUTS_Port_Metric;

// Forward decl.
class CUTS_System_Metrics_Visitor;

typedef std::map <
  long, CUTS_Port_Metric *> CUTS_Sender_Port_Map;

typedef std::map <
  std::string, CUTS_Sender_Port_Map> CUTS_Port_Metric_Map;


//=============================================================================
/**
 * @class CUTS_Component_Metric
 */
//=============================================================================

class CUTS_Export CUTS_Component_Metric
{
public:
  /// Constructor.
  CUTS_Component_Metric (void);

  /// Destructor.
  ~CUTS_Component_Metric (void);

  /// Insert a new port into the component metrics.
  CUTS_Port_Metric * insert_port (const char * port, long sender = -1);

  /// Remove a new port into the component metrics.
  void remove_port (const char * port, long sender = -1);

  /// Get the metrics of the specified port.
  CUTS_Port_Metric * port_metrics (const char * port, long sender = -1);

  void dump (std::ostream & out);

  ACE_RW_Thread_Mutex & lock (void);

  const CUTS_Port_Metric_Map & port_metrics (void) const;

  void accept (CUTS_System_Metrics_Visitor & visitor);

  const ACE_Time_Value & timestamp (void) const;

  void timestamp (const ACE_Time_Value & timestamp);

private:
  /// Mapping of name to port metrics.
  CUTS_Port_Metric_Map port_metrics_;

  /// Locking mechanism for thread sychronization.
  ACE_RW_Thread_Mutex lock_;

  ACE_Time_Value timestamp_;
};

#if defined (__CUTS_INpLINE__)
#include "cuts/Component_Metric.inl"
#endif

#endif  // !defined _CUTS_COMPONENT_METRIC_H_
