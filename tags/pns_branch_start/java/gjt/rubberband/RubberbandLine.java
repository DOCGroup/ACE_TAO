package gjt.rubberband;

import java.awt.Component;
import java.awt.Graphics;

/** 
 * A Rubberband that does lines.
 *
 * @version 1.0, 12/27/95
 * @author  David Geary
 * @see     Rubberband
 * @see     gjt.test.RubberbandTest
 */
public class RubberbandLine extends Rubberband {
    public RubberbandLine(Component component) {
        super(component);
    }
    public void drawLast(Graphics graphics) {
        graphics.drawLine(anchor.x, anchor.y, last.x, last.y);
    }
    public void drawNext(Graphics graphics) {
        graphics.drawLine(anchor.x, anchor.y, 
                          stretched.x, stretched.y);
    }
}
