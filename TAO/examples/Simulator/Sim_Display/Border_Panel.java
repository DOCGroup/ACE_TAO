// $Id$ 
import java.awt.*;

public class Border_Panel extends Panel
{
  private String title_;
  private final static Insets SPACE = new Insets (16, 16, 16, 16);
  
  public Border_Panel (Component component, String title)
    {
      GridBagLayout gb = new GridBagLayout ();
      GridBagConstraints gbc = new GridBagConstraints ();
      
      title_ = title;           
      setFont (new Font ("Dialog", Font.PLAIN, 12));
      setLayout (new CardLayout (SPACE.left, SPACE.top));
		 
      add ("Front", component);
    }
  
  public void paint (Graphics g)
    {
      FontMetrics fm = g.getFontMetrics ();
      Dimension d = getSize ();
      int font_height = fm.getAscent (),
	rect_x = SPACE.left/2,
	rect_y = SPACE.top/2,
	rect_width = d.width - SPACE.right,
	rect_height = d.height - SPACE.top/2 - SPACE.bottom/2,
	font_x = SPACE.left/2 + 20,
	font_y = (int)Math.round (SPACE.top/2 + font_height/2.0 - 1);

      g.setColor (getBackground ());
      
      g.draw3DRect (rect_x - 1, rect_y - 1,
		    rect_width + 1,  rect_height + 1, false);
      g.draw3DRect (rect_x, rect_y,
		    rect_width - 1, rect_height - 1, true);
      g.fillRect (font_x - 2, 0, fm.stringWidth (title_) + 4, SPACE.top);
      
      g.setColor (Color.black);
      g.drawString (title_, font_x, font_y);
    }

  public static void main (String[] args)
    {
      Frame yadda = new Frame ("Border Frame Test");
      Panel blank = new Panel ();
      Border_Panel bpanel = new Border_Panel (blank, "Border Panel");

      yadda.setLayout (new BorderLayout ());
      yadda.add ("Center", bpanel);
      yadda.setBounds (0, 0, 640, 480);

      yadda.show ();
    }
}
