// $Id$

#include "cuts/XML_System_Metrics_Visitor.h"

#if !defined (__CUTS_INLINE__)
#include "cuts/XML_System_Metrics_Visitor.inl"
#endif

#include "cuts/Component_Registry.h"
#include "cuts/Time.h"
#include "cuts/System_Metric.h"
#include "cuts/Component_Metric.h"
#include "cuts/Port_Metric.h"
#include "cuts/Time_Metric.h"

#include "ace/Guard_T.h"

#include <ostream>

//
// ~CUTS_XML_System_Metrics_Visitor
//
CUTS_XML_System_Metrics_Visitor::CUTS_XML_System_Metrics_Visitor (
  const CUTS_Component_Registry & registry,
  std::ostream & output)
: registry_ (registry),
  output_ (output)
{

}

//
// ~CUTS_XML_System_Metrics_Visitor
//
CUTS_XML_System_Metrics_Visitor::~CUTS_XML_System_Metrics_Visitor ()
{

}

//
// visit_system_metrics
//
void CUTS_XML_System_Metrics_Visitor::visit_system_metrics (
  CUTS_System_Metric & metrics)
{
  // Save the <timestamp> for future reference.
  this->timestamp_ = metrics.timestamp ();

  this->output_
    // Generate the opening tag.
    << "<SystemMetrics timestamp=\""
    << CUTS_string_time (this->timestamp_.sec (), "%m-%d-%Y %H:%M:%S")
    << "\">" << std::endl;

  ACE_READ_GUARD (ACE_RW_Thread_Mutex, guard, metrics.lock ());

  CUTS_Component_Metric_Map::const_iterator iter;
  for (iter  = metrics.component_metrics ().begin ();
       iter != metrics.component_metrics ().end ();
       iter ++)
  {
    // There is no need to continue if there isn't any
    // up-to-date information.
    if (iter->second->timestamp () != this->timestamp_)
    {
      continue;
    }

    // Generate the open tag for the <ComponentMetrics>
    this->output_
      << "  " << "<ComponentMetrics instance=\""
      << this->registry_.get_component_by_id (iter->first)
      << "\">" << std::endl;

    // Visit the component's metric.
    iter->second->accept (*this);

    // Generate the closing tag for the <ComponentMetrics>.
    this->output_
      << "  " << "</ComponentMetrics>" << std::endl;
  }

  // Generate the closing tag.
  this->output_
    << "</SystemMetrics>" << std::endl;
}

//
// visit_component_metrics
//
void CUTS_XML_System_Metrics_Visitor::visit_component_metrics (
  CUTS_Component_Metric & metrics)
{
  ACE_READ_GUARD (ACE_RW_Thread_Mutex, guard, metrics.lock ());

  CUTS_Port_Metric_Map::const_iterator iter;

  for (iter  = metrics.port_metrics ().begin ();
       iter != metrics.port_metrics ().end ();
       iter ++)
  {
    CUTS_Sender_Port_Map::const_iterator s_iter;

    // Generate the opening tag.
    this->output_
      << "    <Port name=\"" << iter->first << "\">" << std::endl;

    for (s_iter  = iter->second.begin ();
         s_iter != iter->second.end ();
         s_iter ++)
    {
      // There is no need to continue if there isn't any
      // up-to-date information.
      if (s_iter->second->timestamp () != this->timestamp_)
      {
        continue;
      }

      // Generate the opening tag for the port metrics.
      this->output_
        << "      <PortMetric sender=\""
        << this->registry_.get_component_by_id (s_iter->first)
        << "\">" << std::endl;

      // Visit the port metrics for this sender.
      s_iter->second->accept (*this);

      // Generate the closing tag.
      this->output_
        << "      </PortMetric>" << std::endl;
    }

    // Generate the closing tag.
    this->output_
      << "    </Port>" << std::endl;
  }
}

//
// visit_port_metrics
//
void CUTS_XML_System_Metrics_Visitor::visit_port_metrics (
  CUTS_Port_Metric & metrics)
{
  ACE_READ_GUARD (ACE_RW_Thread_Mutex, guard, metrics.lock ());

  // Generate the opening tag for the <TransitTime>.
  this->output_
    << "        <TransitTime count=\""
    << metrics.transit_time ().count () << "\">" << std::endl;

  // Visit the time metrics for the transit time.
  metrics.transit_time ().accept (*this);

  // Generate the closing tag for the <TransitTime>.
  this->output_
    << "        </TransitTime>" << std::endl;

  CUTS_Endpoint_Metric_Map::const_iterator e_iter;

  for (e_iter  = metrics.endpoints ().begin ();
       e_iter != metrics.endpoints ().end ();
       e_iter ++)
  {
    // There is no need to continue if there isn't any
    // up-to-date information.
    if (e_iter->second->timestamp () != this->timestamp_)
    {
      continue;
    }

    // Generate the opening tag.
    this->output_
      << "        <ExitPoint name=\"" << e_iter->first <<  "\" count=\""
      << e_iter->second->count ()
      << "\">" << std::endl;

    // Visit the time metrics for this exit point.
    e_iter->second->accept (*this);

    // Generate the closing tag.
    this->output_
      << "        </ExitPoint>" << std::endl;
  }
}

//
// visit_time_metrics
//
void CUTS_XML_System_Metrics_Visitor::visit_time_metrics (
  CUTS_Time_Metric & metrics)
{
  this->output_
    << "          <Best>" << metrics.best_time ()
    << "</Best>" << std::endl
    << "          <Average>" << metrics.average_time ()
    << "</Average>" << std::endl
    << "          <Worse>" << metrics.worse_time ()
    << "</Worse>" << std::endl;
}
