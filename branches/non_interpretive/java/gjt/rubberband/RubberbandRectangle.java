package gjt.rubberband;

import java.awt.Component;
import java.awt.Graphics;
import java.awt.Rectangle;

/** 
 * A Rubberband that does rectangles.
 *
 * @version 1.00, 12/27/95
 * @author  David Geary
 * @see     Rubberband
 * @see     gjt.test.RubberbandTest
 */
public class RubberbandRectangle extends Rubberband {
    public RubberbandRectangle(Component component) {
        super(component);
    }
    public void drawLast(Graphics graphics) {
        Rectangle rect = lastBounds();
        graphics.drawRect(rect.x, rect.y, 
                          rect.width, rect.height);
    }
    public void drawNext(Graphics graphics) {
        Rectangle rect = bounds();
        graphics.drawRect(rect.x, rect.y, 
                          rect.width, rect.height);
    }
}
