package imaging.filters;

public class Timer
{
  long start_time_;
  long stop_time_;
  
  public void start()
  {
    start_time_ = System.currentTimeMillis();
  }

  public void stop()
  {
    stop_time_ = System.currentTimeMillis();
  }

  public String toString()
  {
    long total = stop_time_ - start_time_;
    return "Total Time:" + total + " ms";
  }
}
