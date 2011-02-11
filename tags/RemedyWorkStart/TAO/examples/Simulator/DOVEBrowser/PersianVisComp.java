// $Id$
//
// = FILENAME
//     PersianVisComp.java
//
// = AUTHOR
//      Chris Gill <cdgill@cs.wustl.edu>
//
// = DESCRIPTION
//     This is a Visualization Component for displaying a Persian
//     Recursion drawing.
//
// ============================================================================




import java.awt.*;
import java.util.*;

// This was needed to help the class loader.
import java.awt.image.MemoryImageSource;

public class PersianVisComp extends Canvas implements VisComp
{
  private static final Font FONT = new Font ("Dialog", Font.PLAIN, 10);
  private static final int PIXELS_WIDE = 256;
  private static final int TOTAL_PIXELS = PIXELS_WIDE * PIXELS_WIDE;
  private static final int DRAW_OFFSET = 32;

  private String title_;
  private Graphics offgraphics_;
  private Image offscreen_;
  private Dimension offscreensize_;
  private int pixel_array_ [];
  private MemoryImageSource image_source_;
  public PersianVisComp ()
  {
    super ();
    title_ = "";
    this.setBackground (Color.white);
    this.setForeground (Color.black);
    pixel_array_ = new int [TOTAL_PIXELS];
    for (int i = 0; i < TOTAL_PIXELS; ++i)
      pixel_array_ [i] = Color.white.getRGB ();
    image_source_ = new MemoryImageSource (PIXELS_WIDE, PIXELS_WIDE,
                                           pixel_array_, 0, PIXELS_WIDE);
  }

  public void drawPixels (int x1, int y1, int x2, int y2, Color c) {

    int x_min;
    int x_max;
    int y_min;
    int y_max;
    int rgb_value = c.getRGB ();

    if (x1 < 0 || y1 < 0 || x2 < 0 || y2 < 0
        || x1 >= PIXELS_WIDE || y1 >= PIXELS_WIDE
        || x2 >= PIXELS_WIDE || y2 >= PIXELS_WIDE)
      {
        System.err.println ("coordinates out of range: ("
                            + x1 + ", " + y1 + ") ("
                            + x2 + ", " + y2 + ")\n");
        return;
      }

    if (x1 < x2)
      {
        x_min = x1;
        x_max = x2;
      }
    else
      {
        x_min = x2;
        x_max = x1;
      }

    if (y1 < y2)
      {
        y_min = y1;
        y_max = y2;
      }
    else
      {
        y_min = y2;
        y_max = y1;
      }

    //    System.out.println ("Updating pixels: ("
    //                   + x1 + ", " + y1 + ") ("
    //                   + x2 + ", " + y2 + ")\n");

    for (int x = x_min; x <= x_max; ++x)
      for (int y = y_min; y <= y_max; ++y)
        pixel_array_ [y * PIXELS_WIDE + x] = rgb_value;
  }

  public void setName (String title) {
      title_ = title;
  }

  public int getProperty () {
      return Properties.PERSIAN;
    }

  public Dimension getMinimumSize () {
      return new Dimension (PIXELS_WIDE + 2 * DRAW_OFFSET,
                            PIXELS_WIDE + 2 * DRAW_OFFSET);
    }

  public Dimension getPreferredSize () {
      return new Dimension (PIXELS_WIDE + 2 * DRAW_OFFSET,
                            PIXELS_WIDE + 2 * DRAW_OFFSET);
    }

  public String getName () {
    return title_;
  }

  public void update (java.util.Observable observable, java.lang.Object obj)
  {
    PersianRecursion.Data data_temp_;
    try {
      data_temp_ = (PersianRecursion.Data) obj;
    }
    catch (Exception excp) {
      data_temp_ = null;
      System.out.println (excp);
      System.out.println ("PR Visualization Component received exception!");
      return;
    }

    Color c = Color.black;

    if (data_temp_.line_color == PersianRecursion.Line_Color_t.BLUE)
     {
       c = Color.red;
       // c = Color.blue;
     }
    else if (data_temp_.line_color == PersianRecursion.Line_Color_t.RED)
     {
       c = Color.yellow;
       //       c = Color.red;
     }
    else if (data_temp_.line_color == PersianRecursion.Line_Color_t.GREEN)
     {
       c = Color.blue;
       //       c = Color.green;
     }
    else if (data_temp_.line_color == PersianRecursion.Line_Color_t.YELLOW)
     {
       c = Color.green;
       // c = Color.yellow;
     }

    drawPixels (data_temp_.x1, data_temp_.y1,
                data_temp_.x2, data_temp_.y2, c);

    //    System.out.println ("PersianVisComp: drawing {"
    //                  + data_temp_.x1 + ", " + data_temp_.y1 + ", "
    //                  + data_temp_.x2 + ", " + data_temp_.y2 + "}");

    repaint ();
  }

  public void update (Graphics g)
  {
    Dimension d = getSize ();
    FontMetrics fm = g.getFontMetrics ();
    int x1 = d.width - 8, y1, x2, y2, fheight = fm.getHeight (), i;
    PersianRecursion.Data data_temp_;

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

    Image img = createImage (image_source_);
    offgraphics_.drawImage (img, DRAW_OFFSET, DRAW_OFFSET, null);

    g.drawImage (offscreen_, 3, 3, null);

    //    System.out.println ("updated.");
  }

  public void paint (Graphics g)
  {
    update (g);
    //    System.out.println ("painted.");
  }
}


