/**
 * Title:        Connector
 * Description:  Connects two primitives in the Event Analysis Configurator
 */
package EAC;
import java.awt.*;

public class Connector extends Primitive {

  // for drawing
  protected RubberbandLine rbLine;

  // Primitives connected by connector
  Primitive startP,
            endP;

  // the second point of the connector
  // (the anchor point is inherited from Primitive)
  protected Point endpoint;

  public Connector(EACPanel p) {
    super(p);
    rbLine = new RubberbandLine(p);
  }

  // inappropriate inherited methods: should not be used
  public void setTop(Point p) {}
  public Point getTop() { return null; }

  public void setEndpoint(Point p) {
    endpoint = p;
  } /* setEndpoint */

  public Point getEndpoint() {
    return endpoint;
  } /* getEndpoint */

  // Anchor the rubberband line AND the connector itself
  public void anchor(Point p) {
    rbLine.anchor(p);
    anchor = p;
  } /* anchor */

  // Stretch the rubberband line
  public void stretch(Point p) {
    rbLine.stretch(p);
  } /* stretch */

  // End the rubberband line AND the connector itself
  public void end(Point p) {
    rbLine.end(p);
    endpoint = p;
  } /* end */

  public void draw() throws BoundsException {
    if (inBounds()) {
      Graphics g = canvas.getGraphics();

      g.drawLine(anchor.x,anchor.y,endpoint.x,endpoint.y);
      drawArrowhead(g);
    } else
      throw new BoundsException("ERROR: Attempted to place Connector endpoint out of bounds");
  } /* draw */

  private void drawArrowhead(Graphics g) {
    final int arrowHeadWidth = 10;
    Point P1 = anchor;
    Point P2 = endpoint;

    // Draw Arrowhead (using java.lang.Math and floating point)
    // source provided by Erik in newsgroup posting
    double dx = P2.x - P1.x;
    double dy = P2.y - P1.y;
    final double ra = java.lang.Math.sqrt(dx*dx + dy*dy);
    final double ri = (double) arrowHeadWidth;
    dx /= ra;
    dy /= ra;
    Point p2 = new Point((int)Math.round(P2.x - dx*ri),
    (int)Math.round(P2.y - dy*ri));
    int[] x = new int[3];
    int[] y = new int[3];
    double r = 0.4 * ri;
    x[0] = (int)Math.round(p2.x + dy * r);
    y[0] = (int)Math.round(p2.y - dx * r);
    x[1] = P2.x;
    y[1] = P2.y;
    x[2] = (int)Math.round(p2.x - dy * r);
    y[2] = (int)Math.round(p2.y + dx * r);
    g.fillPolygon(x, y, x.length);
  } /* drawArrowhead */

  public boolean contains(Point p) {
    return false;
  } /* contains */

  public Point upperLeft() {
    return anchor;
  } /* upperLeft */

  public Point lowerRight() {
    return endpoint;
  } /* lowerRight */

  public Point upperRight() {
    return endpoint;
  } /* upperRight */

  public Point lowerLeft() {
    return anchor;
  } /* lowerLeft */

  public boolean inBounds() {
    if (canvas.contains(endpoint))
      return true;
    else
      return false;
  } /* inBounds */

  public void write(File f) throws java.io.IOException {
    f.writeInt(f.CONNECTOR);
    f.writeInt(anchor.x);
    f.writeInt(anchor.y);
    f.writeInt(endpoint.x);
    f.writeInt(endpoint.y);
  } /* write */

  public void read(File f) throws java.io.IOException {
    anchor = new Point();
    endpoint = new Point();

    anchor.x = f.readInt();
    anchor.y = f.readInt();
    endpoint.x = f.readInt();
    endpoint.y = f.readInt();
  } /* read */

  public int addInput(Connector c) throws ConnectionException {
    throw new ConnectionException("ERROR: Connectors cannot connect each other");
  } /* addInput */

  public int addOutput(Connector c) throws ConnectionException {
    throw new ConnectionException("ERROR: Connectors cannot connect each other");
  } /* addOutput */

  public void setInput(Primitive p) {
    startP = p;
  } /* setInput */

  public void setOutput(Primitive p) {
    endP = p;
  } /* setOutput */

  public Primitive getInput() {
    return startP;
  } /* getInput */

  public Primitive getOutput() {
    return endP;
  } /* getOutput */

  public Connector getInput(int i) throws ConnectionException {
    throw new ConnectionException("Internal error: wrong getInput() called on Connector");
  } /* getInput */

  public Connector getOutput(int i) throws ConnectionException {
    throw new ConnectionException("Internal error: wrong getOutput() called on Connector");
  } /* getOutput */

  public void removeInput(int i) throws ConnectionException {
    //throw new ConnectionException("ERROR: Attempted to remove input from connector");
    startP = null;
  } /* removeInput */

  public void removeOutput(int i) throws ConnectionException {
    //throw new ConnectionException("ERROR: Attempted to remove output from connector");
    endP = null;
  } /* removeOutput */

  public int getInputCount() throws ConnectionException {
    //throw new ConnectionException("ERROR: Attempted to retrieve input count from connector");
    return (startP == null) ? 0 : 1;
  } /* getInputCount */

  public int getOutputCount() throws ConnectionException {
    //throw new ConnectionException("ERROR: Attempted to retrieve output count from connector");
    return (endP == null) ? 0 : 1;
  } /* getOutputCount */

  public void event(Source s) throws ConnectionException {
    //throw new ConnectionException("ERROR: Attempted to push event to connector");
    endP.event(s);
  } /* event */

  public void wakeup(long t) throws ConnectionException {
    throw new ConnectionException("ERROR: Attempted to wake up connector");
  } /* wakeup */
}