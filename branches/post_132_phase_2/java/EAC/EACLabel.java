/**
 * Title:        EACLabel
 * Description:  Text labels for Event Analysis Configurator graphical
 *               primitives
 */
package EAC;
import java.awt.*;

public class EACLabel extends Primitive {

  // Font for labels
  protected Font label_font = new EACFont().getFont();

  // Font Metrics for sizing and bounds checking labels
  protected FontMetrics fm = canvas.getFontMetrics(label_font);

  // Label text
  protected char[] text;

  // associated Primitive
  protected Primitive labelee;

  public EACLabel(EACPanel p, int n) {
    super(p);
    text = new char[n];
  } /* constructor */

  public void setLabelee(Primitive p) {
    labelee = p;
  } /* setLabelee */

  public Primitive getLabelee() {
    return labelee;
  } /* getLabelee */

  public void setText(String s) {
    text = s.toCharArray();
  } /* setText */

  public String getText() {
    return java.lang.String.valueOf(text);
  } /* getText */

  public void setTop(Point p) {
    anchor = new Point(p.x - (3 * text.length),
                       p.y + 9);
  } /* setTop */

  public Point getTop() {
    return new Point(anchor.x + (3 * text.length),
                     anchor.y - 9);
  } /* getTop */

  public void draw() throws BoundsException {
    if (inBounds()) {
      Graphics g = canvas.getGraphics();
      g.setFont(label_font);
      g.drawChars(text,0,text.length,anchor.x,anchor.y);
    } else
      throw new BoundsException("Attempted to place Label partially out of bounds");
  } /* draw */

  public void specialDraw() {
    Graphics g = canvas.getGraphics();

    g.setXORMode(canvas.getBackground());
    g.drawRect(upperLeft().x, upperLeft().y,
               upperRight().x - upperLeft().x, // WIDTH
               lowerRight().y - upperRight().y); // HEIGHT
  } /* specialDraw */

  public void specialUndraw() {
    Graphics g = canvas.getGraphics();

    g.setColor(canvas.getBackground());
    g.setXORMode(canvas.getForeground());
    g.drawRect(upperLeft().x, upperLeft().y,
               upperRight().x - upperLeft().x, // WIDTH
               lowerRight().y - upperRight().y); // HEIGHT
  } /* specialUndraw */

  public boolean contains(Point p) {
    return ((p.x >= upperLeft().x) &&
            (p.x <= upperRight().x) &&
            (p.y <= lowerLeft().y) &&
            (p.y >= upperLeft().y));
  } /* contains */

  public boolean inBounds() {
    // don't know why we need this, but fm.stringWidth seems to return a
    // value that's bigger than the actual width of the text
    //final int xTweak = 20;

    //if (canvas.contains(anchor.x + fm.stringWidth(text.toString()) - xTweak,
    //                    anchor.y - fm.getHeight()))
    if ((canvas.contains(upperLeft())) &&
        (canvas.contains(upperRight())) &&
        (canvas.contains(lowerLeft())) &&
        (canvas.contains(lowerRight())))
      return true;
    else
      return false;
  } /* inBounds */

  public Point upperLeft() {
    return new Point(anchor.x,anchor.y - 9);
  } /* upperLeft */

  public Point upperRight() {
    return new Point(anchor.x + (6 * text.length),anchor.y - 9);
  } /* upperRight */

  public Point lowerLeft() {
    return anchor;
  } /* lowerLeft */

  public Point lowerRight() {
    return new Point(anchor.x + (6 * text.length),anchor.y);
  } /* lowerRight */

  public void write(File f) throws java.io.IOException {
    int i;

    // if there's an associated primitive, that primitive
    // is responsible for writing out the label info too,
    // so only write out the info if there is no labelee
    if (labelee == null) {
      f.writeInt(f.LABEL);
      f.writeInt(text.length);
      f.writeInt(anchor.x);
      f.writeInt(anchor.y);
      for (i = 0; i < text.length; i++)
        f.writeChar(text[i]);
    }
  } /* write */

  public void read(File f) throws java.io.IOException {
    int i;

    anchor = new Point();
    anchor.x = f.readInt();
    anchor.y = f.readInt();

    for (i = 0; i < text.length; i++)
      text[i] = f.readChar();
  } /* read */

  public int addInput(Connector c) throws ConnectionException {
    throw new ConnectionException("ERROR: Attempted to add input to label");
  } /* addInput */

  public int addOutput(Connector c) throws ConnectionException {
    throw new ConnectionException("ERROR: Attempted to add output to label");
  } /* addOutput */

  public Connector getInput(int i) throws ConnectionException {
    throw new ConnectionException("ERROR: Attempted to retrieve input from label");
  } /* getInput */

  public Connector getOutput(int i) throws ConnectionException {
    throw new ConnectionException("ERROR: Attempted to retrieve output from label");
  } /* getOutput */

  public int getInputCount() throws ConnectionException {
    throw new ConnectionException("ERROR: Attempted to retrieve input count from label");
  } /* getInputCount */

  public int getOutputCount() throws ConnectionException {
    throw new ConnectionException("ERROR: Attempted to retrieve output count from label");
  } /* getOutputCount */

  public void removeInput(int i) throws ConnectionException {
    throw new ConnectionException("ERROR: Attempted to remove input from label");
  } /* removeInput */

  public void removeOutput(int i) throws ConnectionException {
    throw new ConnectionException("ERROR: Attempted to remove output from label");
  } /* removeOutput */

  public void event(Source s) throws ConnectionException {
    throw new ConnectionException("ERROR: Attempted to push event to label");
  } /* event */

  public void wakeup(long t) throws ConnectionException {
    throw new ConnectionException("ERROR: Attempted to wake up label");
  } /* wakeup */
}