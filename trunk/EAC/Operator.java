/**
 *  Title:        Operator
 *  Description:  An abstract binary operator class for the Event Analysis
 *                Configurator
 */
package EAC;
import java.awt.*;

public abstract class Operator extends Primitive {

  // Radius of the circle
  protected final int DIAMETER = 40;

  // inputs
  protected Connector left_input,
                      right_input;

  // outputs
  protected Connector output[] = new Connector[MAX_OUTPUTS];
  protected int output_count = 0;

  // helps with dragging the operator
  protected boolean selected;
  Point old_anchor;

  public Operator(EACPanel p) {
    super(p);
  } /* constructor */

  public void setTop(Point p) {
    anchor = new Point(p.x - (int) (DIAMETER / 2), p.y);
  } /* setTop */

  public Point getTop() {
    return new Point(anchor.x + (int) (DIAMETER / 2), anchor.y);
  } /* getTop */

  public void draw() throws BoundsException {
    Graphics g = canvas.getGraphics();

    if (selected)
      selected = false;
    else if (inBounds())
      g.drawOval(anchor.x, anchor.y, DIAMETER, DIAMETER);
    else
      throw new BoundsException("ERROR: Attempted to place Operator partially out of bounds");
  } /* draw */

  public void specialDraw() {
    Graphics g = canvas.getGraphics();

    g.setXORMode(canvas.getBackground());
    g.drawOval(anchor.x,anchor.y,DIAMETER,DIAMETER);
  } /* specialDraw */

  public void specialUndraw() {
    Graphics g = canvas.getGraphics();

    g.setColor(canvas.getBackground());
    g.setXORMode(canvas.getForeground());
    g.drawOval(anchor.x,anchor.y,DIAMETER,DIAMETER);
  } /* specialUndraw */

  public void selectedDraw() {
    old_anchor = anchor;
    selected = true;
    super.selectedDraw();
  } /* selectedDraw */

  public void reconnect() {
    int i;

    // move output anchors
    for (i = 0; i < output_count; i++)
      output[i].setAnchor(new Point(output[i].getAnchor().x + anchor.x - old_anchor.x,
                                    output[i].getAnchor().y + anchor.y - old_anchor.y));

    // move input endpoints
    if (left_input != null)
      left_input.setEndpoint(new Point(left_input.upperRight().x + anchor.x - old_anchor.x,
                                       left_input.upperRight().y + anchor.y - old_anchor.y));
    if (right_input != null)
      right_input.setEndpoint(new Point(right_input.upperRight().x + anchor.x - old_anchor.x,
                                        right_input.upperRight().y + anchor.y - old_anchor.y));

  } /* reconnect */

  public boolean contains(Point p) {
    if ((p.x >= anchor.x) &&
        (p.x <= anchor.x + DIAMETER) &&
        (p.y >= anchor.y) &&
        (p.y <= anchor.y + DIAMETER))
      return true;
    else
      return false;
  } /* contains */

  public Point upperLeft() {
    return anchor;
  } /* upperLeft */

  public Point lowerRight() {
    return new Point(anchor.x + DIAMETER, anchor.y + DIAMETER);
  } /* lowerRight */

  public Point upperRight() {
    return new Point(anchor.x + DIAMETER, anchor.y);
  } /* upperRight */

  public Point lowerLeft() {
    return new Point(anchor.x, anchor.y + DIAMETER);
  } /* lowerLeft */

  public int addInput(Connector c) throws ConnectionException {
    if (left_input == null) {
      left_input = c;
      return 0;
    } else if (right_input == null) {
      right_input = c;
      return 1;
    } else
      throw new ConnectionException("ERROR: Operator already has two inputs");
  } /* addInput */

  public int addOutput(Connector c) throws ConnectionException {
    if (output_count < MAX_OUTPUTS)
      output[output_count++] = c;
    else
      throw new ConnectionException("ERROR: Maximum outputs established for this operator");

    return output_count;
  } /* addOutput */

  public Connector getInput(int i) throws ConnectionException {
    if ((i < 0) || (i > 1))
      throw new ConnectionException("ERROR: Bad input index for operator");
    else if (i == 0) {
      if (left_input == null)
        throw new ConnectionException("ERROR: No left input for operator");
      else
        return left_input;
    } else // i == 1
      if (right_input == null)
        throw new ConnectionException("ERROR: No right input for operator");

    return right_input;
  } /* getInput */

  public Connector getOutput(int i) throws ConnectionException {
    if ((i >= 0) && (i < output_count))
      return output[i];
    else
      throw new ConnectionException("ERROR: Bad output index for operator");
  } /* getOutput */

  public void removeInput(int i) throws ConnectionException {
    if ((i < 0) || (i > 1))
      throw new ConnectionException("ERROR: Bad input index for operator");
    else if (i == 0) {
      if (left_input == null)
        throw new ConnectionException("ERROR: No left input for operator");
      else
        left_input = null;
    } else if (i == 1) {
      if (right_input == null)
        throw new ConnectionException("ERROR: No right input for operator");
      else
        right_input = null;
    }
  } /* removeInput */

  public void removeOutput(int i) throws ConnectionException {
    if ((i < 0) || (i > output_count))
      throw new ConnectionException("ERROR: Bad output index for operator");
    else {
      output[i] = output[output_count-1];
      --output_count;
    }
  } /* removeOutput */

  public int getOutputCount() throws ConnectionException {
    return output_count;
  } /* getOutputCount */

  public int getInputCount() throws ConnectionException {
    if (left_input == null)
      return 0;
    else if (right_input == null)
      return 1;
    else
      return 2;
  } /* getInputCount */

  public void wakeup(long t) throws ConnectionException {
    throw new ConnectionException("ERROR: Attempted to wake up operator");
  } /* wakeup */

  public boolean inBounds() {
    if (canvas.contains(anchor.x,anchor.y) &&
        canvas.contains(anchor.x + DIAMETER, anchor.y + DIAMETER))
      return true;
    else
      return false;
  } /* inBounds */

}