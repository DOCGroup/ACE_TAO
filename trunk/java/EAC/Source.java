/**
 * Title:        Source
 * Description:  An event source for the Event Analysis Configurator
 */
package EAC;
import java.awt.*;

public class Source extends Primitive {

  // Font for text
  protected Font text_font = new EACFont().getFont();

  // triangle dimensions
  protected final int WIDTH = 20;
  protected final int HEIGHT = 35;

  // lower left and lower right corner points
  // inherited anchor attribute holds the top point
  protected Point p2,
                  p3;

  // period at which events will be generated
  protected int period;

  // where the Source was anchored prior to a move
  protected Point old_anchor;

  // where events will be pushed
  protected Connector output[] = new Connector[MAX_OUTPUTS];
  protected int output_count;

  // Is this Source currently selected for dragging?
  protected boolean selected = false;

  // Black-on-white (normal) or white-on-black (reversed)?
  protected boolean reverse_color = false;

  // configuration to which the Source belongs
  Configuration config;

  public Source(EACPanel p, Configuration c) {
    super(p);
    config = c;
    output_count = 0;
    period = 0;
  } /* constructor */

  private void drawTriangle(Graphics g) {
    g.drawLine(anchor.x,anchor.y,p2.x,p2.y);
    g.drawLine(p2.x,p2.y,p3.x,p3.y);
    g.drawLine(p3.x,p3.y,anchor.x,anchor.y);
  } /* drawTriangle */

  public void draw() throws BoundsException {
    int x[] = new int[3],
        y[] = new int[3];

    if (selected)
      selected = false;
    else if (inBounds()) {
      Graphics g = canvas.getGraphics();
      Color bg,
            fg;

      x[0] = anchor.x;
      x[1] = p2.x;
      x[2] = p3.x;

      y[0] = anchor.y;
      y[1] = p2.y;
      y[2] = p3.y;

      if (reverse_color) {
        bg = Color.black;
        fg = Color.white;
      } else {
        bg = Color.white;
        fg = Color.black;
      }

      g.setColor(bg); // for interior
      g.fillPolygon(x,y,3);

      g.setColor(Color.black); // triangle itself is always black
      drawTriangle(g);

      g.setColor(fg); // for text

      // print period
      if (period > 0) {
        g.setFont(text_font);
        g.drawString(java.lang.String.valueOf(period),
                     anchor.x - (3 * java.lang.String.valueOf(period).length()),
                     anchor.y + 28);
      }
    } else
      throw new BoundsException("ERROR: Attempted to place source partially out of bounds");
  } /* draw */

  public void selectedDraw() {
    Graphics g = canvas.getGraphics();
    int x[] = new int[3],
        y[] = new int[3];

    old_anchor = anchor;
    x[0] = anchor.x;
    x[1] = p2.x;
    x[2] = p3.x;

    y[0] = anchor.y;
    y[1] = p2.y;
    y[2] = p3.y;

    //drawTriangle();
    g.setColor(canvas.getBackground());
    g.fillPolygon(x,y,3);

    // this will preclude drawing this Source for one repaint() call
    selected = true;

    super.selectedDraw();
  } /* selectedDraw */

  public void specialDraw() {
    Graphics g = canvas.getGraphics();

    g.setXORMode(canvas.getBackground());
    drawTriangle(g);
  } /* specialDraw */

  public void specialUndraw() {
    Graphics g = canvas.getGraphics();

    g.setColor(canvas.getBackground());
    g.setXORMode(canvas.getForeground());
    drawTriangle(g);
  } /* specialUndraw */

  public void reverseColor() {
    reverse_color = true;
  } /* reverseColor */

  public void normalColor() {
    reverse_color = false;
  } /* normalColor */

