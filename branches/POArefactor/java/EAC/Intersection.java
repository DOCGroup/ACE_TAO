/**
 *  Title:        Intersection
 *  Description:  A binary intersection operator class for the Event Analysis
 *                Configurator.  Acts as a sort of filter, combining two event
 *                streams into one.
 */
package EAC;
import java.awt.*;

public class Intersection extends Operator {

  // Where did the last event come from?
  protected Primitive last_event_source;

  public Intersection(EACPanel p) {
    super(p);
    last_event_source = null;
  } /* constructor */

  public void draw() throws BoundsException {
    // the intersection symbol
    final char iSymbol = '^';

    // subtle adjustments to position symbol within circle
    final int xTweak = 2;
    final int yTweak = -5;

    char symbol[] = new char[1];
    Graphics g = canvas.getGraphics();

    super.draw();
    symbol[0] = iSymbol;
    g.drawChars(symbol,0,1,
                anchor.x + (int) (DIAMETER / 2) - xTweak,
                anchor.y + (int) (DIAMETER / 2) - yTweak);
  } /* draw */

  public void event(Source s) throws ConnectionException {
    //System.out.println("Intersection event");
    if (last_event_source == null)
      last_event_source = s;
    else if (last_event_source != s) { // we've now gotten events from two distinct sources
      int i;

      last_event_source = null;
      for (i = 0; i < output_count; i++)
        output[i].event(s);
    } // else no-op, because this event is from the same source as the last event
  } /* event */

  public void write(File f) throws java.io.IOException {
    f.writeInt(f.INTERSECTION);
    f.writeInt(anchor.x);
    f.writeInt(anchor.y);
  } /* write */

  public void read(File f) throws java.io.IOException {
    anchor = new Point();
    anchor.x = f.readInt();
    anchor.y = f.readInt();
  } /* read */
}