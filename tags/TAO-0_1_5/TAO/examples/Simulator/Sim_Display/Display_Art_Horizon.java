// $Id$


import java.awt.*;
import java.io.*;

public class Display_Art_Horizon
  extends Panel
  implements Display_Object
{
  private Alt_Horizon alt_hor_ = new Alt_Horizon ();
  private Position pos_ = new Position ();

  public Display_Art_Horizon ()
    {
      setLayout (new BorderLayout (0, 2));
      add ("Center", alt_hor_);
      add ("South", pos_);
    }

  public int update_display (Display_Push_Consumer display_push_consumer) 
    {
      Navigation navigation = display_push_consumer.get_navigation ();
      /*
      System.out.println ("Art_Horizon: lat " + latitude + " long " + longitude + " alt " +
			  altitude + " heading " + heading + " roll " +
			  roll + " pitch " + pitch); 
			  */
      
      alt_hor_.update_display (navigation.roll, navigation.pitch);
      pos_.update_display (navigation.position_latitude, navigation.position_longitude, navigation.altitude, navigation.heading);
      
      return 0;
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

      /*
      System.out.println ("Start Angle " + angles[0] + 
      			  " End Angle " + angles[1] + 
      			  " Arc Angle " + arc_angle);
			  */
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
      
      // Shift the terrain line to account for the pitch.
      //      if (Math.abs (roll_) != 90)
      //	{
	  /*
	    boolean upside_down = Math.abs (roll_) >= 90;
	    double offset = - (double)rad * (double)pitch_ / 90.0;
	    double y_intercept = (Math.abs (roll_) <= 90) ? offset : offset;
	    double slope = caclulate_slope (rad, angles);
	    Point[] roots = null;
	    int x_l, x_r, y_l, y_r;
	    
	    roots = line_circle_intesect (rad, y_intercept, slope);
	    
	    // System.out.println ("Y intercept " + y_intercept + " slope " + slope);
	    // System.out.println ("Root x1 " + roots[0].x + " x2 " + roots[1].x);
	    // System.out.println ("Root y1 " + roots[0].y + " y2 " + roots[1].y);
	    // System.out.println ("Was left_point.x " + left_point.x + " left_point.y " + left_point.y);
	    // System.out.println ("Was right_point.x " + right_point.x + " right_point.y " + right_point.y);
	    
	    x_l = upside_down ? roots[0].x : roots[1].x;
	    y_l = upside_down ? roots[0].y : roots[1].y;
	    x_r = upside_down ? roots[1].x : roots[0].x;
	    y_r = upside_down ? roots[1].y : roots[0].y;
	    
	    angles[0] = calculate_angle (rad, x_l, y_l);
	    angles[1] = calculate_angle (rad, x_r, y_r);
	  
	  // System.out.println ("Now left_point.x " + left_point.x + " left_point.y " + left_point.y);
	  // System.out.println ("Now right_point.x " + right_point.x + " right_point.y " + right_point.y);
	  */
      //	}
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
      
	  //	  boolean rightside_up = Math.abs (roll_) <= 90,
	  //	    positive_roll = roll_ >= 0;
	  //	  double roll_rad = (double) roll_ * Math.PI / 180.0;	  
	  //	  Point tmp =
	  //	    new Point ((int) Math.round ((double) rad * Math.cos (roll_rad)),
	  //		       (int) Math.round ((double) rad * Math.sin (roll_rad)));

	  // System.out.println ("Was left_point.x " + left_point.x + " left_point.y " + left_point.y);
	  // System.out.println ("Was right_point.x " + right_point.x + " right_point.y " + right_point.y);

	  //	  right_point.x = tmp.x;
	  //	  left_point.x  = -tmp.x;
	  //	  right_point.y = tmp.y;
	  //	  left_point.y  = -tmp.y;
	  
	  // System.out.println ("Now left_point.x " + left_point.x + " left_point.y " + left_point.y);
	  // System.out.println ("Now right_point.x " + right_point.x + " right_point.y " + right_point.y);
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

class Position
extends Panel
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
      
      //      lat_.setBackground (Color.lightGray);
      //      long_.setBackground (Color.lightGray);
      //      alt_.setBackground (Color.lightGray);
      //      heading_.setBackground (Color.lightGray);

      //      setBackground (Color.black);
      //      setLayout (new CardLayout (1, 1));

      //      grid_panel.setBackground (Color.black);
      //      grid_panel.setLayout (new GridLayout(1, 4, 1, 0));
      //      grid_panel.add (lat_);
      //      grid_panel.add (long_);
      //      grid_panel.add (alt_);
      //      grid_panel.add (heading_);

      //      add ("Grid Panel", grid_panel);
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
