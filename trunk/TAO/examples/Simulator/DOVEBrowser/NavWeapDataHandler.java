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
        
    public void updateJitter (int completion_time_secs,
                              int completion_time_usecs,
		              int computation_time_secs,
		              int computation_time_usecs,
		              int arrival_time_secs,
		              int arrival_time_usecs) {

      latency = 1000000.0 * (double)(completion_time_secs) +
                (double)(completion_time_usecs) -
                1000000.0 * (double)(computation_time_secs) -
                (double)(computation_time_usecs) -
                1000000.0 * (double)(arrival_time_secs) -
                (double)(arrival_time_usecs);
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
    
    public void updateDeadlines (int deadline_time_secs, 
                                 int deadline_time_usecs, 
				 int completion_time_secs,
				 int completion_time_usecs) {

      double missed_ = 0.0;

      // Assumes data values are already normalized
      if ((deadline_time_secs < completion_time_secs) ||
          ((deadline_time_secs == completion_time_secs) &&
           (deadline_time_usecs < completion_time_usecs)))
        {
          missed_ = 1.0;
	}      	

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
    
    public void updateLatency (int completion_time_secs,
                               int completion_time_usecs,
			       int computation_time_secs,
			       int computation_time_usecs,
			       int arrival_time_secs,
			       int arrival_time_usecs) {
      last_latency = latency;
      latency = 1000000.0 * (double)(completion_time_secs) +
                (double)(completion_time_usecs) -
                1000000.0 * (double)(computation_time_secs) -
                (double)(computation_time_usecs) -
                1000000.0 * (double)(arrival_time_secs) -
                (double)(arrival_time_usecs);
      latency = latency > 0 ? latency : 0;
      
      setChanged ();
      Double temp_ = new Double(latency);
      notifyObservers (temp_);	
    }
  }
  
  
  public void update (Any any_value) {
    
    if (any_value.type().equal (NavigationHelper.type()))
      {
	Navigation navigation_ = NavigationHelper.extract (any_value);
	NavigationObservable nobs = (NavigationObservable)ObservablesTable.get ("NavigationObservable");
	nobs.updateNavigation (navigation_);
	Cpu_UsageObservable cobs = (Cpu_UsageObservable)ObservablesTable.get ("Cpu_UsageObservable");
	cobs.updateCpu_Usage (navigation_.utilization);
	OverheadObservable oobs = (OverheadObservable)ObservablesTable.get ("OverheadObservable");
	oobs.updateOverhead (navigation_.overhead);
	JitterObservable jobs = (JitterObservable)ObservablesTable.get ("JitterObservable");
	jobs.updateJitter (navigation_.completion_time_secs,
                           navigation_.completion_time_usecs,
			   navigation_.computation_time_secs,
			   navigation_.computation_time_usecs,
			   navigation_.arrival_time_secs,
			   navigation_.arrival_time_usecs);
	DeadlinesObservable dobs = (DeadlinesObservable)ObservablesTable.get ("DeadlinesObservable");
	dobs.updateDeadlines (navigation_.deadline_time_secs,
	                      navigation_.deadline_time_usecs,
			      navigation_.completion_time_secs,
			      navigation_.completion_time_usecs);
	LatencyObservable lobs = (LatencyObservable)ObservablesTable.get ("LatencyObservable");
	lobs.updateLatency (navigation_.completion_time_secs,
	                    navigation_.completion_time_usecs,
			    navigation_.computation_time_secs,
			    navigation_.computation_time_usecs,
			    navigation_.arrival_time_secs,
			    navigation_.arrival_time_usecs);
	received_events_++;
      }
    else if (any_value.type().equal (WeaponsHelper.type()))
      {
	Weapons weapons_ = WeaponsHelper.extract (any_value);
	WeaponsObservable wobs = (WeaponsObservable)ObservablesTable.get ("WeaponsObservable");;
	wobs.updateWeapons (weapons_);
	Cpu_UsageObservable cobs = (Cpu_UsageObservable)ObservablesTable.get ("Cpu_UsageObservable");
	cobs.updateCpu_Usage (weapons_.utilization);
	OverheadObservable oobs = (OverheadObservable)ObservablesTable.get ("OverheadObservable");
	oobs.updateOverhead (weapons_.overhead);
	JitterObservable jobs = (JitterObservable)ObservablesTable.get ("JitterObservable");
	jobs.updateJitter (weapons_.completion_time_secs,
	                   weapons_.completion_time_usecs,
			   weapons_.computation_time_secs,
			   weapons_.computation_time_usecs,
			   weapons_.arrival_time_secs,
			   weapons_.arrival_time_usecs);
	DeadlinesObservable dobs = (DeadlinesObservable)ObservablesTable.get ("DeadlinesObservable");
	dobs.updateDeadlines (weapons_.deadline_time_secs,
	                      weapons_.deadline_time_usecs,
			      weapons_.completion_time_secs,
			      weapons_.completion_time_usecs);
	LatencyObservable lobs = (LatencyObservable)ObservablesTable.get ("LatencyObservable");
	lobs.updateLatency (weapons_.completion_time_secs,
	                    weapons_.completion_time_usecs,
			    weapons_.computation_time_secs,
			    weapons_.computation_time_usecs,
			    weapons_.arrival_time_secs,
			    weapons_.arrival_time_usecs);
	received_events_++;
      }
	else 
	System.out.println ("Received wrong type information"+any_value.type());
  }
  
  NavWeapDataHandler () {
    ObservablesTable = new java.util.Hashtable();

    ObservablesTable.put ("NavigationObservable", new NavigationObservable());
    ObservablesTable.put ("WeaponsObservable", new WeaponsObservable());
    ObservablesTable.put ("Cpu_UsageObservable", new Cpu_UsageObservable());
    ObservablesTable.put ("OverheadObservable", new OverheadObservable());
    ObservablesTable.put ("JitterObservable", new JitterObservable());
    ObservablesTable.put ("DeadlinesObservable", new DeadlinesObservable());
    ObservablesTable.put ("LatencyObservable", new LatencyObservable()); 
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


    
    
