package tests.Concurrency.Condition;

public class JoinableThreadGroup extends ThreadGroup
{
  public JoinableThreadGroup(String name)
  {
    super(name);
  }

  public JoinableThreadGroup(ThreadGroup parent, String name)
  {
    super(parent, name);
  }

  public void join() throws InterruptedException
  {
    Thread list[] = new Thread[activeCount()];

    enumerate(list, true);

    for (int i = 0; i < list.length; i++)
      list[i].join();
  }
}
