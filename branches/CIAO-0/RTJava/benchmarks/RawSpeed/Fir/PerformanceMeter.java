//$Id$

package Fir;

/**
 * This class provide a pretty easy and naive way of
 * measuring the performance of software, in term of
 * time taken to execute a given test. This class
 * represent the abstract base for class that effectively
 * preform some test. What this class provide is a template
 * method, that implements the logic related to the measuring
 * of the time ellapsed.
 */
public class PerformanceMeter {

    /**
     * This method runs the test by setting the start time
     * invoking the sublclass specific Run method.
     */
    public void Start() {
        this.mTimeTrace = new TimeTrace();
        this.mTimeTrace.Start();
        this.Run();
        this.mTimeTrace.Stop();
    }

    /**
     * Gives the time taken to run the test.
     */
    public double GetTraceTime() {
        return this.mTimeTrace.GetElapsedTime();
    }

    /**
     * Gets an estimate of the accuracy of the estimate given.
     */
    public double GetTraceTimeResolution() {
        return this.mTimeTrace.GetResolution();
    }

    /**
     * This method has to be filled by the sub-class
     * that provide a specific test strategy.
     */
    protected  void Run() { }

    private TimeTrace mTimeTrace;
}
