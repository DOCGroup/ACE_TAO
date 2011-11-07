// $Id$
//
// = FILENAME
//    NavigationVisComp.java
//
// = AUTHOR
//    Seth Widoff (core functionality)
//    Michael Kircher (mk1@cs.wustl.edu)
//
// = DESCRIPTION
//   This is a Visualization Component for displaying navigation.
//
// ============================================================================



import java.awt.*;
import java.io.*;

public class NavigationVisComp extends Panel implements VisComp
{
  private Alt_Horizon alt_hor_ = new Alt_Horizon ();
  private Position pos_ = new Position ();

  public NavigationVisComp ()
    {
      setLayout (new BorderLayout (0, 2));
      add ("Center", alt_hor_);
      add ("South", pos_);
    }

  public void setName (String title) {
  }

  public int getProperty () {
      return Properties.NAVIGATION;
    }

  public void update (java.util.Observable observable, java.lang.Object obj) {
    Navigation navigation_ = null;
    try {
      navigation_ = (Navigation) obj;
    }
    catch (Exception excp) {
      System.out.println (excp);
      System.out.println ("Visualization Component received wrong data type!");
    }
    if (navigation_ != null) {
      // make sure all the values are in the proper range.
      navigation_.roll = (navigation_.roll > 180 || navigation_.roll < -180) ?
	0 :  navigation_.roll;
      navigation_.pitch = (navigation_.pitch > 90 || navigation_.pitch < -90) ?
	0 :  navigation_.pitch;

      // update the artificial horizon
      alt_hor_.update_display (navigation_.roll, navigation_.pitch);

      navigation_.pitch = (navigation_.position_latitude > 90 || navigation_.position_latitude < -90) ?
	0 :  navigation_.position_latitude;
      navigation_.pitch = (navigation_.position_longitude > 100 || navigation_.position_longitude < 00) ?
	0 :  navigation_.position_longitude;
      navigation_.pitch = (navigation_.altitude > 90 || navigation_.altitude < -90) ?
	0 :  navigation_.altitude;
      navigation_.pitch = (navigation_.heading > 180 || navigation_.heading < -180) ?
	0 :  navigation_.heading;

      // update the position display
      pos_.update_display (navigation_.position_latitude,
			   navigation_.position_longitude,
			   navigation_.altitude,
			   navigation_.heading);
    }
  }
}

