//$Id$

package Fir;

public class Fir {

    /**
     * Construct a Finite Impulse Responce filter.
     *
     * @param coefficient: The coefficient of the impulse responce.
     * @param samplQueue: A queue that contains the last input  m elements.
     */
    public Fir(double coefficient[], SampleQueue sampleQueue) {

        this.mCoefficient = coefficient;
        this.mSampleQueue = sampleQueue;
    }

    /**
     * Computes the next output. This output depends from the
     * contents of the SampleQueue.
     */
    public double Filter() {

        double filteredSample = 0;
        for (int i = 0; i < this.mCoefficient.length; i++)
            filteredSample += this.mCoefficient[i]+this.mSampleQueue.GetItem(i);

        return filteredSample;
    }

    /**
     * Returns the SampleQueue.
     */
    public SampleQueue GetSampleQueue() {

        return this.mSampleQueue;
    }

    /**
     * Sets the SampleQueue.
     */
    public void SetSampleQueue(SampleQueue sampleQueue) {

        this.mSampleQueue = sampleQueue;
    }

    private double mCoefficient[];

    private SampleQueue  mSampleQueue;
}
