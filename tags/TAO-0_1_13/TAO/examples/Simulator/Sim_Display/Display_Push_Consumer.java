// $Id$
//
// ============================================================================
//
// 
// = FILENAME
//    Display_Push_Consumer.java
//
// = AUTHOR
//    Michael Kircher (mk1@cs.wustl.edu)
//
// = DESCRIPTION
//    Implemenation of a PushConsumer
// 
//
// ============================================================================





// The Consumer has to implement the Skeleton Consumer

public class Display_Push_Consumer extends RtecEventComm._PushConsumerImplBase
{
  
  public static final int ACE_ES_EVENT_ANY = 0;
  public static final int ACE_ES_EVENT_SHUTDOWN = 1;
  public static final int ACE_ES_EVENT_ACT = 2;
  public static final int ACE_ES_EVENT_NOTIFICATION = 3;
  public static final int ACE_ES_EVENT_TIMEOUT = 4;
  public static final int ACE_ES_EVENT_INTERVAL_TIMEOUT = 5;
  public static final int ACE_ES_EVENT_DEADLINE_TIMEOUT = 6;
  public static final int ACE_ES_GLOBAL_DESIGNATOR = 7;
  public static final int ACE_ES_CONJUNCTION_DESIGNATOR = 8;
  public static final int ACE_ES_DISJUNCTION_DESIGNATOR = 9;
  public static final int ACE_ES_EVENT_UNDEFINED = 16;
  public static final int TOTAL_MESSAGES = 30;

  // Store the number of received events
  private int total_received_ = 0;
  private org.omg.CORBA.ORB orb_;
  private Navigation navigation_;
  private Weapons weapons_;
  private Display display_;
  private RtecScheduler.handle_tHolder rt_info_;
  private RtecEventChannelAdmin.EventChannel channel_admin_;
  private RtecEventChannelAdmin.ConsumerAdmin consumer_admin_;
  private RtecEventChannelAdmin.ProxyPushSupplier suppliers_;
  
  public Display_Push_Consumer (org.omg.CORBA.ORB orb, Display display)
    {
      orb_ = orb;
      display_ = display;
    }
  
  
  public void push (RtecEventComm.Event[] events)
    {
      //if (total_received_ < 5)	  
      //	System.out.println ("Demo Consumer: Received an event! ->Number: " + total_received_);
      //else if (total_received_ == 5)
      //	System.out.println ("Demo Consumer: Everything is fine. Going to be mute.");
      
      
      if (events.length == 0)
	{
	    System.err.println ("No events");
	}
      else
	{
	  total_received_++;
	  /*
	    if (total_received_ >= TOTAL_MESSAGES)
	      {
	      orb.disconnect (this);
	      System.exit (0);
	      } 
	      */
	  for (int i = 0; i < events.length; ++i)
	    {
	      if(events[i].type_ == ACE_ES_EVENT_NOTIFICATION) 
		{
		  try
		    {		
		      if (events[i].data_.any_value.type().equal (NavigationHelper.type()))
			{
			    navigation_ = NavigationHelper.extract (events[i].data_.any_value);
			    display_.update_metrics (navigation_.utilization,
						     navigation_.overhead,
						     navigation_.arrival_time,
						     navigation_.deadline_time,
						     navigation_.completion_time,
						     navigation_.computation_time);
			    display_.update_simulation (Display_Object_Factory.ART_HORIZON_ENUM, this);
			}
		      else if (events[i].data_.any_value.type().equal (WeaponsHelper.type()))
			{
			  weapons_ = WeaponsHelper.extract (events[i].data_.any_value);
			  display_.update_metrics (weapons_.utilization,
						   weapons_.overhead,
						   weapons_.arrival_time,
						   weapons_.deadline_time,
						   weapons_.completion_time,
						   weapons_.computation_time);
			    display_.update_simulation (Display_Object_Factory.WEAPONS_ENUM, this);			    
			}
		      else 
			{		     
			  System.out.println ("Demo Consumer: Received an event! ->Number: " + total_received_);
			  System.out.println (">>TypeCode in the any does not match!");
			  System.out.println ("Id: " + events[i].data_.any_value.type().id ());
			  System.out.println ("Name: " + events[i].data_.any_value.type().name ());
			  System.out.println ("Kind: " + events[i].data_.any_value.type().kind ());			
			}		
		    }
		  catch (org.omg.CORBA.TypeCodePackage.BadKind e)
		    {
		      System.err.println (e);
		    }
		  catch(org.omg.CORBA.SystemException e) 
		    {
		      System.err.println(e);
		    }
		}		
	    }
	}
    }
  