class Alt_Horizon
extends Canvas
{
  private final static Color GREEN = new Color (0, 100, 0),
    BLUE = new Color (30, 144, 255);

  private Graphics offgraphics_;
  private Image offscreen_;
  private Dimension offscreensize_;

  private int roll_ = 0, pitch_ = 0;

  public void update_display (int roll, int pitch)
    {
      roll_ = roll;
      pitch_ = pitch;

      repaint ();
    }

  public Dimension getPreferredSize ()
    {
      return new Dimension (180, 180);
    }

  public Dimension getMinimumSize ()
    {
      return new Dimension (80, 80);
    }

  public void paint (Graphics g)
    {
      update (g);
    }

  public void update (Graphics g)
    {
      Dimension d = getSize ();
      int rad, angles[] = { 180, 0 };
      Point center;

      if ((offscreen_ == null) || (d.width != offscreensize_.width) ||
	  (d.height != offscreensize_.height))
	{
	  offscreen_ = createImage (d.width, d.height);
	  offscreensize_ = new Dimension (d.width, d.height);
	  offgraphics_ = offscreen_.getGraphics ();
	  offgraphics_.setFont (getFont());

	  //	  g.setColor (Color.lightGray);
	  //	  g.draw3DRect (0, 0, d.width - 1, d.height - 1, true);
	  //	  g.draw3DRect (1, 1, d.width - 3, d.height - 3, true);
	  //	  g.draw3DRect (2, 2, d.width - 5, d.height - 5, true);
	}

      offgraphics_.setColor (getBackground());
      offgraphics_.fillRect (0, 0, d.width, d.height);
      offgraphics_.setColor (BLUE);

      // Calculate from the dimensions, the largest square.
      center = new Point (d.width / 2, d.height / 2);
      rad = ((center.x < center.y) ? center.x : center.y);

      // Draw a circle of blue
      offgraphics_.fillOval (center.x - rad, center.y - rad,
			     2*rad, 2*rad);

      // Roll the horizon based on the roll angle
      if (roll_ != 0)
	roll_horizon (rad, angles);

      // Pitch the horizon based on the pitch angle
      if (pitch_ != 0)
	pitch_horizon (rad, angles);

      // Draw the resulting terrain
      draw_horizon (rad, center, angles);

      // Draw the plotted Image.
      g.drawImage (offscreen_, 0, 0, null);
    }

  private void draw_horizon (int rad, Point center, int[] angles)
    {
      // Draw an arc
      int arc_angle =
	((angles[0] > angles[1]) ?
	 (360 - angles[0]) + angles[1] :
	 (angles[1] - angles[0]));

      Polygon remainder = new Polygon ();

      offgraphics_.setColor (GREEN);
      offgraphics_.fillArc (center.x - rad, center.y - rad,
			    2*rad, 2*rad,
			    angles[0], arc_angle);

      if (pitch_ != 0)
	{
	  if ((pitch_ > 0 && Math.abs (roll_) < 90) ||
	      (pitch_ < 0 && Math.abs (roll_) >= 90))
	    offgraphics_.setColor (BLUE);

	  int cover_angle = (angles[0] + arc_angle/2 + ((arc_angle < 180) ? 180 : 0)) % 360;

	  // System.out.println (points[0] + " " + points[1]);

	  // System.out.println (accepted_point);

	  remainder.addPoint (center.x + polar_to_rect_x (rad, cover_angle),
			      center.y - polar_to_rect_y (rad, cover_angle));
	  remainder.addPoint (center.x + polar_to_rect_x (rad, angles[0]),
			      center.y - polar_to_rect_y (rad, angles[0]));
	  remainder.addPoint (center.x + polar_to_rect_x (rad, angles[1]),
			      center.y - polar_to_rect_y (rad, angles[1]));
	  offgraphics_.fillPolygon (remainder);
	  //offgraphics_.setColor (getBackground ());
	  //offgraphics_.drawPolygon (remainder);
	}
    }

  private void pitch_horizon (int rad, int[] angles)
    {
      boolean upside_down = Math.abs (roll_) >= 90;
      int angle_shift = (int) Math.round ((double)(90 - (Math.abs (roll_) % 180)) / 90.0 * pitch_);

      //      System.out.println ("angle_shift " + angle_shift);

      angles[0] += angle_shift;
      angles[1] -= angle_shift;


    }

  private void roll_horizon (int rad, int[] angles)
    {
      // Roll the left and right points of the terrain.
      angles[0] += roll_;
      angles[1] += roll_;

      if (angles[0] < 0)
	angles[0] += 360;

      if (angles[1] < 0)
	angles[1] += 360;
    }

  private int polar_to_rect_x (int rad, int angle)
    {
      return (int) Math.round (rad * Math.cos ((double)angle * Math.PI/180.0));
    }

  private int polar_to_rect_y (int rad, int angle)
    {
      return (int) Math.round (rad * Math.sin ((double)angle * Math.PI/180.0));
    }

  private double caclulate_slope (int rad, int[] angles)
    {
      int x1 = polar_to_rect_x (rad, angles[0]),
	x2 = polar_to_rect_x (rad, angles[1]),
	y1 = polar_to_rect_y (rad, angles[0]),
	y2 = polar_to_rect_y (rad, angles[1]);

      return ((double) (y2 - y1)) / ((double) (x2 - x1));
    }

  private Point[] line_circle_intesect (int rad, double y_intercept, double slope)
    {
      double r_2 = (double)(rad * rad),
	s_2 = slope * slope,
	a_x = s_2 + 1,
	b_x = 2.0 * slope * y_intercept,
	c_x = y_intercept * y_intercept - r_2;
      int[] x_roots = quad_eq (a_x, b_x, c_x),
	y_roots = { (int) Math.round ((double)((double) x_roots[0])*slope + y_intercept),
		    (int) Math.round ((double)((double) x_roots[1])*slope + y_intercept) };
      Point[] points = new Point [2];

      points[0] = new Point (x_roots[0], y_roots[0]);
      points[1] = new Point (x_roots[1], y_roots[1]);

      return points;
    }

  private int calculate_angle (int rad, int x, int y)
    {
      /*
	double angle = 0,
      sin_value = Math.asin ((double)y / (double)rad),
      tan_value = Math.atan ((double)y / (double)x);

	if (x >= 0)
      angle = (x !=  0) ? tan_value : sin_value +
      ((y < 0) ? 2*Math.PI : 0);
	  else
      angle = Math.PI + tan_value;

      return (int) Math.round (angle * 180.0 / Math.PI);
      */

      double angle = 0.0,
	sin_value = Math.asin ((double)Math.abs (y) / (double)rad);

      if (x >= 0 && y >= 0)
	angle = sin_value;
      else if (x < 0 && y >= 0)
	angle = sin_value + Math.PI/2.0;
      else if (x < 0 && y < 0)
	angle = sin_value + Math.PI;
      else if (x >= 0 && y < 0)
	angle = sin_value + 3.0*Math.PI/2.0;

      return (int) Math.round (angle * 180.0 / Math.PI);
    }

  private int[] quad_eq (double a, double b, double c)
    {
      int[] roots = new int [2];
      double body = Math.sqrt (b*b - 4.0*a*c);

      roots[0] = (int) Math.round ((-b + body) / (2.0 * a));
      roots[1] = (int) Math.round ((-b - body) / (2.0 * a));

      return roots;
    }

  private int distance (Point point1, Point point2)
    {
      double xdiff = point1.x - point2.x,
	ydiff = point1.y - point2.y;

      return (int) Math.round (Math.sqrt (xdiff*xdiff + ydiff*ydiff));
    }
}

class Position extends Panel
{
  private final static Font FONT = new Font ("Dialog", Font.BOLD, 12);
  private final static char DEGREE = '\u00B0';

  private Label lat_ = new Label ("0" + DEGREE + " N", Label.RIGHT),
    long_ = new Label ("0" + DEGREE + " S", Label.RIGHT),
    alt_ = new Label ("0 Kft", Label.RIGHT),
    heading_ = new Label ("0" + DEGREE + "  ", Label.RIGHT);

  public Position ()
    {
      Panel grid_panel = new Panel ();

      lat_.setFont (FONT);
      long_.setFont (FONT);
      alt_.setFont (FONT);
      heading_.setFont (FONT);

      setLayout (new GridLayout (1, 4));
      add (lat_);
      add (long_);
      add (heading_);
      add (alt_);
    }

  public void update_display (int lat, int lon, int alt, int heading)
    {
      String lat_str =
	Math.abs (lat) + "" + DEGREE + ((lat > 0) ? " N" : " S");
      String long_str =
	Math.abs (lon) + "" + DEGREE + ((lon > 0) ? " E" : " W");

      lat_.setText (lat_str);
      long_.setText (long_str);
      alt_.setText (alt + " Kft");
      heading_.setText (heading + "" + DEGREE + "  ");
    }
}
