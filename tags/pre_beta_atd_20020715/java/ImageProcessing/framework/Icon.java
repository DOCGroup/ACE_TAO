package imaging.framework;

import java.awt.*;
import java.applet.*;
import java.net.*;

public class Icon
{
  protected Image icon_;
  protected Dimension iconSize_;
  
  Icon()
  {
    super();
  }
  
  Icon(String file_name, Component comp)
  {
    this.icon_ = Toolkit.getDefaultToolkit().getImage(file_name);
    this.loadImage(comp);
  }
  
  Icon(Image icon_image, Component comp)
  {
    this.icon_ = icon_image;
    this.loadImage(comp);
  }

  Icon(String url, Applet applet) throws MalformedURLException
  {
    this.icon_ = applet.getImage(new URL(url));
    loadImage(applet);
  }
  
  public void drawIcon(Graphics g, int x, int y, Component comp)
  {
    g.drawImage(this.icon_, x, y, comp);
  }
  
  private void loadImage(Component comp)
  {
    try
      {
	MediaTracker tracker = new MediaTracker(comp);
	tracker.addImage(this.icon_, 0);
        tracker.waitForID(0);

	this.iconSize_ = new Dimension(this.icon_.getWidth(comp), this.icon_.getHeight(comp));
      }
    catch (InterruptedException excp)
      {
        System.err.println("Icon::getIcon image failed to load");
      }
  }

  public Dimension iconSize()
  {
    return this.iconSize_;
  }

  public Image getIconImage()
  {
    return this.icon_;
  }

}




