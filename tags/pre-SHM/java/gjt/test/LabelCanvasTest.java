package gjt.test;
import java.applet.Applet;
import java.awt.Event;
import java.awt.Panel;
import java.awt.Insets;
import java.awt.Graphics;
import gjt.LabelCanvas;
import gjt.SelectionEvent;
import gjt.Util;

/**
 * Four LabelCanvases, each with different insets.  The leftmost
 * LabelCanvas has standard insets (2 all around), while the
 * other three were constructed as follows:
 * <pre>
 *      insetFive.setInsets   (new Insets(5,5,5,5));
 *      insetTen.setInsets    (new Insets(10,10,10,10));
 *      insetFifteen.setInsets(new Insets(15,15,15,15));
 * </pre><p>
 *
 * LabelCanvases generate SelectionEvents, that we watch
 * for in our handleEvent() method, and print out.<p>
 *
 * @version 1.0, April 25, 1996
 * @author  David Geary
 * @see     gjt.test.UnitTest
 * @see     gjt.LabelCanvas
 */
public class LabelCanvasTest extends UnitTest {
    public String title() { return "LabelCanvas Test"; }
    public Panel centerPanel() { 
        return new LabelCanvasTestPanel(this); 
    }
}

class LabelCanvasTestPanel extends Panel {
    Applet applet;
    public LabelCanvasTestPanel(Applet applet) {
        this.applet = applet;
        LabelCanvas standard = 
            new LabelCanvas("Standard Insets");
        LabelCanvas insetFive = 
            new LabelCanvas("Insets = Five");
        LabelCanvas insetTen = 
            new LabelCanvas("Insets = Ten");
        LabelCanvas insetFifteen = 
            new LabelCanvas("Insets = Fifteen");

        insetFive.setInsets   (new Insets(5,5,5,5));
        insetTen.setInsets    (new Insets(10,10,10,10));
        insetFifteen.setInsets(new Insets(15,15,15,15));
        
        add(standard);
        add(insetFive);
        add(insetTen);
        add(insetFifteen);
    }
    public boolean handleEvent(Event event) {
        if(event instanceof SelectionEvent) {
            SelectionEvent sevent = (SelectionEvent)event;
            LabelCanvas    canvas = (LabelCanvas)event.target;

            if(sevent.isSelected())
                System.out.println("LabelCanvas "    + 
                                   canvas.getLabel() + 
                                   " selected");
            else
                System.out.println("LabelCanvas "    + 
                                   canvas.getLabel() +
                                   " deselected");
            return true;
        }
        return super.handleEvent(event);
    }
}
