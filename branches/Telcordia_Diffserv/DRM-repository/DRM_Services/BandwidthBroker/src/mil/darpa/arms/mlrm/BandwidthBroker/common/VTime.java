package mil.darpa.arms.mlrm.BandwidthBroker.common;

public class VTime {
    public static void main(String args[]) {
	System.out.println("vtime: " + getProcessCPUTime() +", rtime: " + System.currentTimeMillis());
	try {Thread.sleep(3000);} catch (Exception x) {}
	System.out.println("vtime: " + getProcessCPUTime() +", rtime: " + System.currentTimeMillis());
    }

    private static final String VTLIB = "vt";
    private static boolean initted;

    static
    {
        try
        {
            System.loadLibrary (VTLIB);
	    initted = true;
        }
        catch (UnsatisfiedLinkError e)
        {
            System.err.println ("native lib '" + VTLIB +
				"' not found in 'java.library.path': "
            + System.getProperty ("java.library.path"));
        }
    }

    public static long getProcessCPUTime() {
	if (!initted) return 0;
	return _getProcessCPUTime();
    }

    public static native long _getProcessCPUTime();
}
