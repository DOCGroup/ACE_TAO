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
    double last_latency = 0.0;
        
    public int getProperty () {
      return Properties.DOUBLE;
    }
        
    public void updateJitter (int completion_time,
			       int computation_time,
			       int arrival_time ) {
      last_latency = latency;
      latency = (double)(completion_time
                         - computation_time
                         - arrival_time);
      latency = latency > 0 ? latency : 0;      
      double jitter_ = (double)Math.abs(latency - last_latency); 

      setChanged ();
      Double temp_ = new Double (jitter_);
      notifyObservers (temp_);
    }
  }
  class DeadlinesObservable extends DemoObservable {
    
    public int getProperty () {
      return Properties.DOUBLE;
    }
    
    public void updateDeadlines (int deadline_time, 
				 int completion_time) {
      double missed_ = (double) ((deadline_time 
				  < completion_time) 
				 ? 1.0 : 0.0);      	

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
    
    public void updateLatency (int completion_time,
			       int computation_time,
			       int arrival_time ) {
      last_latency = latency;
      latency = (double)(completion_time
                         - computation_time
                         - arrival_time);
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
	jobs.updateJitter (navigation_.completion_time,
			  navigation_.computation_time,
			  navigation_.arrival_time);
	DeadlinesObservable dobs = (DeadlinesObservable)ObservablesTable.get ("DeadlinesObservable");
	dobs.updateDeadlines (navigation_.deadline_time,
			     navigation_.completion_time);
	LatencyObservable lobs = (LatencyObservable)ObservablesTable.get ("LatencyObservable");
	lobs.updateLatency (navigation_.completion_time,
			   navigation_.computation_time,
			   navigation_.arrival_time);
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
	jobs.updateJitter (weapons_.completion_time,
			  weapons_.computation_time,
			  weapons_.arrival_time);
	DeadlinesObservable dobs = (DeadlinesObservable)ObservablesTable.get ("DeadlinesObservable");
	dobs.updateDeadlines (weapons_.deadline_time,
			     weapons_.completion_time);
	LatencyObservable lobs = (LatencyObservable)ObservablesTable.get ("LatencyObservable");
	lobs.updateLatency (weapons_.completion_time,
			   weapons_.computation_time,
			   weapons_.arrival_time);
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


    
    