  public void reconnect() {
    int i;

    for (i = 0; i < output_count; i++)
      output[i].setAnchor(new Point(output[i].getAnchor().x + anchor.x - old_anchor.x,
                                    output[i].getAnchor().y + anchor.y - old_anchor.y));

    // move label anchor
    label.setAnchor(new Point(label.getAnchor().x + anchor.x - old_anchor.x,
                              label.getAnchor().y + anchor.y - old_anchor.y));
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

  public void setPeriod(int p) throws NumberFormatException {
    if (p > 0) {
      period = p;
      config.eventEnqueue(this,period); // Enqueue first wakeup time
    }
    else
      throw new NumberFormatException();
  } /* setPeriod */

  public int getPeriod() {
    return period;
  } /* getPeriod */

  public void restart() {
    config.eventEnqueue(this,period);
    reverse_color = false;
    selected = false;
  } /* restart */

  public int addOutput(Connector c) throws ConnectionException {
    if (output_count == MAX_OUTPUTS)
      throw new ConnectionException("ERROR: Maximum outputs established for source");
    else
      output[output_count++] = c;

    return output_count;
  } /* addOutput */

  public int addInput(Connector c) throws ConnectionException {
    throw new ConnectionException("ERROR: Attempted to add input to source");
  } /* addInput */

  public Connector getOutput(int i) throws ConnectionException {
    if ((i < 0) || (i >= output_count))
      throw new ConnectionException("ERROR: Bad output index for source");
    else
      return output[i];
  } /* getOutput */

  public Connector getInput(int i) throws ConnectionException {
    throw new ConnectionException("ERROR: Attempted to retrieve input from source");
  } /* getInput */

  public void removeInput(int i) throws ConnectionException {
    throw new ConnectionException("ERROR: Attempted to remove input from source");
  } /* removeInput */

  public void removeOutput(int i) throws ConnectionException {
    if ((i >= 0) && (i < output_count)) {
      output[i] = output[output_count-1];
      --output_count;
    } else
      throw new ConnectionException("ERROR: Bad output index for source");
  } /* removeOutput */

  public int getOutputCount() throws ConnectionException {
    return output_count;
  } /* getOutputCount */

  public int getInputCount() throws ConnectionException {
    throw new ConnectionException("ERROR: Attempted to retrieve input count from source");
  } /* getInputCount */

  public void event(Source s) throws ConnectionException {
    throw new ConnectionException("Internal error: event() called on source");
  } /* event */

  public void wakeup(long t) throws ConnectionException {
    int i;

    //System.out.println("Source wakeup at: " + java.lang.Long.toString(t));
    config.eventEnqueue(this,t + period); // enqueue next wakeup time

    // send an event to all outputs
    for (i = 0; i < output_count; i++)
      output[i].event(this);
  } /* wakeup */

  public boolean inBounds() {
    if (canvas.contains(bottomLeft()) &&
        canvas.contains(bottomRight()) &&
        canvas.contains(anchor))
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
    char temp[]; // for reading label
    int i, n;

    f.writeInt(f.SOURCE);
    f.writeInt(anchor.x);
    f.writeInt(anchor.y);
    f.writeInt(period);

    // Write out the associated label
    n = label.getText().length();
    f.writeInt(n); // text length
    temp = new char[n];
    temp = label.getText().toCharArray();

    for (i = 0; i < n; i++)
      f.writeChar(temp[i]);
    f.writeInt(label.getAnchor().x); // label position
    f.writeInt(label.getAnchor().y);
  } /* write */

  public void read(File f) throws java.io.IOException {
    char temp[]; // for reading label
    int i, n;
    int label_x, label_y;

    anchor = new Point();
    anchor.x = f.readInt();
    anchor.y = f.readInt();
    period = f.readInt();
    config.eventEnqueue(this,period); // Enqueue first event
    p2 = bottomLeft();
    p3 = bottomRight();

    // get label info
    n = f.readInt(); // text length
    label = new EACLabel(canvas,n);
    temp = new char[n];
    for (i = 0; i < n; i++)
      temp[i] = f.readChar();
    label.setText(java.lang.String.valueOf(temp));
    label_x = f.readInt();
    label_y = f.readInt();
    label.setAnchor(new Point(label_x,label_y));
    label.setLabelee(this);
    try {
      config.addPrimitive(label);
    } catch (TooManyPrimitivesException tmpe) {
      // have faith :-)
    }
  } /* read */
}