package gjt;

/**
 * Client of a Stopwatch.  Stopwatches that have non-null 
 * clients, call their clients' tick() method every 50 
 * milliseconds.<p>
 *
 * @version 1.0, Apr 21 1996
 * @author  David Geary
 * @see     Stopwatch
 */
public interface StopwatchClient {
    public void tick();
}
