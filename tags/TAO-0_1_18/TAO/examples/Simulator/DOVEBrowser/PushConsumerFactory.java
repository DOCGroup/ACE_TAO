// $Id$
//
// ============================================================================
// 
// = FILENAME
//    PushConsumerFactory.java
//
// = AUTHOR
//    Michael Kircher (mk1@cs.wustl.edu)
//
// = DESCRIPTION
//   This is the administor/factory for a PushConsumer.
//
// ============================================================================




public class PushConsumerFactory {

  private org.omg.CORBA.ORB orb_;
  private org.omg.CORBA.BOA boa_;
  private org.omg.CORBA.Object naming_service_object_;

  private DataHandler dataHandler_;
  private Navigation navigation_;
  private Weapons weapons_;


  public PushConsumerFactory (DataHandler dataHandler, 
			      String nameServiceIOR, 
			      String[] args,
			      java.applet.Applet applet)
    {
      try 
	{      
	  dataHandler_ = dataHandler;
	  
	  // if the DOVE Browser is running as an Applet
	  if (applet != null) {
	    orb_ = org.omg.CORBA.ORB.init (applet, null);
	  }
	  else { // not running as an Applet, but as an normal Application
	    orb_ = org.omg.CORBA.ORB.init ();
	  }
	  boa_ = orb_.BOA_init ();
	  
	  // Get the Naming Service initial reference
	  
	  // Name Service Lookup cannot be used when running as an Applet
	  if (nameServiceIOR == null && applet != null) {
	    System.out.println (" Name Service Lookup cannot be used when running as an Applet! Quit!");
	    System.exit (1);
	  }
	  
	  if (nameServiceIOR == null) {  // only used when running via "java" or "vbj"     
	    System.out.println ("Using the lookup protocol!");
	    NS_Resolve ns_resolve_ = new NS_Resolve ();
		naming_service_object_ = ns_resolve_.resolve_name_service (orb_);
	  }
	  else {
	    System.out.println ("Using the following IOR: " + nameServiceIOR);
	    naming_service_object_ = orb_.string_to_object (nameServiceIOR);
	  }
	  
      } 
      catch(org.omg.CORBA.SystemException e) {
	System.err.println ("PushConsumerFactory constructor: ORB and Name Service initialization");
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
	      System.exit (1);
	    }
	  System.out.println ("Reference to the Naming Service is ok.");

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
	  
	  System.out.println ("Reference to the Event Service is ok.");
	  
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
	  
	  System.out.println ("Reference to the Naming Service is ok.");
	  
	  
	  // Start the consumer
	  System.out.println ("Instantiating the Push Consumer.");
	  PushConsumer pushConsumer_ = new PushConsumer (orb_, dataHandler_);
	  System.out.println ("Initializing the Push Consumer.");
	  pushConsumer_.open_consumer (event_channel_, scheduler_, "demo_consumer");
	  
	  // Tell the CORBA environment that we are ready
	  
	  boa_.obj_is_ready (pushConsumer_);
	  
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
	  System.err.println ("PushConsumerFactory.run: Failure");
	  System.err.println(e);
	}	
    }

    
} // public class PushConsumerFactory







