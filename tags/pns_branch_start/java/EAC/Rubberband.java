/**
 * A abstract base class for rubberbands.<p>
 *
 * Rubberbands do their rubberbanding inside of a Component,
 * which must be specified at construction time.<p>
 *
 * Subclasses are responsible for implementing
 * <em>void drawLast(Graphics g)</em> and
 * <em>void drawNext(Graphics g)</em>.
 *
 * drawLast() draws the appropriate geometric shape at the last
 * rubberband location, while drawNext() draws the appropriate
 * geometric shape at the next rubberband location.  All of the
 * underlying support for rubberbanding is taken care of here,
 * including handling XOR mode setting; extensions of Rubberband
 * need not concern themselves with anything but drawing the
 * last and next geometric shapes.<p>
 *
 * @version 1.00, 12/27/95
 * @author  David Geary
 */
package EAC;
import java.awt.*;

abstract public class Rubberband {
  protected Point anchor    = new Point(0,0);
  protected Point stretched = new Point(0,0);
  protected Point last      = new Point(0,0);
  protected Point end       = new Point(0,0);

  private Component component;
  private boolean   firstStretch = true;

  abstract public void drawLast(Graphics g);
  abstract public void drawNext(Graphics g);

  public Rubberband(Component component) {
    this.component = component;
  }

  public Point getAnchor   () { return anchor; }
  public Point getStretched() { return stretched; }
  public Point getLast     () { return last; }
  public Point getEnd      () { return end; }
  public void setAnchor(Point p) { anchor = p; }
  public void setEnd(Point p) { end = p; }

  public void anchor(Point p) {
      firstStretch = true;
      anchor.x = p.x;
      anchor.y = p.y;

      stretched.x = last.x = anchor.x;
      stretched.y = last.y = anchor.y;
  }

  public void stretch(Point p) {
    last.x      = stretched.x;
    last.y      = stretched.y;
    stretched.x = p.x;
    stretched.y = p.y;

    Graphics g = component.getGraphics();
    if (g != null) {
      g.setXORMode(component.getBackground());

      if (firstStretch == true)
        firstStretch = false;
      else
        drawLast(g);

      drawNext(g);
    }
  }

  public void end(Point p) {
    last.x = end.x = p.x;
    last.y = end.y = p.y;

    Graphics g = component.getGraphics();
    if(g != null) {
      g.setXORMode(component.getBackground());
      drawLast(g);
    }
  }

  public Rectangle bounds() {
    return new Rectangle(stretched.x < anchor.x ?
                         stretched.x : anchor.x,
                         stretched.y < anchor.y ?
                         stretched.y : anchor.y,
                         Math.abs(stretched.x - anchor.x),
                         Math.abs(stretched.y - anchor.y));
  }

  public Rectangle lastBounds() {
    return new Rectangle(
                last.x < anchor.x ? last.x : anchor.x,
                last.y < anchor.y ? last.y : anchor.y,
                Math.abs(last.x - anchor.x),
                Math.abs(last.y - anchor.y));
  }
}
