// $Id$
//
// = FILENAME
//    NavWeapDataHandler.java
//
// = AUTHOR
//    Michael Kircher (mk1@cs.wustl.edu)
//
// = DESCRIPTION
//   This is an implementation of the interface Data Handler,
//   it handles Navigation and Weapons data, where statistic data is
//   part of it. Several Observables are declared, they can be accessed by
//   any number of Observers, which could reside in a Java Bean for example.
//
// ============================================================================

import org.omg.CORBA.*;

public class NavWeapDataHandler implements DataHandler {

  java.util.Hashtable ObservablesTable;
  int received_events_;


  // Observable for Persian Recursion data
  class PersianObservable extends DemoObservable {

    // to ask which kind of viewer is needed to display data
    public int getProperty () {
	return Properties.PERSIAN;
    }

    public void updatePersianData (PersianRecursion.Data data) {

      //      System.out.println ("in PersianObservable.updatePersianData");

      setChanged ();
      notifyObservers (data);
    }
  }


  // Observable for Navigation data
  class NavigationObservable extends DemoObservable {

    // to ask which kind of viewer is needed to display data
    public int getProperty () {
	return Properties.NAVIGATION;
    }

    public void updateNavigation (Navigation navigation) {
      setChanged ();
      notifyObservers (navigation);
    }
  }

  class WeaponsObservable extends DemoObservable {

    // to ask which kind of viewer is needed to display data
    public int getProperty () {
	return Properties.WEAPONS;
      }

    public void updateWeapons (Weapons weapons) {
      setChanged ();
      notifyObservers (weapons);
    }
  }

  class Cpu_UsageObservable extends DemoObservable {

    public int getProperty () {
      return Properties.DOUBLE;
    }

    public void updateCpu_Usage (double utilization) {
      setChanged ();
      Double temp_ = new Double (utilization);
      notifyObservers (temp_);
    }
  }

  class OverheadObservable extends DemoObservable {

    public int getProperty () {
      return Properties.DOUBLE;
    }

    public void updateOverhead (double overhead) {
      setChanged ();
      Double temp_ = new Double (overhead);
      notifyObservers (temp_);
    }
  }

  class JitterObservable extends DemoObservable {
    double latency = 0.0;
    double avg_latency = 0.0;
    double sample_count = 0.0;

    public int getProperty () {
      return Properties.DOUBLE;
    }

    public void updateJitter (long completion_time,
		              long computation_time,
		              long arrival_time) {

      latency = (double)(completion_time) -
                (double)(computation_time) -
                (double)(arrival_time);
      latency = latency > 0 ? latency : 0;

      sample_count = sample_count + 1.0;
      avg_latency = (avg_latency * (sample_count - 1.0) + latency) /
                    (sample_count);

      double jitter_ = (double)Math.abs(latency - avg_latency);

      setChanged ();
      Double temp_ = new Double (jitter_);
      notifyObservers (temp_);
    }
  }

  class DeadlinesObservable extends DemoObservable {

    public int getProperty () {
      return Properties.DOUBLE;
    }

    public void updateDeadlines (long deadline_time,
				 long completion_time) {

      double missed_ = (deadline_time < completion_time) ? 1.0 : 0.0;
      Double temp_ = new Double (missed_);
      setChanged ();
      notifyObservers (temp_);
    }
  }

  class CriticalDeadlinesObservable extends DeadlinesObservable {

    public int getProperty () {
      return Properties.DOUBLE;
    }

    public void updateDeadlines (long deadline_time,
				 long completion_time,
                                 long criticality) {

      double missed_ = ((criticality > 0) &&
                        (deadline_time < completion_time)) ? 1.0 : 0.0;
      Double temp_ = new Double (missed_);
      setChanged ();
      notifyObservers (temp_);
    }
  }

  class LatencyObservable extends DemoObservable {
    double latency = 0.0;
    double last_latency = 0.0;

    public int getProperty () {
      return Properties.DOUBLE;
    }

    public void updateLatency (long computation_time) {
      last_latency = latency;
      latency = (double)(computation_time);
      latency = latency > 0 ? latency : 0;

      setChanged ();
      Double temp_ = new Double(latency);
      notifyObservers (temp_);
    }

    public void updateLatency (long completion_time,
			       long computation_time,
			       long arrival_time) {
      last_latency = latency;
      latency = (double)(completion_time) -
                (double)(computation_time) -
                (double)(arrival_time);
      latency = latency > 0 ? latency : 0;

      setChanged ();
      Double temp_ = new Double(latency);
      notifyObservers (temp_);
    }
  }


