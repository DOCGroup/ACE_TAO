package gjt;

import java.awt.*;

/**
 * A Thread that acts as a stopwatch.<p>
 *
 * Stopwatch starts running when it is constructed, and may be 
 * reset by the reset() method.  getHour(), getMinute(), 
 * getSecond(), and getMillisecond() are used to get the 
 * elapsed time since construction, or since the last reset.<p>
 *
 * toString() returns the elapsed time in the form of 
 * HH:MM:SS:mm, where HH == hours, MM == minutes, SS == seconds 
 * and mm == milliseconds.<p>
 *
 * Each Stopwatch may have a StopwatchClient associated with it.
 * If the StopwatchClient is non-null, the StopwatchClients' 
 * tick() method is invoked every 50 milliseconds.<p>
 *
 * @version 1.0, Apr 21 1996
 * @author  David Geary
 * @see     StopwatchClient
 * @see     gjt.animation.Sequence
 * @see     gjt.animation.Sprite
 */
public class Stopwatch extends Thread {
    private StopwatchClient client;
    private long            start, now, elapsed;
    private long            hour, minute, second, millisecond;

    public Stopwatch() {
        this(null);
    }
    public Stopwatch(StopwatchClient client) {
        start       = System.currentTimeMillis();
        this.client = client;
    }
    public void update() {
        now     = System.currentTimeMillis();
        elapsed = now - start;
        hour    = minute = second = millisecond = 0;

        second = elapsed / 1000;
        millisecond  = elapsed % 1000;
        millisecond  = (millisecond == 0) ? 0 : millisecond/10;

        if(second > 59) {
            minute = second / 60;
            second = second - (minute*60);
        }
        if(minute > 59) {
            hour   = minute / 60;
            minute = minute - (hour*60);
        }
    }
    public String toString() {
        update();
        return new String(stringValueOf(hour)   + ":" + 
                          stringValueOf(minute) + ":" + 
                          stringValueOf(second) + ":" +
                          stringValueOf(millisecond));
    }
    public long getHour        () { return hour;        }
    public long getMinute      () { return minute;      }
    public long getSecond      () { return second;      }
    public long getMillisecond () { return millisecond; }

    public long elapsedTime() { 
        update();
        return elapsed;
    }
    public void reset() {
        start = System.currentTimeMillis();
    }
    public void run() {
        while(true) {
            try {
                Thread.currentThread().sleep(50, 0);
                update();
                if(client != null)
                    client.tick();
            }
            catch(InterruptedException e) {
                Assert.notFalse(false);
            }
        }
    }
    private String stringValueOf(long l) {
        if(l < 10) return "0" + String.valueOf(l);
        else       return String.valueOf(l);

    }
}
