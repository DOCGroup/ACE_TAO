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


  public PushConsumerFactory (DataHandler dataHandler, String nameServiceIOR, java.applet.Applet applet)
    {
      try {      
	dataHandler_ = dataHandler;
	orb_ = org.omg.CORBA.ORB.init (applet, null);
	boa_ = orb_.BOA_init ();
	
	// Get the Naming Service initial reference
	
	if (nameServiceIOR == null) {      
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
	      System.exit (1);
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
	  PushConsumer pushConsumer_ = new PushConsumer (orb_, dataHandler_);
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
	  System.err.println ("Client run: Failure");
	  System.err.println(e);
	}	
    }

    
} // public class PushConsumerFactory







