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
//   This Consumer connects to the Event Channel (TAO Event Channel).
//   It accepts Notification events containing Navigation and 
//   Weapons data.
// 
//
// ============================================================================




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

  public synchronized void init () 
    {
    }


public Display_Consumer (Display display) //, String name_service_ior, java.applet.Applet applet)
    {
      display_ = display;
      try {      
	orb_ = org.omg.CORBA.ORB.init (); // applet, null);
	boa_ = orb_.BOA_init ();
	
	// Get the Naming Service initial reference
	 
        //naming_service_object_ = orb_.string_to_object (name_service_ior);

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
	  Display_Push_Consumer display_push_consumer_ = new Display_Push_Consumer (orb_, display_);
	  display_push_consumer_.open_consumer (event_channel_, scheduler_, "demo_consumer");
	 
	  // Tell the CORBA environment that we are ready

	  boa_.obj_is_ready (display_push_consumer_);
	  
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

    
} // public class Display_Consumer







