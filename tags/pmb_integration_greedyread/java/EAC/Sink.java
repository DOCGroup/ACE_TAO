/**
 * Title:        Sink
 * Description:  An event sink for the Event Analysis Configurator
 */
package EAC;
import java.awt.*;

public class Sink extends Primitive {

  // triangle dimensions
  protected final int WIDTH = 20;
  protected final int HEIGHT = 35;

  // lower left and lower right corner points
  // inherited anchor attribute holds the top point
  protected Point p2,
                  p3;

  // inputs
  protected Connector input[] = new Connector[MAX_INPUTS];
  protected int input_count;

  // for help with dragging this Sink
  Point old_anchor;

  public Sink(EACPanel p) {
    super(p);
  } /* constructor */

  public void draw() throws BoundsException {
    int x[] = new int[3],
        y[] = new int[3];

    if (inBounds()) {
      Graphics g = canvas.getGraphics();

      g.drawLine(anchor.x,anchor.y,p2.x,p2.y);
      g.drawLine(p2.x,p2.y,p3.x,p3.y);
      g.drawLine(p3.x,p3.y,anchor.x,anchor.y);

      /* Make it black */
      x[0] = anchor.x;
      x[1] = p2.x;
      x[2] = p3.x;

      y[0] = anchor.y;
      y[1] = p2.y;
      y[2] = p3.y;

      g.fillPolygon(x,y,3);
    } else
      throw new BoundsException("ERROR: Attempted to place Sink partially out of bounds");
  } /* draw */

  public void selectedDraw() {
    old_anchor = anchor;
    super.selectedDraw();
  } /* selectedDraw */

  public void specialDraw() {
    int x[] = new int[3],
        y[] = new int[3];

    Graphics g = canvas.getGraphics();

    g.setXORMode(canvas.getBackground());
    g.drawLine(anchor.x,anchor.y,p2.x,p2.y);
    g.drawLine(p2.x,p2.y,p3.x,p3.y);
    g.drawLine(p3.x,p3.y,anchor.x,anchor.y);

    /* Make it black */
    x[0] = anchor.x;
    x[1] = p2.x;
    x[2] = p3.x;

    y[0] = anchor.y;
    y[1] = p2.y;
    y[2] = p3.y;

    g.fillPolygon(x,y,3);
  } /* specialDraw */

  public void specialUndraw() {
    int x[] = new int[3],
        y[] = new int[3];

    Graphics g = canvas.getGraphics();

    g.setColor(canvas.getBackground());
    g.setXORMode(canvas.getForeground());
    g.drawLine(anchor.x,anchor.y,p2.x,p2.y);
    g.drawLine(p2.x,p2.y,p3.x,p3.y);
    g.drawLine(p3.x,p3.y,anchor.x,anchor.y);

      /* Make it black */
      x[0] = anchor.x;
      x[1] = p2.x;
      x[2] = p3.x;

      y[0] = anchor.y;
      y[1] = p2.y;
      y[2] = p3.y;

      g.fillPolygon(x,y,3);
  } /* specialUndraw */

  public void reconnect() {
    int i;

    for (i = 0; i < input_count; i++)
      input[i].setEndpoint(new Point(input[i].upperRight().x + anchor.x - old_anchor.x,
                                     input[i].upperRight().y + anchor.y - old_anchor.y));
  } /* reconnect */

  public boolean contains(Point p) {
    if ((p.x >= p2.x) &&
        (p.x <= p3.x) &&
        (p.y >= anchor.y) &&
        (p.y <= anchor.y + HEIGHT))
      return true;
    else
      return false;
  } /* contains */

  public Point upperLeft() {
    return new Point(p2.x,anchor.y);
  } /* upperLeft */

  public Point lowerRight() {
    return p3;
  } /* lowerRight */

  public Point upperRight() {
    return new Point(p3.x,anchor.y);
  } /* upperRight */

  public Point lowerLeft() {
    return p2;
  } /* lowerLeft */

  public void setTop(Point p) {
    anchor = p;
    p2 = bottomLeft();
    p3 = bottomRight();
  } /* setTop */

  public Point getTop() {
    return anchor;
  } /* getTop */

  public boolean inBounds() {
    if (canvas.contains(bottomLeft()) &&
        canvas.contains(bottomRight()))
      return true;
    else
      return false;
  } /* inBounds */

  private Point bottomLeft() {
    return new Point(anchor.x - WIDTH, anchor.y + HEIGHT);
  } /* bottomLeft */

  private Point bottomRight() {
    return new Point(anchor.x + WIDTH, anchor.y + HEIGHT);
  } /* bottomRight */

  public void write(File f) throws java.io.IOException {
    f.writeInt(f.SINK);
    f.writeInt(anchor.x);
    f.writeInt(anchor.y);
  } /* write */

  public void read(File f) throws java.io.IOException {
    anchor = new Point();
    anchor.x = f.readInt();
    anchor.y = f.readInt();
    p2 = bottomLeft();
    p3 = bottomRight();
  } /* read */

  public int addInput(Connector c) throws ConnectionException {
    if (input_count == MAX_INPUTS)
      throw new ConnectionException("ERROR: Maximum inputs established for sink");
    else
      input[input_count++] = c;

    return input_count;
  } /* addInput */

  public int addOutput(Connector c) throws ConnectionException {
    throw new ConnectionException("ERROR: Attempted to add output to sink");
  } /* addOutput */

  public Connector getInput(int i) throws ConnectionException {
    if ((i < 0) || (i >= input_count))
      throw new ConnectionException("ERROR: Bad input index for sink");
    else
      return input[i];
  } /* getInput */

  public Connector getOutput(int i) throws ConnectionException {
    throw new ConnectionException("ERROR: Attempted to retrieve output from source");
  } /* getOutput */

  public void removeOutput(int i) throws ConnectionException {
    throw new ConnectionException("ERROR: Attempted to remove output from source");
  } /* removeOutput */

  public void removeInput(int i) throws ConnectionException {
    if ((i >= 0) && (i < input_count)) {
      input[i] = input[input_count-1];
      --input_count;
    } else
      throw new ConnectionException("ERROR: Bad input index for sink");
  } /* removeInput */

  public int getInputCount() throws ConnectionException {
    return input_count;
  } /* getInputCount */

  public int getOutputCount() throws ConnectionException {
    throw new ConnectionException("ERROR: Attempted to retrieve output count from sink");
  } /* getOutputCount */

  public void event(Source s) {
  } /* event */

  public void wakeup(long t) throws ConnectionException {
    throw new ConnectionException("ERROR: Attempted to wake up sink");
  } /* wakeup */
}