  public synchronized void update (RtecEventComm.Event event) {

    // System.out.println ("in NavWeapDataHandler.update");

    Any any_value;
    PersianRecursion.Data persian_recursion_data;

    any_value = event.data.any_value;

    if (any_value.type().equal (PersianRecursion.DataHelper.type()))
      {
	//        System.out.println ("type matched PersianRecursion.Data");

        try
	  {
            persian_recursion_data =
              PersianRecursion.DataHelper.extract (any_value);
	  }
        catch (Exception e)
	  {
            System.err.println (e.getMessage () +
                                "\nThe stack trace is:\n");
            e.printStackTrace ();
            return;
	  }

	//       System.out.println ("extracted any");

        if (persian_recursion_data.criticality_level.equals (RtecScheduler.Criticality_t.HIGH_CRITICALITY)
            || persian_recursion_data.criticality_level.equals (RtecScheduler.Criticality_t.VERY_HIGH_CRITICALITY))
	  {
	    //            System.out.println ("obtaining high priority persian recursion observable");

	    PersianObservable pobs_hi =
              (PersianObservable) ObservablesTable.get ("High Consumer Persian Recursion");

	    //            System.out.println ("updating high priority persian recursion observable");

	    pobs_hi.updatePersianData (persian_recursion_data);

            // LatencyObservable lobs_hi =
	    // (LatencyObservable) ObservablesTable.get ("High Consumer Execution Time (100 ns)");

	    // lobs_hi.updateLatency (persian_recursion_data.computation_time);
	  }
        else
	  {
	    //            System.out.println ("obtaining low priority persian recursion observable");
	    PersianObservable pobs_lo =
              (PersianObservable) ObservablesTable.get ("Low Consumer Persian Recursion");

	    //            System.out.println ("obtained low priority persian recursion observable");
            // System.out.println ("updating low priority persian recursion observable");

            pobs_lo.updatePersianData (persian_recursion_data);

	    //            System.out.println ("updated low priority persian recursion observable");

            // LatencyObservable lobs_lo =
	    // (LatencyObservable) ObservablesTable.get ("Low Consumer Execution Time (100 ns)");

	    // lobs_lo.updateLatency (persian_recursion_data.computation_time);
	  }

	//        System.out.println ("done updating PersianObservables");

	received_events_++;

	//        System.out.println ("total events received: " + received_events_);
     }
    else if (any_value.type().equal (NavigationHelper.type()))
      {
	Navigation navigation_ = NavigationHelper.extract (any_value);

        // if the navigation data structure's update data flag is set,
        // update its scheduling data with actual values from the EC
        if (navigation_.update_data > 0)
          {
            navigation_.arrival_time = event.header.creation_time;
            navigation_.completion_time = event.header.ec_send_time;
            navigation_.deadline_time += event.header.creation_time;
          }

	NavigationObservable nobs = (NavigationObservable)ObservablesTable.get ("Navigation");
	nobs.updateNavigation (navigation_);
	Cpu_UsageObservable cobs = (Cpu_UsageObservable)ObservablesTable.get ("CPU Usage");
	cobs.updateCpu_Usage (navigation_.utilization);
	OverheadObservable oobs = (OverheadObservable)ObservablesTable.get ("Overhead");
	oobs.updateOverhead (navigation_.overhead);
	JitterObservable jobs = (JitterObservable)ObservablesTable.get ("Latency Jitter (100 ns)");
	jobs.updateJitter (navigation_.completion_time,
			   navigation_.computation_time,
			   navigation_.arrival_time);
	JitterObservable njobs = (JitterObservable)ObservablesTable.get ("Navigation Latency Jitter (100 ns)");
	njobs.updateJitter (navigation_.completion_time,
			    navigation_.computation_time,
			    navigation_.arrival_time);
	DeadlinesObservable dobs = (DeadlinesObservable)ObservablesTable.get ("Missed Deadlines");
	dobs.updateDeadlines (navigation_.deadline_time,
			      navigation_.completion_time);
	CriticalDeadlinesObservable cdobs =
          (CriticalDeadlinesObservable)ObservablesTable.get ("Missed Critical Deadlines");
	cdobs.updateDeadlines (navigation_.deadline_time,
			       navigation_.completion_time,
			       navigation_.criticality);
	LatencyObservable lobs = (LatencyObservable)ObservablesTable.get ("Latency (100 ns)");
	lobs.updateLatency (navigation_.completion_time,
			    navigation_.computation_time,
			    navigation_.arrival_time);
	LatencyObservable nlobs = (LatencyObservable)ObservablesTable.get ("Navigation Latency (100 ns)");
	nlobs.updateLatency (navigation_.completion_time,
                             navigation_.computation_time,
                             navigation_.arrival_time);
	received_events_++;
      }
    else if (any_value.type().equal (WeaponsHelper.type()))
      {
	Weapons weapons_ = WeaponsHelper.extract (any_value);

        // if the weapons structure's update data flag is set, update
        // itss scheduling data with actual values from the EC
        if (weapons_.update_data > 0)
          {
            weapons_.arrival_time = event.header.creation_time;
            weapons_.completion_time = event.header.ec_send_time;
            weapons_.deadline_time += event.header.creation_time;
          }

	WeaponsObservable wobs = (WeaponsObservable)ObservablesTable.get ("Weapons");;
	wobs.updateWeapons (weapons_);
	Cpu_UsageObservable cobs = (Cpu_UsageObservable)ObservablesTable.get ("CPU Usage");
	cobs.updateCpu_Usage (weapons_.utilization);
	OverheadObservable oobs = (OverheadObservable)ObservablesTable.get ("Overhead");
	oobs.updateOverhead (weapons_.overhead);
	JitterObservable jobs = (JitterObservable)ObservablesTable.get ("Latency Jitter (100 ns)");
	jobs.updateJitter (weapons_.completion_time,
			   weapons_.computation_time,
			   weapons_.arrival_time);
	JitterObservable wjobs = (JitterObservable)ObservablesTable.get ("Weapons Latency Jitter (100 ns)");
	wjobs.updateJitter (weapons_.completion_time,
		 	    weapons_.computation_time,
			    weapons_.arrival_time);
	DeadlinesObservable dobs = (DeadlinesObservable)ObservablesTable.get ("Missed Deadlines");
	dobs.updateDeadlines (weapons_.deadline_time,
			      weapons_.completion_time);
	CriticalDeadlinesObservable cdobs = (CriticalDeadlinesObservable)ObservablesTable.get ("Missed Critical Deadlines");
	cdobs.updateDeadlines (weapons_.deadline_time,
			       weapons_.completion_time,
			       weapons_.criticality);
	LatencyObservable lobs = (LatencyObservable)ObservablesTable.get ("Latency (100 ns)");
	lobs.updateLatency (weapons_.completion_time,
			    weapons_.computation_time,
			    weapons_.arrival_time);
	LatencyObservable wlobs = (LatencyObservable)ObservablesTable.get ("Weapons Latency (100 ns)");
	wlobs.updateLatency (weapons_.completion_time,
                             weapons_.computation_time,
                             weapons_.arrival_time);
	received_events_++;
      }
    else
      {
       	System.out.println ("Received wrong type information");

	System.out.println ("Received any_value.type (): [" +
                            any_value.type() + "]");

	System.out.println ("Expected NavigationHelper.type (): [" +
                            NavigationHelper.type() + "]");

	System.out.println ("OR WeaponsHelper.type (): [" +
                            WeaponsHelper.type() + "]");

	System.out.println ("OR PersianRecursion.DataHelper.type (): [" +
                            PersianRecursion.DataHelper.type() + "]");
      }
  }

