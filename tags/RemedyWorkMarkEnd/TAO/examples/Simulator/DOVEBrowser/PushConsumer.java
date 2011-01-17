// $Id$
//
// ============================================================================
//
//
// = FILENAME
//    PushConsumer.java
//
// = AUTHOR
//    Michael Kircher (mk1@cs.wustl.edu)
//
// = DESCRIPTION
//    This is a Push Consumer which takes the data field of the
//    event and updates with it a Data Handler.
//
//
// ============================================================================



// The Consumer has to implement the Skeleton Consumer

public class PushConsumer extends RtecEventComm.PushConsumerPOA
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
  private org.omg.PortableServer.POA poa_;
  private MTDataHandlerAdapter dataHandlerAdapter_;
  private int rt_info_;
  private RtecEventChannelAdmin.EventChannel channel_admin_;
  private RtecEventChannelAdmin.ConsumerAdmin consumer_admin_;
  private RtecEventChannelAdmin.ProxyPushSupplier suppliers_;

  public PushConsumer (org.omg.CORBA.ORB orb,
                       org.omg.PortableServer.POA poa,
                       DataHandler dataHandler,
                       boolean use_queueing)
    {
      orb_ = orb;
      poa_ = poa;
      dataHandlerAdapter_ =
        new MTDataHandlerAdapter (dataHandler, use_queueing);
      if (use_queueing)
        {
          dataHandlerAdapter_.start ();
        }
    }


  public void push (RtecEventComm.Event[] events)
    {
      if (total_received_ < 5)
        System.out.println ("Demo Consumer: Received an event set! ->Number: "
                            + total_received_);
      else if (total_received_ == 5)
        System.out.println ("Demo Consumer: Everything is fine. " +
                            "Going to be mute.");

      if (events.length == 0)
        {
          System.err.println ("No events");
        }
      else
        {
          total_received_++;
          dataHandlerAdapter_.push (events);
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

        rt_info_ = scheduler_.create (name);

        scheduler_.set (rt_info_,
                        RtecScheduler.Criticality_t.VERY_LOW_CRITICALITY,
                        0L,
                        0L,
                        0L,
                        2500000,  // period
                        RtecScheduler.Importance_t.VERY_LOW_IMPORTANCE,
                        0L,
                        1,
                        RtecScheduler.Info_Type_t.OPERATION);


        // Register for Notification and Shutdown events

        byte payload[] = new byte[1];
        payload[0] = 0;
        RtecEventComm.Event notification_event_ = new RtecEventComm.Event ();
        notification_event_.header =
          new RtecEventComm.EventHeader (ACE_ES_EVENT_NOTIFICATION,
                                         0, 1, 0, 0, 0);

        notification_event_.data =
          new RtecEventData (0, payload, orb_.create_any());

        RtecEventChannelAdmin.Dependency dependencies_[] = new RtecEventChannelAdmin.Dependency[1];
        dependencies_[0] = new RtecEventChannelAdmin.Dependency (notification_event_, rt_info_);


        // @@ Carlos please help me to set the right boolean value
        RtecEventChannelAdmin.ConsumerQOS qos = new RtecEventChannelAdmin.ConsumerQOS (dependencies_, false);


        // The channel administrator is the event channel we got from the invocation
        // of this routine

        channel_admin_ = event_channel_;

        // Connect as a consumer

        consumer_admin_ = channel_admin_.for_consumers ();

        // Obtain a reference to the proxy push supplier

        suppliers_ = consumer_admin_.obtain_push_supplier ();

        org.omg.CORBA.Object objref = poa_.servant_to_reference (this);
        RtecEventComm.PushConsumer consumer_ref =
          RtecEventComm.PushConsumerHelper.narrow (objref);
        suppliers_.connect_push_consumer (consumer_ref, qos);

        System.out.println ("Registered the consumer successfully.");


  }
  catch (RtecEventChannelAdmin.TypeError e)
        {
          System.err.println ("Demo_Consumer.open_consumer: RtecEventChannelAdmin.TypeError");
          System.err.println (e);
        }
  catch (RtecEventChannelAdmin.AlreadyConnected e)
        {
          System.err.println ("Demo_Consumer.open_consumer: RtecEventChannelAdmin.AlreadyConnected");
          System.err.println (e);
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
  catch (RtecScheduler.INTERNAL e)
        {
          System.err.println ("Demo_Consumer.open_consumer: internal scheduler error");
          System.err.println (e);
        }
  catch (RtecScheduler.SYNCHRONIZATION_FAILURE e)
        {
          System.err.println ("Demo_Consumer.open_consumer: scheduler synchronization failure");
          System.err.println (e);
        }
  catch (org.omg.PortableServer.POAPackage.ServantNotActive e)
        {
          System.err.println ("Demo_Consumer.open_consumer: org.omg.PortableServer.POAPackage.ServantNotActive");
          System.err.println (e);
        }
  catch (org.omg.PortableServer.POAPackage.WrongPolicy e)
        {
          System.err.println ("Demo_Consumer.open_consumer: org.omg.PortableServer.POAPackage.WrongPolicy");
          System.err.println (e);
        }
  catch(org.omg.CORBA.SystemException e)
        {
          System.err.println(e);
        }
    }
}
