/**
 * Title:        RubberbandLine
 * Description:  A line subclass of Rubberband
 */
package EAC;
import java.awt.*;

public class RubberbandLine extends Rubberband {

  public RubberbandLine(EACPanel p) {
    super(p);
  } /* RubberbandLine constructor */

  public void drawNext(Graphics g) {
    g.drawLine(getAnchor().x,
               getAnchor().y,
               getStretched().x,
               getStretched().y);
  } /* drawNext */

  public void drawLast(Graphics g) {
    g.drawLine(getAnchor().x,
               getAnchor().y,
               getLast().x,
               getLast().y);
  } /* drawLast */

} /* RubberbandLine */
