package imaging.framework;

import java.awt.*;
import java.util.Vector;

public class Hierarchy extends Canvas
{
  // Attributes
  private String name_;
  private Hierarchy root_;
  private Hierarchy parent_;
  private Icon icon_ = null;
  
  // State
  private boolean collapsed_ = true;
  private boolean deadend_ = true;
  private Vector contained_ = new Vector();
  private Rectangle self_;
  private Rectangle scope_;
  
  // Constants
  private Color COLOR = Color.black;
  private Font FONT = new Font("Dialog", Font.BOLD, 12);
  private static int TRI_HEIGHT = 8;
  private int SPACE = 15;

  static Hierarchy last_lit_ = null;
  
  public Hierarchy(String name, boolean root)
  {
    name_ = name;
    
    if (root)
      {
	deadend_ = false;
	root_ = this;
	parent_ = null;
      }
  }
  
  public Hierarchy(String name, Icon icon, boolean root)
  {
    this(name, root);
    icon_ = icon;
  }

  public Hierarchy(String name)
  {
    this(name, false);
  }
  
  public Hierarchy(String name, Icon icon)
  {
    this(name, icon, false);
  }

  public void setExpandable(boolean expandable)
  {
    deadend_ = ! expandable;

    /*
    if (root_ != null)
      root_.repaint();
      */
  }

  public void setCollapsed(boolean collapsed)
  {
    collapsed_ = collapsed;

    if (root_ != null)
      root_.repaint();
  }

  public void addEntry(Hierarchy entry)
  {
    deadend_ = false;
    entry.parent_ = this;
    entry.root_ = root_;
    entry.FONT = FONT;
    entry.COLOR = COLOR;
    entry.SPACE = SPACE;

    contained_.addElement(entry);
  }

  public boolean removeEntry(String name)
  {
    if (contained_.size() == 1)
      deadend_ = true;
    
    return contained_.removeElement(name);
  }

  public String getName()
  {
    return name_;
  }

  public void setName(String name)
  {
    name_ = name;
  }

  public Hierarchy getHierarchyParent()
  {
    return parent_;
  }

  public void setFont(Font font)
  {
    FONT = font;
  }

  public void setColor(Color color)
  {
    COLOR = color;
  }

  public void setSpace(int space)
  {
    SPACE = space;
  }

  public void handleEvent() {}

  public boolean mouseDown(Event evt, int x, int y)
  {
    Hierarchy value = mouseInside(x, y);

    if (value != null)
      {
	if (! value.deadend_)
	  {
	    if (x < value.self_.x + TRI_HEIGHT + SPACE)
	      {
		value.collapsed_ = ! value.collapsed_;
		if (value.contained_.isEmpty())
		  value.handleEvent();
		repaint();
	      }
	  }
	else
	  {
	    if (value.contained_.isEmpty())
	      value.handleEvent();
	  }
      }
    
    return false;
  }
  
  public boolean mouseMove(Event evt, int x, int y)
  {
    Hierarchy value = mouseInside(x, y);

    if ((value != last_lit_) && (last_lit_ != null))
      last_lit_.highlight(getGraphics(), false);
    
    if (value != null && value.deadend_)
      value.highlight(getGraphics(), true);
    
    last_lit_ = value;
    
    return false;
  }

