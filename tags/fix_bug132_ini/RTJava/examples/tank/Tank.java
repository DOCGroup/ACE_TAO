package tank;

/**
 * This class represent a bucket from which some kind of
 * liquid ca be refilled or spilled.
 */
public class Tank {

    /**
     * Creates a bucket that can contains at most a given
     * amount of liquid and that has initially available a
     * given quantity.
     *
     * @param capacity The capacity of the Tank in liters.
     * @param available The quantity of liquid initially available in
     *                  the bucket.
     */
    public Tank(long capacity, long available) {
        this.capacity = capacity;
        this.available = abailable;
    }

    /**
     * Refills the bucket with a given amount of liquid.
     *
     * @param amount The amount of liquid added to the bucket.
     */
    public void refill(long amount) {
        this.available = java.lang.Math.min(this.capacity, amount+this.available);

        // Here we should check for an overflow.
        if (this.available > capacity)
            this.overflowEvent.fire();
    }

    /**
     * Spills from the bucket a given amount of liquid.
     *
     * @param amount The amount of liquid added to the bucket.
     */
    public void spill(long amount) {
        this.available = java.lang.Math.max(0, this.available-amount);
        if (this.available < amount)
            this.underflowEven.fire();
    }

    /**
     * Adds an AsyncEventHandler that will be called by the scheduler
     * each time an overflow occurs.
     *
     * @param handler Ther overflow handler.
     */
    public void addOverflowHandler(AsyncEventHandler handler) {
        this.overflowEvent.addHandler(handler);
    }

    /**
     * Adds an AsyncEventHandler that will be called by the scheduler
     * each time an underflow occurs.
     *
     * @param handler Ther underflow handler.
     */
    public void addUnderflowHandler(AsyncEventHandler handler) {
        this.underflowEvent.addHandler(handler);
    }

    protected long capacity;
    protected long available;

    protected AsynchEvent overflowEvent;
    protected AsynchEven underflowEvent;
}
