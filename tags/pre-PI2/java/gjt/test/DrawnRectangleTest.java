package gjt.test;

import java.awt.*;
import gjt.DrawnRectangle;
import gjt.EtchedRectangle;
import gjt.ThreeDRectangle;

/**
 * 9 DrawnRectangles (some of which are EtchedRectangles
 * and ThreeDRectangles) with varying characteristics such
 * as line widths and colors.<p>
 *
 * A mouse down (any mouse button) in any of the rectangles 
 * causes information about the rectangle to be printed to 
 * System.out. (Output will go to Java Console in Netscape).<p>
 *
 * @version 1.0, April 25, 1996
 * @author  David Geary
 * @see     gjt.test.UnitTest
 * @see     gjt.DrawnRectangle
 * @see     gjt.EtchedRectangle
 * @see     gjt.ThreeDRectangle
 */
public class DrawnRectangleTest extends UnitTest {
    public String title() { 
        return "Drawn Rectangle Test";  
    }
    public Panel centerPanel() { 
        return new DrawnRectangleTestPanel();
    }
}

class DrawnRectangleTestPanel extends Panel {
    private DrawnRectangle  drawnFilledOrange, 
                            drawnFilledBlue, drawnBlue;
    private EtchedRectangle etchedOut, 
                            etchedIn, etchedFilledCyan;
    private ThreeDRectangle thinRaised, 
                            thinInset, thickRaised, thickInset;

    public DrawnRectangleTestPanel() {
        drawnFilledOrange = 
            new DrawnRectangle (this, 10,  10,  100, 100);
        drawnFilledBlue = 
            new DrawnRectangle (this, 135, 135, 100, 100);
        drawnBlue = 
            new DrawnRectangle (this, 505, 135, 100, 100);
        etchedFilledCyan = 
            new EtchedRectangle(this, 10,  135, 100, 100);

        etchedIn = new EtchedRectangle(this, 385, 10, 100, 100);
        etchedOut= new EtchedRectangle(this, 505, 10, 100, 100);

        thinRaised = 
            new ThreeDRectangle(this, 135, 10,  100, 100);
        thinInset  = 
            new ThreeDRectangle(this, 260, 10,  100, 100);
        thickRaised = 
            new ThreeDRectangle(this, 385, 135, 100, 100);
        thickInset = 
            new ThreeDRectangle(this, 260, 135, 100, 100);

        drawnFilledOrange.setLineColor(Color.black);

        drawnFilledBlue.setLineColor(Color.yellow);
        drawnFilledBlue.setThickness(3);

        drawnBlue.setLineColor(Color.blue);
        drawnBlue.setThickness(5);

        thickRaised.setThickness(5);
        thickInset.setThickness (5);
    }
    public Dimension preferredSize() {
        return new Dimension(610, 270);
    }
    public void paint(Graphics g) {
        drawnFilledOrange.paint();
        drawnFilledOrange.fill (Color.orange);

        drawnFilledBlue.paint  ();
        drawnFilledBlue.fill   (Color.blue);

        drawnBlue.paint         ();

        etchedIn.paintEtchedIn  ();
        etchedOut.paintEtchedOut();

        etchedFilledCyan.paintEtchedIn();
        etchedFilledCyan.fill(Color.cyan);

        thinRaised.paintRaised  ();
        thinInset.paintInset    ();

        thickRaised.paintRaised ();

        thickInset.paintInset  ();
        thickInset.fill      (Color.red);
    }
    public boolean mouseDown(Event event, int x, int y) {
        if(drawnFilledOrange.inside(x,y)) 
            show(drawnFilledOrange);

        if(drawnFilledBlue.inside(x,y))  show(drawnFilledBlue);
        if(drawnBlue.inside(x,y))        show(drawnBlue);
        if(etchedIn.inside(x,y))         show(etchedIn);
        if(etchedOut.inside(x,y))        show(etchedOut);
        if(etchedFilledCyan.inside(x,y)) show(etchedFilledCyan);
        if(thinRaised.inside(x,y))       show(thinRaised);
        if(thickRaised.inside(x,y))      show(thickRaised);
        if(thinInset.inside(x,y))        show(thinInset);
        if(thickInset.inside(x,y))       show(thickInset);

        return true;
    }
    private void show(DrawnRectangle drawnRectangle) {
        System.out.println(drawnRectangle);
    }
}