  public void paint(Graphics g)
  {
    Dimension d = size();

    drawLevel(g, SPACE, SPACE);

    if (d.width < scope_.width || d.height < scope_.height)
	resize(scope_.width, scope_.height);
  }

  
  private Point drawLevel(Graphics g, int x, int y)
  {
    g.setFont(FONT);
    FontMetrics font_metrics = g.getFontMetrics();
    int string_height = font_metrics.getHeight();
    int string_width = font_metrics.stringWidth(name_); 
    int entry_height = string_height;
    int entry_width = string_width + 2*SPACE + TRI_HEIGHT;
    int total_height, total_width, initial_x = x, initial_y = y;
    int indent = SPACE + TRI_HEIGHT/2;
    int temp_x = x;
    Point place_marker;
    Hierarchy entry;

    if (icon_ != null)
      {
	entry_height = Math.max(string_height, icon_.iconSize().height);
	entry_width += icon_.iconSize().width;
	indent += icon_.iconSize().width/2;
	if (! deadend_)
	  drawShape(g, x, y + (entry_height - TRI_HEIGHT)/2, deadend_, collapsed_);
      }
    else
      drawShape(g, x, y + (entry_height - TRI_HEIGHT)/2, deadend_, collapsed_);
    
    self_ = new Rectangle(initial_x, initial_y, entry_width, entry_height);

    temp_x += TRI_HEIGHT + SPACE;

    if (icon_ != null)
      {
	icon_.drawIcon(g, temp_x, y, root_);
	temp_x += SPACE + icon_.iconSize().width;
      }

    g.setColor(COLOR);
   
    g.drawString(name_, temp_x, y + (entry_height + string_height)/2);

    total_width = entry_width;
    y += entry_height + SPACE;
    
    if (! (deadend_ || collapsed_))
      {	
	x += indent;
	for (int i = 0; i < contained_.size(); i++)
	  {
	    entry = (Hierarchy)contained_.elementAt(i);
	    place_marker = entry.drawLevel(g, x, y);
	    total_width = Math.max(total_width, entry.scope_.width + indent + SPACE);
	    x = place_marker.x;
	    y = place_marker.y;
	  }
	x -= indent;
      }     

    total_height = y - initial_y;
    scope_ = new Rectangle(initial_x, initial_y, total_width, total_height);
    
    return new Point(x, y);
  }

  private Hierarchy mouseInside(int x, int y)
  {
    Hierarchy entry;
    Hierarchy return_value = null;

    if (self_.inside(x, y))
      {
	return_value = this;
      }
    else
      {
	if (scope_.inside(x, y) && (! collapsed_))
	  {
	    for (int i = 0; i < contained_.size(); i++)
	      {
		entry = (Hierarchy)contained_.elementAt(i);
		if ((return_value = entry.mouseInside(x, y)) != null)
		  break;		
	      }
	  }
	else
	  return_value =  null;
      }

    return return_value;
  }

  private void highlight(Graphics g, boolean lit)
  {  
    g.setFont(FONT);
    FontMetrics fm = g.getFontMetrics();
    int string_height = fm.getHeight();
    int x = self_.x + SPACE + TRI_HEIGHT;
    int y = self_.y;

    if (icon_ != null)
      x += icon_.iconSize().width + SPACE;
    
    if (lit) 
      g.setColor(Color.magenta);
    else
      g.setColor(COLOR);
    
    g.drawString(name_, x, y + (self_.height + string_height)/2);
  } 

  
  private static void drawShape(Graphics g, int x, int y, boolean dead_end, boolean collapsed)   
  {
    int xpoints[] = new int[3];
    int ypoints[] = new int[3];

    xpoints[0] = x;
    ypoints[0] = y;

    if (dead_end)
      {
	g.fillOval(x, y, TRI_HEIGHT, TRI_HEIGHT);
      }
    else
      {
	// Draw a small collapsed triangle: |>
	if (collapsed)
	  {
	    xpoints[1] = x;
	    xpoints[2] = x + TRI_HEIGHT;
	    ypoints[1] = y + TRI_HEIGHT;
	    ypoints[2] = y + TRI_HEIGHT/2;
	    g.fillPolygon(xpoints, ypoints, 3);
	  }
	//Draw a small uncollapsed triangle
	else
	  {
	    xpoints[1] = x + TRI_HEIGHT;
	    xpoints[2] = x + TRI_HEIGHT/2;
	    ypoints[1] = y;
	    ypoints[2] = y + TRI_HEIGHT;
	    g.fillPolygon(xpoints, ypoints, 3);
	  }
      }
  }

 private static void drawVertHashedLine(Graphics g, int x, int y1, int y2, int hashsize)
  {
    for (int ytemp = y1; ytemp < y2; ytemp += hashsize*3)
      {
	if (y2 - hashsize < ytemp)
	  g.drawLine(x, ytemp, x, y2);
	else
	  g.drawLine(x, ytemp, x, ytemp + hashsize);
      }
  }
  
  private static void drawHoroHashedLine(Graphics g, int y, int x1, int x2, Color color, int hashsize)
  {
    for (int xtemp = x1; xtemp < x2; xtemp += hashsize*3)
      {
	if (x2 - hashsize < xtemp)
	  g.drawLine(xtemp, y, x1, y);
	else
	  g.drawLine(xtemp, y, xtemp + hashsize, y);
      }
  }
}


