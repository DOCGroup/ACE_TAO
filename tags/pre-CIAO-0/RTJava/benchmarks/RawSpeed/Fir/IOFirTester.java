//$Id$

package Fir;

import java.io.FileInputStream;
import java.io.DataInputStream;

public class IOFirTester extends PerformanceMeter {

    private DataInputStream mDataStream;

    private double mCoefficient[];
    private Fir mFir;
    private SampleQueue mQueue;

    private int mSampleNum;

    public IOFirTester(String sampleFileName,
                       int sampleNum,
                       String coeffFileName,
                       int coeffNum) throws java.io.IOException {

        FileInputStream ifile = new FileInputStream(coeffFileName);
        this.mDataStream = new DataInputStream(ifile);
        this.mCoefficient = new double[coeffNum];

        for (int i = 0; i < this.mCoefficient.length; i++)
            this.mCoefficient[i] = this.mDataStream.readDouble();

        ifile = new FileInputStream(sampleFileName);
        this.mDataStream = new DataInputStream(ifile);

        this.mQueue = new SampleQueue(sampleNum);
        this.mFir = new Fir(this.mCoefficient, this.mQueue);
        this.mSampleNum = sampleNum;
    }

    protected void Run() {
        try {
            for (int n = 0; n < this.mSampleNum; n++) {
                this.mQueue.AddItem(this.mDataStream.readDouble());
                this.mFir.Filter();
            }
        }
        catch (java.io.IOException e) {
            e.printStackTrace();
            System.exit(-1);
        }
    }
}
