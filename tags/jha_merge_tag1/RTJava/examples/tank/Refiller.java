package tank;

/**
 * This class represent an activity that refills periodically
 * the bucket. In this application it is assumed that this activity
 * should be carried periodically.
 */
public class Refiller implements java.lang.Runnable {

    /**
     * Constructs a refiller that refills the tank every
     * given amount of time.
     *
     * @param period The period of time at which the refill should
     *               occur.
     */
    public Refiller(int period) {
        this.period = period;
    }

    public Refiller(int period) {
        this.period = period;
    }

    protected long period;
}
