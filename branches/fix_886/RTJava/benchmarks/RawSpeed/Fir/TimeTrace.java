//$Id$

package Fir;

/**
 * This class provide a way of recording the time
 * elapsed between two events. This implementation
 * doesn't assume that you should match each start
 * of a trace with a stop. But it simply associate
 * the end of the trace with the last start point.
 */
public class TimeTrace {

    /**
     * Creates a TimerTrace object and sets the
     * start clock tick and the end clock ticks
     * at the clock tick when the object was
     * created.
     */
    public TimeTrace() {
        this.mStartTime = System.currentTimeMillis();
        this.mStopTime = this.mStartTime;
    }

    /**
     * Starts the time trace.
     */
    public void Start() {
        this.mStartTime = System.currentTimeMillis();
    }

    /**
     * Stops the time trace. There is no guard to
     * check whether the trace was started, to avoid
     * to spent time cheching condition.
     */
    public void Stop() {
        this.mStopTime = System.currentTimeMillis();
    }

    /**
     * Retrieve the time at which the trace started.
     */
    public long GetStartTime() {
        return this.mStartTime;
    }

    /**
     * Retrieve the time at which the trace ended.
     *
     */
    public long GetStopTime() {
        return this.mStopTime;
    }

    /**
     * Retrieves the time ellapsed between start and stop.
     *
     * return The time ellapsed in millis.
     */
    public long GetElapsedTime() {
        return this.mStopTime - this.mStartTime;
    }

    /**
     * Provide the resolution of the underlying timer used to
     * make the measurament.
     *
     * @return The resolution in millis.
     */
    public double GetResolution() {
        return 0.001;
    }

    private long mStartTime;
    private long mStopTime;
}
