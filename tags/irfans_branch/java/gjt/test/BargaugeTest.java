package gjt.test;

import java.awt.*;
import java.applet.*;
import gjt.Bargauge;

/**
 * An array of either horizontal or vertical animated bargauges.
 * The orientation of the bargauges is controlled by a parameter
 * passed into the applet.<p>
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
public class BargaugeTest extends UnitTest {
    private Bargauge[] gauges = new Bargauge[10];
    private Thread     animatorThread;
    private boolean    running;

    public String title() { 
        return "Bargauge Test"; 
    }
    public Panel  centerPanel() { 
        return new BargaugeTestPanel(
                    gauges, getParameter("orientation"));
    }
    public boolean mouseDown(Event event, int x, int y) {
        if(running == true) { 
            animatorThread.suspend(); 
            running = false; 
        }
        else { 
            animatorThread.resume (); 
            running = true;  
        }
        return true;
    }
    public void start() {
        super.start();
        animatorThread = new BargaugeAnimator(gauges);
        animatorThread.start();
        running = true;
    }
    public void stop() {
        super.stop();
        animatorThread.suspend();
        running = false;
    }
}

class BargaugeTestPanel extends Panel {
    public BargaugeTestPanel(Bargauge[] gauges, String orient) {
        Panel bargaugePanel = new Panel();

        setLayout(new BorderLayout());
        add("North",  
            new Label("Mouse Down Starts/Stops",Label.CENTER));
        add("Center", bargaugePanel);

        bargaugePanel.add(new BargaugeGridPanel(gauges,orient));
    }
}

class BargaugeGridPanel extends Panel {
    private Dimension  preferredSize = new Dimension(200, 250);

    public BargaugeGridPanel(Bargauge[] gauges, String orient) {
        Bargauge nextGauge;
        Color    color = Color.gray;

        if("horizontal".equals(orient))
            setLayout(new GridLayout(gauges.length,0,5,5));
        else
            setLayout(new GridLayout(0,gauges.length,5,5));

        for(int i=0; i < gauges.length; ++i) {
            switch(i) {
                case 1:  color = Color.darkGray;    break;
                case 2:  color = Color.blue;        break;
                case 3:  color = Color.magenta;     break;
                case 4:  color = Color.yellow;      break;
                case 5:  color = Color.green;       break;
                case 6:  color = Color.cyan;        break;
                case 7:  color = Color.orange;      break;
                case 8:  color = Color.pink;        break;
                case 9:  color = Color.red;         break;
                case 10: color = Color.yellow;      break;
            }
            nextGauge = new Bargauge(color);
            gauges[i] = nextGauge;
            add(nextGauge);
        }
    }
    public Dimension preferredSize() { return preferredSize; }
    public Dimension minimumSize  () { return preferredSize; }
}

class BargaugeAnimator extends Thread {
    private Bargauge[] gauges;
    private boolean    firstAnimation = true;

    public BargaugeAnimator(Bargauge[] gauges) {
        this.gauges = gauges;
    }
    public void run() {
        int count = gauges.length;

        while(true) {
            try { Thread.currentThread().sleep(500,0); }
            catch(InterruptedException e) { }
            for(int i=0; i < count; ++i) {
                gauges[i].setFillPercent(Math.random() * 100);
                gauges[i].fill();

                if(firstAnimation)
                    System.out.println(gauges[i].toString());
            }
            firstAnimation = false;
        }
    }
}
