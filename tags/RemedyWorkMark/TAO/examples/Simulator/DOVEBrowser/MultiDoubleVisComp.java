// $Id$
//
// = FILENAME
//    MultiDoubleVisComp.java
//
// = AUTHOR
//    Chris Gill (core graph functionality), based on
//    DoubleVisComp.java by Seth Widoff, and Michael Kircher
//    (mk1@cs.wustl.edu)
//
// = DESCRIPTION
//   This is a Visualization Component for displaying multiple
//   plots of double precision integer values.
//
// ============================================================================


import java.awt.*;
import java.util.*;

public class MultiDoubleVisComp extends Canvas implements VisComp
{
  private static int plot_count_default_ = 5;

  private static final int MIN_SPACING = 2;
  private static final int POINT_HISTORY = 200;
  private static final Font FONT = new Font ("Dialog", Font.PLAIN, 10);

  private Queue plots_ [];
  private int plot_count_ = 0;
  private String title_;
  private Graphics offgraphics_;
  private Image offscreen_;
  private Dimension offscreensize_;
  private int max_value_;
  private int old_max_value_;
  private int spacing_;
  private boolean max_increased_ = false;
  private static boolean monotonic_scale_ = false;

  private float local_max_[];
  private static float local_max_values_ [];
  private static int local_max_value_count_ = 0;
  private int local_max_value_index_[];

  public MultiDoubleVisComp ()
  {
    super ();

    // Re-initialize the global and local arrays of local maxima.
    local_max_init (plot_count_default_);

    // Set up the array of plot queues
    plot_count_ =  plot_count_default_;
    plots_ = new Queue [plot_count_];
    for (int i = 0; i < plot_count_; ++i)
      {
        plots_ [i] = new Queue ();
      }

    spacing_ = MIN_SPACING;
    title_ = "";
    max_value_ = 1;
    old_max_value_ = max_value_;

    java.util.Random rand = new java.util.Random (System.currentTimeMillis ());
    float hue_ = rand.nextFloat ();
    float brightness = rand.nextFloat ();

    hue_ += .075;

    if (hue_ > 1.0)
      hue_ -= 1.0;

    if (brightness > 0.75)
      brightness -= 0.25;

    Color new_color = Color.getHSBColor (hue_, 1, brightness);

    this.setBackground (new_color);
    this.setForeground (Color.white);
  }

  public static synchronized void plot_count_default (int d) {
    plot_count_default_ = d;
  }

  public static synchronized int plot_count_default () {
    return plot_count_default_;
  }

  public static synchronized void monotonic_scale (boolean b) {
    monotonic_scale_ = b;
  }

  public static synchronized boolean monotonic_scale () {
    return monotonic_scale_;
  }

  public void resize (int new_plot_count)
  {
    if (new_plot_count > plot_count_)
      {
        // Re-initialize the global and local arrays of local maxima.
        local_max_init (new_plot_count);

        // Set up the array of plot queues
        Queue plots_temp [] = new Queue [new_plot_count];
        for (int i = 0; i < new_plot_count; ++i)
          {
            if (i < plot_count_)
	      {
                plots_temp [i] = plots_ [i];
	      }
            else
	      {
                plots_temp [i] = new Queue ();
	      }
          }

        plots_ = plots_temp;
      }

    plot_count_ = new_plot_count;
  }



  public void setName (String title) {
      title_ = title;
  }

  public int getProperty () {
      return Properties.MULTIDOUBLE;
    }

  public Dimension getMinimumSize () {
      return new Dimension (75, 75);
    }

  public Dimension getPreferredSize () {
      return new Dimension (175, 175);
    }

  public String getName () {
    return title_;
  }

  public int getMax () {
      return old_max_value_;
    }

  public void update (java.util.Observable observable, java.lang.Object obj)
  {
    Double double_array[];
    try {
      double_array = (Double[]) obj;
    }
    catch (Exception excp) {
      double_array = new Double [plot_count_];
      for (int j = 0; j < plot_count_; ++j)
	{
          double_array [j] = new Double (0.0);
	}
      System.out.println (excp);
      System.out.println ("Multi Double Visualization Component received wrong data type!");
    }

    for (int i = 0; i < double_array.length && i < plot_count_; ++i)
      {
        float new_point = double_array [i].floatValue ();
        Float temp = (Float) plots_ [i].dequeue_tail ();
        plots_ [i].enqueue_head (new Float (new_point));

        if (new_point > local_max_ [i])
          {
            local_max_ [i] = new_point;
            local_max_values_ [local_max_value_index_ [i]] = local_max_ [i];
          }

        if (monotonic_scale_)
          {
            float global_max = 0;
            global_max = global_max_value ();

            while (global_max > max_value_)
              max_value_ *= 2;

            while ((global_max < max_value_/2) && (max_value_ > old_max_value_))
              max_value_ /= 2;
          }
        else
          {
            while (local_max_ [i] > max_value_)
              max_value_ *= 2;

            while ((local_max_ [i] < max_value_/2) && (max_value_ > old_max_value_))
              max_value_ /= 2;
          }
      }

    repaint ();
  }


