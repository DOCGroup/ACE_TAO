// $Id$ 


import java.io.*;
import java.awt.*;

public class Display extends Panel
{
  private Graph_Panel metrics_ = new Graph_Panel ();
  private Sim_Panel sims_ = new Sim_Panel ();
  private double last_latency = 0;
  
  public Display ()
    {
      setLayout (new GridLayout (2, 1));
      add (new Border_Panel (sims_, "Flight Simulator"));
      add (new Border_Panel (metrics_, "Scheduling Performance"));
    }
  
  public void update_metric (String name, float new_value)
    {
      metrics_.updateGraph (name, new_value);
    }
  
  public void add_metric (String name, int max)
    {
      metrics_.addGraph (name, max);
    }  
  
  public void add_simulator (String name)
    {
      Display_Object obj =
	Display_Object_Factory.create_display_object (name);

      if (obj != null)
	sims_.add_simulator (name, obj);
    }

  public void update_simulator (String name, Display_Push_Consumer display_push_consumer)
    {
      sims_.update_simulator (name, display_push_consumer);
    }

  
  public void update_metrics (double utilization,
			      double overhead,
			      int arrival_time,
			      int deadline_time,
			      int completion_time,
			      int computation_time)
    {
      float latency = (float)(completion_time-computation_time-arrival_time);
      latency =  latency > 0 ? latency : 0;
      float jitter = (float)Math.abs(latency - last_latency);
      // float latency = (float)(dispatch_time - arrival_time > 0 ? dispatch_time - arrival_time : 0);
      // float jitter = (float)Math.abs(latency - last_latency);
      float missed_deadline =
	(float)((deadline_time < completion_time) ? 1.0 : 0.0);

      /*
      System.out.println ("Metrics: cpu " + header.utilization_ +
			  " overhead " + header.overhead_ +
			  " jitter " + jitter +
			  " deadlines " + missed_deadline +
			  " latency " + latency);
			  */
      update_metric (Display_Client.CPU_USAGE, (float)utilization);
      update_metric (Display_Client.OVERHEAD, (float)overhead);
      update_metric (Display_Client.JITTER, jitter);
      update_metric (Display_Client.DEADLINES, missed_deadline);
      update_metric (Display_Client.LATENCY, latency);
    }

  public void update_simulation (long simulation_id,
				  Display_Push_Consumer display_push_consumer)
    {
       String sim_name =
      	Display_Object_Factory.resolve_name (simulation_id);

      if (sim_name != null)
	update_simulator (sim_name, display_push_consumer);
    }
}









