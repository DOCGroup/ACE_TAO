#ifndef _CUTS_SYSTEM_METRICS_VISITOR_H_
#define _CUTS_SYSTEM_METRICS_VISITOR_H_

#include "cuts/config.h"
#include "cuts/CUTS_export.h"

// forward declarations
class CUTS_System_Metric;
class CUTS_Component_Metric;
class CUTS_Port_Metric;
class CUTS_Time_Metric;

//=============================================================================
/**
 * @class CUTS_System_Metrics_Visitor
 *
 * @brief Base class for all visitors that implement the visitor
 * pattern for the system metrics.
 *
 */
//=============================================================================

class CUTS_Export CUTS_System_Metrics_Visitor
{
public:
  /// Constructor.
  CUTS_System_Metrics_Visitor (void);

  /// Destructor.
  virtual ~CUTS_System_Metrics_Visitor (void) = 0;

  /**
   * Visit the system metrics.
   *
   * @param[in]     metrics     Reference to the system metrics.
   */
  virtual void visit_system_metrics (CUTS_System_Metric & metrics);

  /**
   * Visit the component metrics.
   *
   * @param[in]     metrics     Reference to the component metrics.
   */
  virtual void visit_component_metrics (CUTS_Component_Metric & metrics);

  /**
   * Visit the port metrics.
   *
   * @param[in]     metrics     Reference to the port metrics.
   */
  virtual void visit_port_metrics (CUTS_Port_Metric & metrics);

  /**
   * Visit the time metrics.
   *
   * @param[in]     metrics     Reference to the time metrics.
   */
  virtual void visit_time_metrics (CUTS_Time_Metric & metrics);
};

#if defined (__CUTS_INLINE__)
#include "cuts/System_Metrics_Visitor.inl"
#endif

#endif  // !defined _CUTS_SYSTEM_METRICS_VISITOR_H_