  public void update (Graphics g)
  {
    int x1, y1, x2, y2;
    float tmp, value_1, value_2;
    Dimension d = getSize ();
    FontMetrics fm = g.getFontMetrics ();
    int fheight = fm.getHeight ();
    String value =  "Scale: " + max_value_ ;

    if ((offscreen_ == null) ||
	(offscreensize_.width != d.width - 8) ||
	(offscreensize_.height != d.height - 8))
      {
	offscreen_ = createImage (d.width - 8, d.height - 8);
	offscreensize_ = new Dimension (d.width - 8, d.height - 8);
	offgraphics_ = offscreen_.getGraphics ();
	offgraphics_.setFont (FONT);
      }

    g.setColor (Color.lightGray);
    g.draw3DRect (0, 0, d.width - 1, d.height - 1, true);
    g.draw3DRect (1, 1, d.width - 3, d.height - 3, true);
    g.draw3DRect (2, 2, d.width - 5, d.height - 5, true);

    offgraphics_.setColor (getBackground ());
    offgraphics_.fillRect (0, 0, offscreensize_.width, offscreensize_.height);
    offgraphics_.setColor (getForeground ());
    offgraphics_.drawString (title_, 5, fheight);
    offgraphics_.drawString (value, 5, offscreensize_.height - 5);

    for (int i = 0; i < plot_count_; ++i)
      {
        local_max_ [i] = 0;
        Enumeration queue_iter = plots_ [i].forward_iterator ();
        value_1 = ((Float) queue_iter.nextElement ()).floatValue ();
        x1 = d.width - 8;
        while (queue_iter.hasMoreElements ())
          {
	    value_2 = ((Float) queue_iter.nextElement ()).floatValue ();

	    if (value_1 > local_max_ [i])
	      local_max_ [i] = value_1;

            y1 = normalize (offscreensize_.height - fheight, value_1, i);
            y2 = normalize (offscreensize_.height - fheight, value_2, i);

	    tmp = value_2;
	    value_2 = value_1;
	    value_1 = tmp;

            x2 = x1 - spacing_;
            offgraphics_.drawLine (x1, y1, x2, y2);
            x1 = x2;
	    if (x1 <= 5)
	      break;
          }

        local_max_values_ [local_max_value_index_ [i]] = local_max_ [i];
      }

    g.drawImage (offscreen_, 3, 3, null);
  }

  public void paint (Graphics g)
  {
    Dimension d = getSize ();

    for (int i = 0; i < plot_count_; ++i)
      {
        int plot_length = plots_ [i].length ();
        int num_points = d.width / spacing_;

        if (plots_[i].length () < num_points)
          {
            for (int j = 0; j < num_points - plot_length; j++)
	      plots_ [i].enqueue_tail (new Float (0));
          }
        else if (plots_ [i].length () > num_points)
          {
	    for (int j = 0; j < plot_length - num_points; j++)
	      plots_ [i].dequeue_tail ();
          }
      }

    update (g);
  }

  private static synchronized float global_max_value () {
    float result = 0;

    for (int i = 0; i < local_max_value_count_; ++i)
      {
        if (result < local_max_values_ [i])
          {
            result = local_max_values_ [i];
          }
      }

    return result;
  }

  private synchronized void local_max_init (int new_count) {

    if (new_count > plot_count_)
      {
        // New larger static global array of all local maxima values
        float local_max_values_temp [] =
          new float [local_max_value_count_ + new_count - plot_count_];

        // Copy the previously stored maxima (if any) into the new array.
        for (int i = 0;
             i < local_max_value_count_ + new_count - plot_count_;
             ++i)
          {
            if (i < local_max_value_count_)
	      {
                local_max_values_temp [i] = local_max_values_ [i];
	      }
            else
              {
                local_max_values_temp [i] = 0;
	      }
          }

        // Create new, larger, non-static arrays for the local maxima, indices
        float local_max_temp [] = new float [new_count];
        int local_max_value_index_temp [] = new int [new_count];

        for (int j = 0; j < new_count; ++j)
          {
            if (j < plot_count_)
 	      {
                local_max_temp [j] = local_max_ [j];
                local_max_value_index_temp [j] = local_max_value_index_ [j];
	      }
            else
 	      {
                local_max_temp [j] = 0;
                local_max_value_index_temp [j] =
                  local_max_value_count_ + j - plot_count_;
	      }
          }

        // Store the newly (re)allocated arrays, set the new global array count
        local_max_values_ = local_max_values_temp;
        local_max_ = local_max_temp;
        local_max_value_index_ = local_max_value_index_temp;
        local_max_value_count_ += new_count - plot_count_;
      }
  }


  private int normalize (int height, float coord, int plot_index)
  {
    // First, figure out the ratio of the value to the max value
    float ratio = (float) coord/max_value_;

    // Second, compute pixel location based on which plot we're drawing
    float pixels = (float) (height*(plot_index + ratio))/plot_count_;

    // Then, subtract the pixel location
    float location = (float) height - pixels;

    return Math.round (location);
  }
}


