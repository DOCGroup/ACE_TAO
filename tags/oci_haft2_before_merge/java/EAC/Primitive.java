/**
 * Title:        Primitive
 * Description:  An abstract class for Event Analysis Configurator graphical
 *               primitives
 */
package EAC;
import java.awt.*;
import java.io.*;

public abstract class Primitive {

  // maximum number of outputs from any primitive
  public final int MAX_OUTPUTS = 8;

  // maximum number of inputs to a sink
  public final int MAX_INPUTS = 8;

  // where the primitive will be drawn
  protected EACPanel canvas;

  // anchor point of the graphical shape
  protected Point anchor;

  // associated label
  protected EACLabel label;

  public Primitive(EACPanel p) {
    canvas = p;
  } /* constructor */

  public void setAnchor(Point p) {
    anchor = p;
  } /* setAnchor */

  public Point getAnchor() {
    return anchor;
  } /* getAnchor */

  public void setLabel(EACLabel l) {
    label = l;
  } /* setLabel */

  public EACLabel getLabel() {
    return label;
  } /* getLabel */

  public void restart() {
    // no-op for most primitives
    // overload as needed
  } /* restart */

  public void specialDraw() {
    // no-op for some primitives
    // overload as needed
  } /* specialDraw */

  public void specialUndraw() {
    // no-op for some primitives
    // overload as needed
  } /* specialUndraw */

  public void reconnect() {
    // no-op for some primitives
    // overload as needed
  } /* reconnect */

  public void selectedDraw() {
    final int d = 4; // dimension of hotspot
    Graphics g = canvas.getGraphics();

    g.setColor(java.awt.Color.darkGray);
    g.drawRect(upperLeft().x - (d/2),upperLeft().y - (d/2), d, d);
    g.drawRect(lowerLeft().x - (d/2),lowerLeft().y - (d/2), d, d);
    g.drawRect(upperRight().x - (d/2),upperRight().y - (d/2), d, d);
    g.drawRect(lowerRight().x - (d/2),lowerRight().y - (d/2), d, d);
  } /* selectedDraw */

  // establish the top center point of the graphical shape
  public abstract void setTop(Point p);

  public abstract Point getTop();

  public abstract boolean inBounds();

  public abstract int addInput(Connector c) throws ConnectionException;

  public abstract int addOutput(Connector c) throws ConnectionException;

  public abstract Connector getInput(int i) throws ConnectionException;

  public abstract Connector getOutput(int i) throws ConnectionException;

  public abstract void removeInput(int i) throws ConnectionException;

  public abstract void removeOutput(int i) throws ConnectionException;

  public abstract int getInputCount() throws ConnectionException;

  public abstract int getOutputCount() throws ConnectionException;

  public abstract void event(Source s) throws ConnectionException;

  public abstract void wakeup(long t) throws ConnectionException;

  public abstract void draw() throws BoundsException;

  public abstract void write(File f) throws java.io.IOException;

  public abstract void read(File f) throws java.io.IOException;

  public abstract boolean contains(Point p);

  public abstract Point upperLeft();

  public abstract Point lowerRight();

  public abstract Point upperRight();

  public abstract Point lowerLeft();

}