//$Id$

package Fir;

import java.io.FileInputStream;
import java.io.DataInputStream;

public class CachedIOFirTester extends PerformanceMeter {

    public CachedIOFirTester(String sampleFileName,
                             int sampleNum,
                             String coeffFileName,
                             int coeffNum,
                             int cycle) throws java.io.IOException {

        FileInputStream ifile = new FileInputStream(sampleFileName);
        DataInputStream dataStream = new DataInputStream(ifile);

        this.mSample = new double[sampleNum];
        for (int i = 0; i < sampleNum; i++)
            this.mSample[i] = dataStream.readDouble();

        ifile = new FileInputStream(coeffFileName);
        dataStream = new DataInputStream(ifile);
        this.mCoefficient = new double[coeffNum];

        for (int i = 0; i < this.mCoefficient.length; i++)
            this.mCoefficient[i] = dataStream.readDouble();

        this.mQueue = new SampleQueue(sampleNum);
        this.mFir = new Fir(this.mCoefficient, this.mQueue);
        this.mCycle = cycle;
    }

    protected void Run() {
        for (int c = 0; c < this.mCycle; c++) {
            for (int n = 0; n < this.mSample.length; n++) {
                this.mQueue.AddItem(this.mSample[n]);
                this.mSample[n] = this.mFir.Filter();
            }
        }
    }


    private double mCoefficient[];

    private double mSample[];

    private Fir mFir;

    private SampleQueue mQueue;

    private int mCycle;

}
