// $Id$
// 
// = FILENAME
//    DoubleVisComp.java
//
// = AUTHOR
//    Seth Widoff (core graph functionality)
//    Michael Kircher (mk1@cs.wustl.edu)
//
// = DESCRIPTION
//   This is a Visualization Component for displaying doubles.
//
// ============================================================================




import java.awt.*;
import java.util.*;

public class DoubleVisComp extends Canvas implements VisComp
{
  private static final int MIN_SPACING = 2;
  private static final int POINT_HISTORY = 200;
  private static final Font FONT = new Font ("Dialog", Font.PLAIN, 10);
  
  private Queue plot_;
  private String title_;
  private Graphics offgraphics_;
  private Image offscreen_;
  private Dimension offscreensize_;
  private int max_value_;
  private int old_max_value_;
  private int spacing_;
  private float local_max_ = 0;
  private boolean max_increased_ = false;
  private static boolean monotonic_scale_ = false;
  private static int global_max_value_ = 0;

  public DoubleVisComp()
  {
    super();

    plot_ = new Queue();
    spacing_ = MIN_SPACING;
    title_ = "";
    max_value_ = 1;
    old_max_value_ = max_value_;
    
    java.util.Random rand = new java.util.Random (System.currentTimeMillis());
    float hue_ = rand.nextFloat();
    float brightness = rand.nextFloat();

    hue_ += .075;
    
    if (hue_ > 1.0)
      hue_ -= 1.0;
    
    if (brightness > 0.75)
      brightness -= 0.25;
    
    Color new_color = Color.getHSBColor(hue_, 1, brightness);

    this.setBackground(new_color);
    this.setForeground(Color.white);
  }

  public static synchronized void monotonic_scale (boolean b) {
    monotonic_scale_ = b;
  }

  public static synchronized boolean monotonic_scale () {
    return monotonic_scale_;
  }

  public static synchronized void raise_global_max_value (int i) {
    global_max_value_ = global_max_value_ < i ? i : global_max_value_;
  }

  public static synchronized int global_max_value () {
    return global_max_value_;
  }

  public void setName (String title) {
      title_ = title;
  }
  
  public int getProperty () {
      return Properties.DOUBLE;
    }

  public Dimension getMinimumSize () {
      return new Dimension (75, 75);
    }

  public Dimension getPreferredSize () {
      return new Dimension (175, 175);
    }
  
  public String getName() {
    return title_;
  }

  public int getMax() {
      return old_max_value_;
    }

  public void update(java.util.Observable observable, java.lang.Object obj)
  {
    Double double_temp_;
    try {
      double_temp_ = (Double) obj;  
    }
    catch (Exception excp) {
      double_temp_ = new Double (0.0);
      System.out.println (excp);
      System.out.println ("Visualization Component received wrong data type!");
    }
        
    float new_point = double_temp_.floatValue();
    Float temp = (Float)plot_.dequeue_tail();
    plot_.enqueue_head(new Float(new_point));
    
    if (new_point > local_max_)
      local_max_ = new_point;
    
    if (monotonic_scale_)
      {
        while (local_max_ > max_value_)
          max_value_ *= 2;

        while (global_max_value_ > max_value_)
          max_value_ *= 2;

        raise_global_max_value (max_value_);
      }
    else
      {
        while (local_max_ > max_value_)
          max_value_ *= 2;
    
        while ((local_max_ < max_value_/2) && (max_value_ > old_max_value_))
          max_value_ /= 2;
      }

    repaint();
  }

  public void update(Graphics g)
  {
    Dimension d = getSize ();
    float tmp, value_1, value_2;
    FontMetrics fm = g.getFontMetrics ();
    Enumeration queue_iter = plot_.forward_iterator();
    int x1 = d.width - 8, y1, x2, y2, fheight = fm.getHeight (), i;
    String value =  "Value (of " + max_value_ + "): " + String.valueOf(plot_.head());
    
    if ((offscreen_ == null) ||
	(offscreensize_.width != d.width - 8) ||
	(offscreensize_.height != d.height - 8))
      {
	offscreen_ = createImage(d.width - 8, d.height - 8);
	offscreensize_ = new Dimension(d.width - 8, d.height - 8);
	offgraphics_ = offscreen_.getGraphics();
	offgraphics_.setFont(FONT);
      }

    g.setColor (Color.lightGray);
    g.draw3DRect (0, 0, d.width - 1, d.height - 1, true);
    g.draw3DRect (1, 1, d.width - 3, d.height - 3, true);
    g.draw3DRect (2, 2, d.width - 5, d.height - 5, true);
    
    local_max_ = 0;
    offgraphics_.setColor (getBackground());
    offgraphics_.fillRect (0, 0, offscreensize_.width, offscreensize_.height);
    offgraphics_.setColor (getForeground());    
    offgraphics_.drawString(title_, 5, fheight);
    offgraphics_.drawString(value, 5, offscreensize_.height - 5);

    value_1 = ((Float)queue_iter.nextElement()).floatValue();
    while (queue_iter.hasMoreElements())
      {
	value_2 = ((Float)queue_iter.nextElement()).floatValue();
	
	if (value_1 > local_max_)
	  local_max_ = value_1;
	
        y1 = normalize(offscreensize_.height - fheight, value_1);
        y2 = normalize(offscreensize_.height - fheight, value_2);
	
	tmp = value_2;
	value_2 = value_1;
	value_1 = tmp;

        x2 = x1 - spacing_;
        offgraphics_.drawLine(x1, y1, x2, y2);
        x1 = x2;
	if (x1 <= 5)
	  break;
      }

    g.drawImage(offscreen_, 3, 3, null);
  }
  
  public void paint(Graphics g)
  {
    Dimension d = getSize ();
    int plot_length = plot_.length();
    int num_points = d.width / spacing_;
    
    if (plot_.length() < num_points)
      {
	for (int i = 0; i < num_points - plot_length; i++)
	  plot_.enqueue_tail(new Float(0));
      }
    else if (plot_.length() > num_points)
      {
	for (int i = 0; i < plot_length - num_points; i++)
	  plot_.dequeue_tail();
      }
    
    update(g);
  }


  private int normalize(int height, float coord)
  {
    float ratio = (float)coord/max_value_;
    float pixels = (float)height*ratio;
    float location = (float)height - pixels;
    
    return Math.round(location);
  }
}