  public void disconnect_push_consumer() 
    {
      System.out.println ("Demo Consumer: Have to disconnect!");
    }
  
  public void open_consumer (RtecEventChannelAdmin.EventChannel event_channel_,
			     RtecScheduler.Scheduler scheduler_,
			     String name)
    {
      try {
	
	// Define Real-time information
	
	rt_info_ = new RtecScheduler.handle_tHolder (scheduler_.create (name));
	
	scheduler_.set (rt_info_.value,
			new TimeBase.ulonglong (0,0), 
			new TimeBase.ulonglong (0,0),
			new TimeBase.ulonglong (0,0),
			2500000,  // period
			RtecScheduler.Importance.VERY_LOW_IMPORTANCE,
			new TimeBase.ulonglong (0,0),
			1);	  	        
	
	
	// Register for Notification and Shutdown events

	
	RtecEventComm.Event disjunction_designator_ = 
	  new RtecEventComm.Event (ACE_ES_DISJUNCTION_DESIGNATOR,  0, 
				     1,        // ttl
				   new TimeBase.ulonglong (0,0),
				   new TimeBase.ulonglong (0,0),
				   new TimeBase.ulonglong (0,0),
				   new RtecEventComm.EventData (0, 0, orb_.create_any())
				    );
	RtecEventComm.Event notification_event_ = 
	  new RtecEventComm.Event (ACE_ES_EVENT_NOTIFICATION,  0, 
				   1,        // ttl
				   new TimeBase.ulonglong (0,0),
				   new TimeBase.ulonglong (0,0),
				   new TimeBase.ulonglong (0,0),
				   new RtecEventComm.EventData (0, 0, orb_.create_any())
				    );
	RtecEventComm.Event shutdown_event_ = 
	    new RtecEventComm.Event (ACE_ES_EVENT_SHUTDOWN,  0, 
				     1,        // ttl
				     new TimeBase.ulonglong (0,0),
				     new TimeBase.ulonglong (0,0),
				     new TimeBase.ulonglong (0,0),
				     new RtecEventComm.EventData (0, 0, orb_.create_any())
				      );
	
	
	RtecEventChannelAdmin.Dependency dependencies_[] = new RtecEventChannelAdmin.Dependency[3];  
	dependencies_[0] = new RtecEventChannelAdmin.Dependency (disjunction_designator_, rt_info_.value);
	dependencies_[1] = new RtecEventChannelAdmin.Dependency (notification_event_, rt_info_.value);
	dependencies_[2] = new RtecEventChannelAdmin.Dependency (shutdown_event_, rt_info_.value); 
	
	
	
	RtecEventChannelAdmin.ConsumerQOS qos = new RtecEventChannelAdmin.ConsumerQOS (dependencies_); 
	  
	
	// The channel administrator is the event channel we got from the invocation
	// of this routine
	
	channel_admin_ = event_channel_;
	
	// Connect as a consumer
	
	consumer_admin_ = channel_admin_.for_consumers ();
	
	// Obtain a reference to the proxy push supplier
	
	suppliers_ = consumer_admin_.obtain_push_supplier ();

	suppliers_.connect_push_consumer (this, qos);
	
	System.out.println ("Registered the consumer successfully.");
	
	 
      }
      catch (RtecScheduler.UNKNOWN_TASK e)
	{
	  System.err.println ("Demo_Consumer.open_consumer: Unknown task");
	  System.err.println (e);
	}
      catch (RtecScheduler.DUPLICATE_NAME e)
	{
	  System.err.println ("Demo_Consumer.open_consumer: Duplicate names");
	  System.err.println (e);
	  }
      catch(org.omg.CORBA.SystemException e) 
	{
	  System.err.println(e);
	}		
    }
  public Navigation get_navigation ()
    {
	return navigation_;    
    }
  
  public Weapons get_weapons ()
    {
      return weapons_;
    } 
  
}
  









