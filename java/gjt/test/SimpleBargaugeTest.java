package gjt.test;

import java.awt.*;
import gjt.Bargauge;

/**
 * A lone Barguage which animates.  This unit test is meant to 
 * illustrate that a Bargauge can cope with having its 
 * orientation chanaged from horizontal to vertical or 
 * vice-versa.  This test is best run in appletviewer, so that
 * the window may be resized such that the Bargauge changes its 
 * orientation.<p>
 *
 * <em>
 * Warning:  An AWT bug causes this test to be a gluttenous 
 * consumer of resources (especially under Win95). A mouse down 
 * will halt the animation thread along with its consumption of 
 * resources.<p>
 * </em>
 *
 * @version 1.0, April 25, 1996
 * @author  David Geary
 * @see     gjt.test.UnitTest
 * @see     gjt.Bargauge
 */
public class SimpleBargaugeTest extends UnitTest {
    public String title() { 
        return "Simple Bargauge Test"; 
    }
    public Panel centerPanel() { 
        return new SimpleBargaugeTestPanel(); 
    }
}

class SimpleBargaugeTestPanel extends Panel implements Runnable {
    private Bargauge gauge = new Bargauge(Color.blue);
    private boolean  running = true;
    private Thread   t;

    public SimpleBargaugeTestPanel() {
        setLayout(new BorderLayout());
        add("Center", gauge);

        t = new Thread(this);
        t.start();
    }
    public void run() {
        while(true) {
            try { Thread.currentThread().sleep(500,0); }
            catch(InterruptedException e) { }

            gauge.setFillPercent(Math.random() * 100);
            gauge.fill();
        }
    }
    public boolean mouseDown(Event event, int x, int y) {
        if(running)  { t.suspend(); running = false; }
        else         { t.resume (); running = true;  }
        return true;
    }
}
