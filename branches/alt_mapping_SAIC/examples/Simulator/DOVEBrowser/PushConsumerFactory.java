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
  private org.omg.PortableServer.POA poa_;
  private org.omg.CORBA.Object naming_service_object_;

  private DataHandler dataHandler_;
  private Navigation navigation_;
  private Weapons weapons_;

  private String[] ec_names_ = null;
  private int ec_names_count_ = 0;

  private String[] ss_names_ = null;
  private int ss_names_count_ = 0;

  private boolean use_queueing_ = false;

  public PushConsumerFactory (DataHandler dataHandler,
			      String nameServiceIOR,
			      String nameServicePort,
			      String[] args,
                              boolean use_queueing,
			      java.applet.Applet applet)
    {
      try
	{
          use_queueing_ = use_queueing;
	  dataHandler_ = dataHandler;

	  // if the DOVE Browser is running as an Applet
	  if (applet != null) {
	    orb_ = org.omg.CORBA.ORB.init (applet, null);
	  }
	  else { // not running as an Applet, but as an normal Application
	    orb_ = org.omg.CORBA.ORB.init (args, null);
	  }

          System.out.println ("Initialized ORB");

          org.omg.CORBA.Object obj =
	    orb_.resolve_initial_references ("RootPOA");

          System.out.println ("Obtained RootPOA");

	  poa_ = org.omg.PortableServer.POAHelper.narrow (obj);

          System.out.println ("Narrowed RootPOA");

	  // Get the Naming Service initial reference

	  // Name Service Lookup cannot be used when running as an Applet
	  if (nameServiceIOR == null && applet != null) {
	    System.out.println (" Name Service Lookup cannot be used when running as an Applet! Quit!");
	    System.exit (1);
	  }

	  if (nameServiceIOR == null) {  // only used when running via "java" or "vbj"
	    System.out.println ("Using the lookup protocol!");
	    NS_Resolve ns_resolve_ = new NS_Resolve (nameServicePort);
		naming_service_object_ = ns_resolve_.resolve_name_service (orb_);
	  }
	  else {
	    System.out.println ("Using the following IOR: " + nameServiceIOR);
	    naming_service_object_ = orb_.string_to_object (nameServiceIOR);
	  }

          // Walk through args and count EC and Scheduling Service names.
          int arg_index = 0;
          while (args.length > arg_index)
            {
              // Count an event service name
              if ((args[arg_index].equals ("-eventservicename")) &&
                  (args.length > arg_index + 1))
                {
                  System.out.println ("switch [" + args[arg_index] + "]");
                  System.out.println ("value [" + args[arg_index + 1] + "]");
                  arg_index += 2;
                  ++ec_names_count_;
                }
              // Count a scheduling service name
              else if ((args[arg_index].equals ("-scheduleservicename")) &&
                  (args.length > arg_index + 1))
                {
                  System.out.println ("switch [" + args[arg_index] + "]");
                  System.out.println ("value [" + args[arg_index + 1] + "]");
                  arg_index += 2;
                  ++ss_names_count_;
                }
              else if (args[arg_index].equals ("-dualECdemo"))
                {
                  System.out.println ("switch [" + args[arg_index] + "]");
                  ++arg_index;
                  ec_names_count_ += 2;
                  ss_names_count_ += 2;
                }
              else if ((args[arg_index].equals ("-dualECdemo1")) ||
                       (args[arg_index].equals ("-dualECdemo2")))
                {
                  System.out.println ("switch [" + args[arg_index] + "]");
                  ++arg_index;
                  ++ec_names_count_;
                  ++ss_names_count_;
                }
              // Skip over anything else
              else
                {
                  arg_index ++;
                }
            }

          // Allocate arrays for the event and schedule service names (if any)
          if (ec_names_count_ > 0)
            {
              ec_names_ = new String [ec_names_count_];
            }
          if (ss_names_count_ > 0)
            {
              ss_names_ = new String [ss_names_count_];
            }

          // Then traverse the args again to allocate and fill in a list
          // of EC and Scheduling Service names.
          arg_index = 0;
          ec_names_count_ = 0;
          ss_names_count_ = 0;
          while (args.length > arg_index)
            {
              // Set an event service name.
              if ((args[arg_index].equals ("-eventservicename")) &&
                  (args.length > arg_index + 1))
                {
                  ec_names_[ec_names_count_] = args[arg_index + 1];
                  ++ec_names_count_;
                  arg_index += 2;
                }
              // Set a schedule service name.
              else if ((args[arg_index].equals ("-scheduleservicename")) &&
                  (args.length > arg_index + 1))
                {
                  ss_names_[ss_names_count_] = args[arg_index + 1];
                  ++ss_names_count_;
                  arg_index += 2;
                }
              else if (args[arg_index].equals ("-dualECdemo"))
                {
                  ss_names_[ss_names_count_] = "DUAL_SCHED_HI";
                  ss_names_[ss_names_count_ + 1] = "DUAL_SCHED_LO";
                  ec_names_[ec_names_count_] = "DUAL_EC_HI";
                  ec_names_[ec_names_count_ + 1] = "DUAL_EC_LO";
                  ++arg_index;
                  ec_names_count_ += 2;
                  ss_names_count_ += 2;
                }
              else if (args[arg_index].equals ("-dualECdemo1"))
                {
                  ss_names_[ss_names_count_] = "DUAL_SCHED_HI";
                  ec_names_[ec_names_count_] = "DUAL_EC_HI";
                  ++arg_index;
                  ++ec_names_count_;
                  ++ss_names_count_;
                }
              else if (args[arg_index].equals ("-dualECdemo2"))
                {
                  ss_names_[ss_names_count_] = "DUAL_SCHED_LO";
                  ec_names_[ec_names_count_] = "DUAL_EC_LO";
                  ++arg_index;
                  ++ec_names_count_;
                  ++ss_names_count_;
                }
              // Skip over anything else.
              else
                {
                  arg_index ++;
                }
            }

          // If names for event or scheduling services have not been provided
          // already, provide the default names used by the service executables.
          if (ec_names_count_ == 0)
            {
              ec_names_count_ = 1;
              ec_names_ = new String [1];
              ec_names_ [0] = "EventService";
            }
          if (ss_names_count_ == 0)
            {
              ss_names_count_ = 1;
              ss_names_ = new String [1];
              ss_names_ [0] = "ScheduleService";
            }
      }
      catch (org.omg.CORBA.ORBPackage.InvalidName e)
	{
	  System.err.println ("CosNaming.NamingContextPackage.InvalidName");
	  System.err.println (e);
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

	  // Create Consumers connected to the EventService
          int ss_number = 0;
          RtecScheduler.Scheduler scheduler_ = null;
          for (int ec_number = 0; ec_number < ec_names_count_; ++ec_number)
            {
              // Get a reference for the next Scheduling Service if there is one.
              // Invariant: we will always execute this code at least once.
              if (ss_number < ss_names_count_)
                {
                  CosNaming.NameComponent[] ss_name_components_ = new CosNaming.NameComponent[1];
                  ss_name_components_[0] = new CosNaming.NameComponent (ss_names_[ss_number],"");
                  org.omg.CORBA.Object scheduler_object_ = naming_context_.resolve (ss_name_components_);

                  if (scheduler_object_ == null)
                  {
                    throw new Object_is_null_exception(ss_names_[ss_number] +
                                                       " Object is null");
                  }

                  scheduler_ = RtecScheduler.SchedulerHelper.narrow (scheduler_object_);

	          if (scheduler_ == null)
                    {
                      throw new Object_is_null_exception(ss_names_[ss_number] +
                                                         " narrowed ref is null");
                    }

                  System.out.println ("Reference to " +
                                      ss_names_[ss_number] + " is ok.");

                  ++ss_number;
                }

              // Get a reference for the next Event Service

              CosNaming.NameComponent[] ec_name_components_ = new CosNaming.NameComponent[1];
              ec_name_components_[0] = new CosNaming.NameComponent (ec_names_[ec_number],"");
              org.omg.CORBA.Object event_channel_object_ = naming_context_.resolve (ec_name_components_);

              if (event_channel_object_ == null)
	        {
	          throw new Object_is_null_exception(ec_names_[ec_number] +
                                                     " Object is null");
	        }

       	      RtecEventChannelAdmin.EventChannel event_channel_ =
	        RtecEventChannelAdmin.EventChannelHelper.narrow (event_channel_object_);

	      if (event_channel_ == null)
	        {
	          throw new Object_is_null_exception(ec_names_[ec_number] +
                                                     " narrowed ref is null");
	        }

              System.out.println ("Reference to " +
                                  ec_names_[ec_number] + " is ok.");

              // Start the consumer
	      System.out.println ("Instantiating the Push Consumer for " +
                                  ec_names_[ec_number]  + ": demo_consumer_" +
                                  ec_number + ".");

	      PushConsumer pushConsumer_ = new PushConsumer (orb_, poa_,
                                                             dataHandler_,
                                                             use_queueing_);
	      System.out.println ("Initializing the Push Consumer for " +
                                  ec_names_[ec_number] + ": demo_consumer_" +
                                  ec_number + ".");

	      pushConsumer_.open_consumer (event_channel_, scheduler_,
                                           "demo_consumer_" + ec_number);
            }


  	  // Tell the CORBA environment that we are ready

	  System.out.println ("PushConsumer registered with POA");
	  poa_.the_POAManager ().activate ();
	  System.out.println ("POA Activated");
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
      catch (org.omg.PortableServer.POAManagerPackage.AdapterInactive e)
	{
	  System.err.println ("org.omg.PortableServer.POAManagerPackage.AdapterInactive");
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







