// -*- C++ -*-

//=============================================================================
/**
 * @file      XML_System_Metrics_Visitor.h
 *
 * @brief     Defines the XML format generator for system metrics.
 *
 * $Id$
 *
 * @author    James H. Hill
 *
 * @note      The location of this file is temporary. Eventually it will
 *            be moved to cuts/utils once the circular dependencies are
 *            corrected.
 */
//=============================================================================

#ifndef _CUTS_XML_SYSTEM_METRICS_VISITOR_H_
#define _CUTS_XML_SYSTEM_METRICS_VISITOR_H_

#include "cuts/config.h"
#include "cuts/CUTS_export.h"
#include "cuts/System_Metrics_Visitor.h"
#include "ace/Time_Value.h"

#include <iosfwd>

// forward declarations
class CUTS_Component_Registry;

//=============================================================================
/**
 * @class CUTS_XML_System_Metrics_Visitor
 *
 * @brief XML format generator for the system metrics.
 */
//=============================================================================

class CUTS_Export CUTS_XML_System_Metrics_Visitor :
  public CUTS_System_Metrics_Visitor
{
public:
  /**
   * Initializing constructor.
   *
   * @param[in]      registry       Reference to the component registry
   *                                that corresponds to the metrics.
   * @param[in]      output         Output stream for the XML representation.
   */
  CUTS_XML_System_Metrics_Visitor (const CUTS_Component_Registry & registry,
                                   std::ostream & output);

  /// Destructor.
  virtual ~CUTS_XML_System_Metrics_Visitor (void);

  /**
   * Visit the component metrics. This method generates the
   * <ComponentMetrics> section of the XML file.
   *
   * @param[in]     metrics     Reference to the metrics.
   */
  virtual void visit_system_metrics (CUTS_System_Metric & metrics);

  /**
   * Visit the system metrics. This method generates the
   * <SystemMetrics> section of the XML file. It will also
   * insert the timestamp when the generation started.
   *
   * @param[in]     metrics     Reference to the metrics.
   */
  virtual void visit_component_metrics (CUTS_Component_Metric & metrics);

  /**
   * Visit the port metrics. This method generates the
   * <ComponentMetrics> section of the XML file.
   *
   * @param[in]     metrics     Reference to the metrics.
   */
  virtual void visit_port_metrics (CUTS_Port_Metric & metrics);

  /**
   * Visit the time metrics. This method generates the
   * <ComponentMetrics> section of the XML file.
   *
   * @param[in]     metrics     Reference to the metrics.
   */
  virtual void visit_time_metrics (CUTS_Time_Metric & metrics);

private:
  /// Registry of the components.
  const CUTS_Component_Registry & registry_;

  /// Output stream for the XML representation.
  std::ostream & output_;

  /// Current timestamp for XML generation.
  ACE_Time_Value timestamp_;
};

#if defined (__CUTS_INLINE__)
#include "cuts/XML_System_Metrics_Visitor.inl"
#endif

#endif  // !defined _CUTS_XML_SYSTEM_METRICS_VISITOR_H_
