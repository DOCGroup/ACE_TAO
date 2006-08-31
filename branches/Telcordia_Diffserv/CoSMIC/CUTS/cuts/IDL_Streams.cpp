// $Id$

#include "cuts/IDL_Streams.h"
#include "cuts/Time_Metric.h"
#include "cuts/Port_Metric.h"
#include "cuts/System_Metric.h"
#include "cuts/Component_Metric.h"
#include "cuts/PortAgent.h"

namespace CUTS
{
  void operator >> (const Benchmark_Data_var & data,
                    CUTS_System_Metric & sys_metrics)
  {
    // Get the <CUTS_Component_Metric> for this <data>. If one does not
    // exist it will create a new one, which should be a one time only
    // occurrence. We also update the timestamp for the metrics.
    CUTS_Component_Metric * metric =
      sys_metrics.component_metrics (data->owner);
    metric->timestamp (sys_metrics.timestamp ());

    // Get the <ports> and the <length> of the <port> sequence.
    Port_Measurement_Seq ports = data->ports;
    long length = ports.length ();

    for (long i = 0; i < length; i ++)
    {
      // Get the next <port_measurement> in the collection.
      Port_Measurement & port_measurement = ports[i];

      long map_length = port_measurement.measurements.length ();

      for (long j = 0; j < map_length; j ++)
      {
        Mapped_Port_Measurement & mapped_measurement =
          port_measurement.measurements[j];

        CUTS_Port_Metric * port_metric =
          metric->port_metrics (port_measurement.port,
                                mapped_measurement.owner);

        if (port_metric)
        {
          // Set the timestampl for the port metrics and insert the
          // data into the metric.
          port_metric->timestamp (metric->timestamp ());
          mapped_measurement >> (*port_metric);
        }
      }
    }
  }

  void operator >> (const Mapped_Port_Measurement & port_measurement,
                    CUTS_Port_Metric & port_metric)
  {
    do
    {
      // Save the transit time.
      ACE_WRITE_GUARD (ACE_RW_Thread_Mutex, guard, port_metric.lock ());
      port_measurement.transit_time >> port_metric.transit_time ();
    } while (0);

    // Get all the exit times for this component.
    long length = port_measurement.exit_times.length ();

    for (long i = 0; i < length; i ++)
    {
      // Get the next <exit_point> from the collection.
      const Exit_Point_Time & exit_point = port_measurement.exit_times[i];

      // Locate the <endpoint> in the <port_metric>.
      CUTS_Time_Metric * time_metric =
        port_metric.endpoint (exit_point.exit_point_);

      if (time_metric)
      {
        // Update the <timestamp> on the <time_metric> and store
        // the <time_sample_> in the <time_metric>.
        time_metric->timestamp (port_metric.timestamp ());
        exit_point.time_sample_ >> (*time_metric);
      }
    }
  }

  void operator >> (const Time_Sample & time_sample,
                    CUTS_Time_Metric & time_metric)
  {
    // Calculate the average timing metric.
    long average = 0;

    if (time_sample.count > 0)
      average = time_sample.time.total / time_sample.count;

    // Update the timing information.
    time_metric.update (time_sample.count,
                        average,
                        time_sample.time.min,
                        time_sample.time.max);
  }

  void operator << (Port_Measurement_Seq & pms,
                    const CUTS_Port_Agent_Set & agents)
  {
    // Set the size of the sequence.
    pms.length (agents.size ());
    size_t i = 0;

    for (CUTS_Port_Agent_Set::const_iterator iter = agents.begin ();
         iter != agents.end ();
         iter ++)
    {
      Port_Measurement & pm = pms[i ++];
      pm.port = CORBA::string_dup ((*iter)->name ());

      pm.measurements << (*iter)->port_measurements ();
    }
  }

  void operator << (Mapped_Port_Measurement_Seq & mpms,
                    CUTS_Port_Measurement_Map & pmm)
  {
    mpms.length (pmm.current_size ());

    CUTS_Port_Measurement_Map::iterator pmm_iter (pmm);
    size_t i = 0;

    for ( ; !pmm_iter.done (); pmm_iter ++)
    {
      Mapped_Port_Measurement & mpm = mpms[i ++];

      mpm.owner = (*pmm_iter).ext_id_;
      mpm << *(*pmm_iter).int_id_;

      (*pmm_iter).int_id_->reset ();
    }
  }

  void operator << (Mapped_Port_Measurement & mpm,
                    const CUTS_Port_Measurement & pm)
  {
    mpm.process_time << pm.process_time ();
    mpm.transit_time << pm.transit_time ();
    mpm.exit_times << pm.exit_points ();
  }

  void operator << (Exit_Point_Time_Seq & epts,
                    const CUTS_Port_Measurement::Exit_Points & ep)
  {
    epts.length (ep.size ());

    size_t i = 0;

    CUTS_Port_Measurement::Exit_Points::const_iterator iter;
    for (iter = ep.begin (); iter != ep.end (); iter ++)
    {
      Exit_Point_Time & ept = epts[i ++];

      ept.exit_point_ = CORBA::string_dup (iter->first.c_str ());
      ept.time_sample_ << iter->second;
    }
  }

  void operator << (Time_Sample & sample,
                    const CUTS_Time_Measurement & pm)
  {
    sample.count = pm.count ();
    sample.time.min = pm.minimum ().msec ();
    sample.time.max = pm.maximum ().msec ();
    sample.time.total = pm.accumulation ().msec ();
  }
}
