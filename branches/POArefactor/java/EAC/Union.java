/**
 *  Title:        Union
 *  Description:  A binary union operator class for the Event Analysis
 *                Configurator.  Acts as a sort of multiplexor, combining two
 *                event streams into one.
 */
package EAC;
import java.awt.*;

public class Union extends Operator {

  public Union(EACPanel p) {
    super(p);
  } /* constructor */

  public void draw() throws BoundsException {
    // the union symbol
    final char uSymbol = 'U';

    // subtle adjustments to position symbol within circle
    final int xTweak = 4;
    final int yTweak = -4;

    char symbol[] = new char[1];

    Graphics g = canvas.getGraphics();

    super.draw();
    symbol[0] = (char) uSymbol;
    g.drawChars(symbol,0,1,
                anchor.x + (int) (DIAMETER / 2) - xTweak,
                anchor.y + (int) (DIAMETER / 2) - yTweak);
  } /* draw */

  public void event(Source s) throws ConnectionException {
    int i;

    //System.out.println("Union event");

    // send an event to all outputs
    for (i = 0; i < output_count; i++)
      output[i].event(s);
  } /* event */

  public void write(File f) throws java.io.IOException {
    f.writeInt(f.UNION);
    f.writeInt(anchor.x);
    f.writeInt(anchor.y);
  } /* write */

  public void read(File f) throws java.io.IOException {
    anchor = new Point();
    anchor.x = f.readInt();
    anchor.y = f.readInt();
  } /* read */
}