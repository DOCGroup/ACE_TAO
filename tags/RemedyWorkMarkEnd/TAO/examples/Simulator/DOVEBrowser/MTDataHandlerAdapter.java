// $Id$

// This is an adapter class for a data handler to be used in a separate
// thread.  The adapter provides a push method that places an event
// set into its synchronized internal MTQueue.  It runs a separate thread
// which blocks until there is an event in the queue, then dequeues the
// event and then unpacks it and updates the underlying data handler.

public class  MTDataHandlerAdapter extends Thread
{
  // Both the queue and the underlying data handler are private
  private MTQueue queue_ = null;
  private DataHandler dataHandler_ = null;
  private boolean use_queueing_ = false;

  // Constructor.
  MTDataHandlerAdapter (DataHandler dh, boolean use_queueing)
    {
      dataHandler_ = dh;
      use_queueing_ = use_queueing;
      if (use_queueing_)
        {
          queue_ = new MTQueue ();
        }
    }

  // Enqueue an event set for the handler thread.
  public void push (RtecEventComm.Event[] events)
    {
      //      System.out.println ("in MTDataHandlerAdapter.push");

      if (use_queueing_)
        {
	  //          System.out.println ("MTDataHandlerAdapter.push queueing events");
          queue_.enqueue_tail (events);
        }
      else
        {
          for (int i = 0; i < events.length; ++i)
            {
              if(events[i].header.type ==
                 PushConsumer.ACE_ES_EVENT_NOTIFICATION)
                {
		  //                  System.out.println ("MTDataHandlerAdapter.push updating data handler");
                  dataHandler_.update (events[i]);
                }
            }
        }
    }

  // Process enqueued event sets in a separate thread.
  public void run ()
    {
      // Loop forever, handling events.
      for (;;)
        {
          try
            {
              // Pull an event set from the head of the queue
             RtecEventComm.Event[] events =
                (RtecEventComm.Event[]) queue_.dequeue_head ();

              for (int i = 0; i < events.length; ++i)
                {
                  if(events[i].header.type ==
                     PushConsumer.ACE_ES_EVENT_NOTIFICATION)
                    {
                      dataHandler_.update (events[i]);
                    }
                }
            }
          catch(org.omg.CORBA.SystemException e)
            {
              System.err.println(e);
            }
        }
    }
}


