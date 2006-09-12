package mil.darpa.arms.mlrm.BandwidthBroker.common;

import java.util.*;
import java.io.*;

// Utility for collecting timing statistics about code.

// Simplest usage is:
// - TimeMon.startLogging(file) // once at beginning to tell where to log
// - on each segment of code to time:
//        TimeMon tm = new TimeMon(codeSegmentIdString)
//        ... <code segment> ...
//        tm.end();
//
// Stats will be written to log every N (total) iterations.
// Separate statistics are collected for each unique codeSegmentIdString.

public class TimeMon {
    private long start;
    private String id;
    private static Map map = Collections.synchronizedMap(new HashMap());
    private static Map vmap = Collections.synchronizedMap(new HashMap());
    private static PrintStream logStream;
    private static int dumpWhen = 10; // dump every this many times
    private static int warmUp = 10;
    private static boolean warmUpDone;
    private static int whenCnt;
    private static String[] controlIds;

    private long vstart;

    public TimeMon(String id_) {
	id = id_;
	start();
    }

    public void start() {
	start = System.currentTimeMillis();
	vstart = VTime.getProcessCPUTime();
    }
    
    public void end() {
	long period = System.currentTimeMillis() - start;
	long vperiod = VTime.getProcessCPUTime() - vstart;
	Stats stats = getStats(map, id);
	Stats vstats = getStats(vmap, id);
	if (!warmUpDone && (!isControlId(id) || stats.warmUpCnt++ < warmUp)) {
	    return;
	}
	warmUpDone = true;
	stats.update(period);
	vstats.update(vperiod);

	if (logStream != null && isControlId(id) && (++whenCnt%dumpWhen)== 0) {
	    dump(logStream);
	}
    }

    private static Stats getStats(Map map, String id) {
	Stats stats = (Stats)map.get(id);
	if (stats == null) {
	    stats = new Stats();
	    map.put(id, stats);
	}
	return stats;
    }


    public static long getN(String id) {return getStats(map,id).n;}
    public static long getMin(String id) {return getStats(map,id).min;}
    public static long getMax(String id) {return getStats(map,id).max;}
    public static double getMean(String id) {return getStats(map,id).getMean();}
    public static double getVariance(String id) {return getStats(map,id).getVariance();}
    public static ArrayList getPoints(String id) {return getStats(map,id).points;}

    public static long getVN(String id) {return getStats(vmap,id).n;}
    public static long getVMin(String id) {return getStats(vmap,id).min;}
    public static long getVMax(String id) {return getStats(vmap,id).max;}
    public static double getVMean(String id) {return getStats(vmap,id).getMean();}
    public static double getVVariance(String id) {return getStats(vmap,id).getVariance();}
    public static ArrayList getVPoints(String id) {return getStats(vmap,id).points;}

    public static void dump(PrintStream ps) {
	java.text.NumberFormat nf = java.text.NumberFormat.getInstance();
	nf.setMaximumFractionDigits(2);
	ps.println("-------------------------------------------");
	synchronized(map) {
	    for (Iterator it = map.keySet().iterator(); it.hasNext(); ) {
		String id = (String)it.next();
		ps.print("Id: " + id 
			   +", N: "+ getN(id)
			   +", Mean: "+ nf.format(getMean(id))
			   +", Variance: "+ nf.format(getVariance(id))
			   +", Stddev: " + nf.format(StrictMath.sqrt(getVariance(id)))
			   +", Min: "+ getMin(id)
			   +", Max: "+ getMax(id));

		ps.print(", Points: ");
		ArrayList points = getPoints(id);
		for (int i = 0; i < points.size(); ++i) {
		    ps.print(points.get(i) + ",");
		}
		ps.println();

		ps.print("VId: " + id 
			   +", N: "+ getVN(id)
			   +", Mean: "+ nf.format(getVMean(id))
			   +", Variance: "+ nf.format(getVVariance(id))
			   +", Stddev: " + nf.format(StrictMath.sqrt(getVVariance(id)))
			   +", Min: "+ getVMin(id)
			   +", Max: "+ getVMax(id));

		ps.print(", Points: ");
		ArrayList vpoints = getVPoints(id);
		for (int i = 0; i < vpoints.size(); ++i) {
		    ps.print(vpoints.get(i) + ",");
		}
		ps.println();
	    }
	}
    }

    public static void startLogging(File file) {
	if (logStream != null) {
	  System.err.println("Warning: TimeMon.startLogging() already called");
	  return;
	}
	try {
	    logStream = new PrintStream(new FileOutputStream(file, true));
	} catch (IOException x) {
	    x.printStackTrace();
	    return;
	}
	logStream.println("Process started: " +
			java.text.DateFormat.getInstance().format(new Date()));
    }

    public static void setDumpWhen(int when) {
	dumpWhen = when;
    }

    public static void setControlIds(String[] cids) {
	controlIds = cids;
    }

    private static boolean isControlId(String id) {
	if (controlIds == null) return true; // default to true if none set
	for (int i = 0; i < controlIds.length; ++i) {
	    if (controlIds[i].equals(id)) return true;
	}
	return false;
    }

    static class Stats {
	long total;
	long totalE2;
	int n;
	int warmUpCnt;
	long max;
	long min;
	ArrayList points = new ArrayList(50);

	double getMean() {return (double)total/n;}
	double getVariance() {return (double)totalE2/n - getMean()*getMean();}

	void update(long period) {
	    ++n;
	    total += period;
	    totalE2 += period*period;
	    if (period > max) { max = period;}
	    if (period < min || min == 0) {min = period;}
	    points.add(new Long(period));
	}	    
    }

    // test program
    public static void timeThis(String id) {
	//	System.out.println("start "+id);
	for (int i = 0; i < 10; ++i) {
	    TimeMon tm = new TimeMon(id);
	    try {Thread.sleep(100+i);} catch (Exception x) {}
	    tm.end();
	}
	//	System.out.println("end "+id);
    }

    public static void main(String args[]) {
	startLogging(new File("/tmp/timing.log"));
	
	for (int i = 0; i < 20; ++i) {
	    Thread t1 = new Thread(new Runnable() {
		    public void run() {timeThis("foo");}
		});
	    t1.start();
	    Thread t2 = new Thread(new Runnable() {
		    public void run() {timeThis("bar");}
		});
	    t2.start();
	    try {
		t1.join();
		t2.join();
	    } catch (Exception x) {
		x.printStackTrace();
	    }
	}
	TimeMon.dump(System.out);
    }
}

