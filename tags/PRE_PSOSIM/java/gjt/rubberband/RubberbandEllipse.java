package gjt.rubberband;

import java.awt.Component;
import java.awt.Graphics;
import java.awt.Rectangle;

/** 
 *  A Rubberband that does ellipses.
 *
 * @version 1.00, 12/27/95
 * @author  David Geary
 * @see     Rubberband
 * @see     gjt.test.RubberbandTest
 */
public class RubberbandEllipse extends Rubberband {
    private final int startAngle = 0;
    private final int endAngle   = 360;

    public RubberbandEllipse(Component component) {
        super(component);
    }
    public void drawLast(Graphics graphics) {
        Rectangle r = lastBounds();
        graphics.drawArc(r.x, r.y, 
            r.width, r.height, startAngle, endAngle);
    }
    public void drawNext(Graphics graphics) {
        Rectangle r = bounds();
        graphics.drawArc(r.x, r.y, 
            r.width, r.height, startAngle, endAngle);
    }
}
