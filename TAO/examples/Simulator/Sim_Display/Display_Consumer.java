// $Id$
//
// ============================================================================
//
// 
// = FILENAME
//    Display_Consumer.java
//
// = AUTHOR
//    Michael Kircher (mk1@cs.wustl.edu)
//
// = DESCRIPTION
//   This Client connects to the Event Channel (TAO Event Channel).
//   It accepts Notification events containing Navigation and 
//   Weapons data.
// 
//
// ============================================================================





import org.omg.CORBA.*;




public class Display_Consumer {

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

  private org.omg.CORBA.ORB orb_;
  private org.omg.CORBA.BOA boa_;
  private org.omg.CORBA.Object naming_service_object_;

  private Display display_;
  private Navigation navigation_;
  private Weapons weapons_;

  
  public Display_Consumer (Display display)
    {
      display_ = display;
      try {      
	orb_ = ORB.init ();
	boa_ = orb_.BOA_init ();
	
	// Get the Naming Service initial reference

	NS_Resolve ns_resolve_ = new NS_Resolve ();
     
	naming_service_object_ = ns_resolve_.resolve_name_service (orb_);
      } 
      catch(org.omg.CORBA.SystemException e) {
	System.err.println ("Client constructur: Failure");
	System.err.println(e);
      }	
      
    }

  public class Object_is_null_exception extends Exception 
  {
    Object_is_null_exception (String s)
      {
	super (s);
      }
  }
  
  public void run ()
    {
      try
	{
	  
	  // Get the Naming Context to allow resolving the EventService and
	  // ScheduleService
	  CosNaming.NamingContext naming_context_ = 
	    CosNaming.NamingContextHelper.narrow (naming_service_object_);

	  if (naming_context_ == null)
	    {
	      System.err.println ("The Naming Context is null");
	    }

	  // Get a reference for the EventService

	  CosNaming.NameComponent[] ec_name_components_ = new CosNaming.NameComponent[1];
	  ec_name_components_[0] = new CosNaming.NameComponent ("EventService","");
	  org.omg.CORBA.Object event_channel_object_ = naming_context_.resolve (ec_name_components_);

	  if (event_channel_object_ == null)
	    {
	      throw new Object_is_null_exception("EventService Object is null");
	    }
	 
       	  RtecEventChannelAdmin.EventChannel event_channel_ = 
	    RtecEventChannelAdmin.EventChannelHelper.narrow (event_channel_object_);

	  // Get a reference for the ScheduleService

	  CosNaming.NameComponent[] s_name_components_ = new CosNaming.NameComponent[1];
	  s_name_components_[0] = new CosNaming.NameComponent ("ScheduleService","");
       	  org.omg.CORBA.Object scheduler_object_ = naming_context_.resolve (s_name_components_); 

	  if (scheduler_object_ == null)
	    {
	      throw new Object_is_null_exception("ScheduleService Object is null");
	    }
	 
	  RtecScheduler.Scheduler scheduler_ = 
	    RtecScheduler.SchedulerHelper.narrow (scheduler_object_);
	 
 
	  // Start the consumer
	  Demo_Consumer demo_consumer_ = new Demo_Consumer ();
	  demo_consumer_.open_consumer (event_channel_, scheduler_, "demo_consumer");
	 
	  // Tell the CORBA environment that we are ready

	  boa_.obj_is_ready (demo_consumer_);
	  
	  System.out.println ("boa.obj_is_ready succeeded"); 

	  boa_.impl_is_ready ();
      }
      catch (CosNaming.NamingContextPackage.CannotProceed e)
	{
	  System.err.println ("CosNaming.NamingContextPackage.CannotProceed");
	  System.err.println (e);
	}
      catch (CosNaming.NamingContextPackage.InvalidName e)
	{
	  System.err.println ("CosNaming.NamingContextPackage.InvalidName");
	  System.err.println (e);
	}
      catch (CosNaming.NamingContextPackage.NotFound e)
	{
	  System.err.println ("CosNaming.NamingContextPackage.NotFound");
	  System.err.println (e);
	
	}
      catch (Object_is_null_exception e)
	{
	  System.err.println (e);
	}
      catch(org.omg.CORBA.SystemException e) 
	{
	  System.err.println ("Client run: Failure");
	  System.err.println(e);
	}	
    }

  // The Consumer has to implement the Skeleton Consumer

  public class Demo_Consumer extends RtecEventComm._sk_PushConsumer
  {
    // Store the number of received events
    private int total_received_ = 0;
    
    public void push (RtecEventComm.Event[] events)
      {
	System.out.println ("Demo Consumer: Received an event! ->Number: " + total_received_);
	  
	if (events.length == 0)
	  {
	    System.err.println ("No events");
	  }
	else
	  {
	    for (int i = 0; i < events.length; ++i)
	      {
		if (events[i].type_ == ACE_ES_EVENT_SHUTDOWN)
		  {
		    System.out.println ("Demo Consumer: received shutdown event.");
		    System.exit (0);
		  }
		else if(events[i].type_ == ACE_ES_EVENT_NOTIFICATION) 
		  {
		    try
		      {
			if (events[i].data_.any_value.type().equal (NavigationHelper.type()))
			  {
			    navigation_ = NavigationHelper.extract (events[i].data_.any_value);
			    display_.update_metrics (navigation_.utilitzation,
						     navigation_.overhead,
						     navigation_.arrival_time,
						     navigation_.deadline_time,
						     navigation_.dispatch_time);
			    display_.update_simulation (Display_Object_Factory.ART_HORIZON_ENUM, 
							Display_Consumer.this);
			  }
			else if (events[i].data_.any_value.type().equal (WeaponsHelper.type()))
			  {
			    weapons_ = WeaponsHelper.extract (events[i].data_.any_value);
			    display_.update_metrics (weapons_.utilitzation,
						     weapons_.overhead,
						     weapons_.arrival_time,
						     weapons_.deadline_time,
						     weapons_.dispatch_time);
			    display_.update_simulation (Display_Object_Factory.WEAPONS_ENUM, 
							Display_Consumer.this);
						     
			  }
			else 
			  {
			    System.out.println ("TypeCode in the any does not match!");
			  }		
		      }
		    catch(org.omg.CORBA.SystemException e) 
		      {
			System.err.println(e);
		      }
		  }
		else if(events[i].type_ == ACE_ES_EVENT_INTERVAL_TIMEOUT) 
		  {
		    System.out.println ("Demo Consumer: received ACE_ES_EVENT_INTERVAL_TIMEOUT event.");
		  }
		
	      }
	    total_received_++;
	    /*
	    if (total_received_ >= TOTAL_MESSAGES)
	      {
		orb.disconnect (this);
		System.exit (0);
	      } 
	      */
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
			  RtecScheduler.Importance.VERY_LOW,
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

    private RtecScheduler.handle_tHolder rt_info_;
    private RtecEventChannelAdmin.EventChannel channel_admin_;
    private RtecEventChannelAdmin.ConsumerAdmin consumer_admin_;
    private RtecEventChannelAdmin.ProxyPushSupplier suppliers_;
  }
  

    public Navigation get_navigation ()
      {
	return navigation_;    
      }

    public Weapons get_weapons ()
      {
	return weapons_;
      }   
    
} // public class Display_Consumer







