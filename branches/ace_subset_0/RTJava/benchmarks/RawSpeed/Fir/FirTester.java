//$Id$

package Fir;

public class FirTester {

    private String mSampleFile;
    private int mSampleNum;

    private String mCoeffFile;
    private int mCoeffNum;

    private boolean  mCachedIO = true;

    private int mCycle = 1;

    private void ParseArgs(String Args[]) {

        int reqArg = 4;
        int setArg[] = new int[reqArg];
        try {
            int i = 0;

            while (i < Args.length) {
                if (Args[i].equals("-SampleFile")) {
                    setArg[0] = 1;
                    this.mSampleFile = Args[++i];
                }
                else if (Args[i].equals("-CoeffFile")) {
                    setArg[1] = 1;
                    this.mCoeffFile = Args[++i];
                }
                else if (Args[i].equals("-SampleNum")) {
                    setArg[2] = 1;
                    this.mSampleNum = Integer.parseInt(Args[++i]);
                }
                else if (Args[i].equals("-CoeffNum")) {
                    setArg[3] = 1;
                    this.mCoeffNum = Integer.parseInt(Args[++i]);
                }

                else if (Args[i].equals("-NoCaching"))
                    this.mCachedIO = false;

                else if (Args[i].equals("-Cycle"))
                    this.mCycle = Integer.parseInt(Args[++i]);
                ++i;
            }
        }
        catch (java.lang.NumberFormatException nfe) {
            System.out.println("Invalid Number Format!");
            System.exit(-1);
        }

        int sum = 0;

        for (int i = 0; i < 4; i++)
            sum +=setArg[i];

        if ( sum != reqArg) {
            System.out.println("Not enough argumets!");
            System.out.println("USAGE: \nFirTester -SampleFile <filename> -SampleNum <sampleNum> -CoeffFile <filename> -CoeffNum <coeffnum>");
            System.exit(-1);
        }


    }

    public static void main(String Args[]) throws java.io.IOException {

        FirTester ft = new FirTester();
        ft.ParseArgs(Args);

        PerformanceMeter firTester;
        if (ft.mCachedIO)
            firTester = new CachedIOFirTester(ft.mSampleFile,
                                              ft.mSampleNum,
                                              ft.mCoeffFile,
                                              ft.mCoeffNum,
                                              ft.mCycle);
        else
            firTester = new IOFirTester(ft.mSampleFile,
                                        ft.mSampleNum,
                                        ft.mCoeffFile,
                                        ft.mCoeffNum);

        firTester.Start();
        System.out.println("Time Required by the computation: "
                           + firTester.GetTraceTime() +" msec.");
    }
}
