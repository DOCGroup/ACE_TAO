// This class encapsulates a Timer  mechanism 
// Can be used for Profiling of parts of code and gathering statistics 

package NexusII ; 

public class Timer { 

public Timer()
{
  start_ = 0 ; 		
}

public void start()

{
  start_ = System.currentTimeMillis(); 
}

public  long elapsed_time()
{
  return System.currentTimeMillis() - start_ ; 
}

public void stop()
{
  start_ = 0 ; 
}

private long start_ = 0 ; 

}
