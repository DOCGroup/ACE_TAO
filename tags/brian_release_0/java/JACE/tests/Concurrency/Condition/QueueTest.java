//File: QueueTest.java
//Seth Widoff, 8/8/96
//This class is a test method for the Producer and Consumer classes.
//The main method takes as arguments the number of producers, the
//number of consumers and the number of elements in the queue. It then
//spawn the specified threads and starts them.

package JACE.tests.Concurrency.Condition;

import JACE.ASX.TimeValue;

public class QueueTest
{
  public static void main(String[] args)
  {
    if (args.length < 5)
      {
	System.out.println("Usage: java QueueTest <# producers> <# consumers> " +
			   "<# elements> <#iterations> <#timeout secs> <#timeout nano secs>");
	System.exit(1);
      }
    
    int num_producers = Integer.parseInt(args[0]),
      num_consumers = Integer.parseInt(args[1]),
      num_elements = Integer.parseInt(args[2]),
      num_iterations = Integer.parseInt(args[3]),
      num_timeout_secs = Integer.parseInt(args[4]),
      num_timeout_nano_secs = Integer.parseInt(args[5]);
    
    if (num_elements < 1 
	|| num_consumers < 1 
	|| num_producers < 1)
      {
	System.out.println("All the parameters must be larger than zero.");
	System.exit(1);
      }
    
    SimpleMessageQueue queue = new SimpleMessageQueue(num_elements);
    Consumer[] consumers = new Consumer[num_consumers];
    Producer[] producers = new Producer[num_producers];
    JoinableThreadGroup thread_group = new JoinableThreadGroup("Producer Consumer");

    for (int i = 0; i < num_producers; i++)
      {
	producers[i] = new Producer("Number " + (i + 1), 
				    queue, 
				    num_iterations, 
				    new TimeValue (num_timeout_secs, num_timeout_nano_secs));
	new Thread(thread_group, producers[i]).start();
      }

    for (int i = 0; i < num_consumers; i++)
      {
	consumers[i] = new Consumer("Number " + (i + 1), queue, num_iterations, new TimeValue (num_timeout_secs, num_timeout_nano_secs));
	new Thread(thread_group, consumers[i]).start();
      }

    try
      {
	thread_group.join();
      }
    catch(InterruptedException excp)
      {
	System.out.println("QueueTest::main");
	System.out.println(excp);
      }
  }
}