  NavWeapDataHandler () {
    ObservablesTable = new java.util.Hashtable();

    ObservablesTable.put ("Navigation", new NavigationObservable());
    ObservablesTable.put ("Weapons", new WeaponsObservable());
    ObservablesTable.put ("CPU Usage", new Cpu_UsageObservable());
    ObservablesTable.put ("Overhead", new OverheadObservable());
    ObservablesTable.put ("Latency Jitter (100 ns)", new JitterObservable());
    ObservablesTable.put ("Navigation Latency Jitter (100 ns)", new JitterObservable());
    ObservablesTable.put ("Weapons Latency Jitter (100 ns)", new JitterObservable());
    ObservablesTable.put ("Missed Deadlines", new DeadlinesObservable());
    ObservablesTable.put ("Missed Critical Deadlines", new CriticalDeadlinesObservable());
    ObservablesTable.put ("Latency (100 ns)", new LatencyObservable());
    ObservablesTable.put ("Weapons Latency (100 ns)", new LatencyObservable());
    ObservablesTable.put ("Navigation Latency (100 ns)", new LatencyObservable());
    ObservablesTable.put ("High Consumer Persian Recursion", new PersianObservable());
    ObservablesTable.put ("Low Consumer Persian Recursion", new PersianObservable());
    ObservablesTable.put ("High Consumer Execution Time (100 ns)", new LatencyObservable());
    ObservablesTable.put ("Low Consumer Execution Time (100 ns)", new LatencyObservable());
  }

  public java.util.Enumeration getObservablesList () {
    return ObservablesTable.keys ();
  }

  public DemoObservable getObservable(String name) {
    return (DemoObservable)ObservablesTable.get (name);
  }

  public int getObservableProperty (String name) {
    DemoObservable obs = (DemoObservable)ObservablesTable.get (name);
    return obs.getProperty ();
  }
}



