//$Id$

package Fir;

/**
 * This class intented to contain a set of sample from
 * a given continuous variable. The sample are managed
 * as if the n-th item rapresent the item n sample in
 * the past. Each time an item is added the oldest sample
 * is discarded (i.e. the sample that is as old as the size
 * of the queue plus one).
 *
 */
public class SampleQueue {

    /**
     * Create a SampleQueue with a given size.
     *
     * @param sampleNum: The number of sample that the queue can store.
     */
    public SampleQueue(int sampleNum) {

        this.mSampleNum = sampleNum;
        this.mSample = new double[this.mSampleNum];
        this.mTail = 0;
        this.mCachedOffset = 0;
    }
    /**
     * Adds an item to the sample queue. The last
     * item added is treated as the most recent in time.
     *
     * @param item: The item to be added to the queue.
     */
    public void AddItem(double item) {

        this.mCachedOffset = this.mTail+this.mSampleNum;
        this.mSample[this.mTail] = item;
        this.mTail = (this.mTail+1)%this.mSampleNum;
    }

    /**
     * Adds an item to the sample queue. The last
     * item added is treated as the most recent in time.
     *
     * @param item: The item to be added to the queue.
     */
    public double GetItem(int index) {
        return this.mSample[index];
    }

    private int mSampleNum;

    private int mTail;

    private int mCachedOffset;

    private double mSample[];

}
